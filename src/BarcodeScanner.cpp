#include "BarcodeScanner.h"

#include <numeric>

#include "cinder/Log.h"
#include "cinder/app/App.h"

namespace kp {
namespace barcode {

BarcodeScannerRef BarcodeScanner::create(std::string serialPortName) {
	return BarcodeScannerRef(new BarcodeScanner(serialPortName))->shared_from_this();
}

BarcodeScanner::BarcodeScanner(std::string serialPortName) {
	mSerialPortName = serialPortName;
	mSignalConnectionUpdate = ci::app::App::get()->getSignalUpdate().connect(std::bind(&BarcodeScanner::update, this));
	mSerialTimer.start();
}

BarcodeScanner::~BarcodeScanner() {
	mSignalConnectionUpdate.disconnect();
}

bool BarcodeScanner::isConnected() {
	// check pointer
	if (mSerial == nullptr) {
		return false;
	}

	// test connection
	try {
		mSerial->getNumBytesAvailable();
	} catch (std::exception &exc) {
		//CI_LOG_EXCEPTION(Serial device not connected", exc);
		return false;
	}

	// probable there
	return true;
}

void BarcodeScanner::connect() {
	// Discovery of virtual devices is broken, so we have to pass in the port name manually:
	// https://forum.libcinder.org/topic/rfc-windows-serial-device-discovery-rewrite
	try {
		ci::Serial::Device dev = ci::Serial::Device(mSerialPortName);
		mSerial = ci::Serial::create(dev, 9600);
	} catch (std::exception &exc) {
		//CI_LOG_EXCEPTION("Could not initialize the serial device", exc);
		return;
	}

	mSerial->flush();
}

void BarcodeScanner::update() {
	// Read from serial
	if (isConnected()) {
		while ((mSerial->getNumBytesAvailable() > 0)) {
			mSerialReadBuffer += mSerial->readChar();
			mSerialTimer.start(); // reset the timer
		}

		if (!mWasConnected) {
			CI_LOG_V("Successfully connected to barcode scanner on port " << mSerialPortName);
			mWasConnected = true;
		}

	} else {
		if (mWasConnected) {
			CI_LOG_V("Attempting connection to barcode scanner on port " << mSerialPortName);
			mWasConnected = false;
		}

		connect();
	}

	// If we have something, and we haven't seen any bytes over seral in a while, then send out a barcode signal
	if ((mSerialReadBuffer.size() > 0) && (mSerialTimer.getSeconds() > MAX_RECEIVE_INTERVAL)) {
		mLatestBarcode = mSerialReadBuffer;
		mSignalBarcodeScanned.emit(mLatestBarcode);
		mSerialReadBuffer = "";
	}
}

void BarcodeScanner::sendMessage(uint8_t opCode, std::vector<uint8_t> parameters) {
	if (isConnected()) {

		std::vector<uint8_t> message = {opCode};
		message.push_back(0x04); // Message source, we're the host
		message.push_back(0x00); // Status, First transmission, temporary change

		// Add any parameters, if any
		for (uint8_t param : parameters) {
			message.push_back(param);
		}

		// Prepend length
		message.insert(message.begin(), message.size() + 1);

		// Add checksum
		uint16_t checksum = calculateChecksum(message);
		message.push_back((checksum >> 8) & 0xff); // high byte
		message.push_back(checksum & 0xff);				 // low byte

		// Send it
		mSerial->writeBytes(&message[0], message.size());
	}
};

uint16_t BarcodeScanner::calculateChecksum(std::vector<uint8_t> bytes) {
	// Twos complement of the sum of the message
	int sum = std::accumulate(bytes.begin(), bytes.end(), 0);
	uint16_t sum16 = sum % 255;
	uint16_t twosComplement = ~sum16 + 1;
	return twosComplement;
}

void BarcodeScanner::simulateScan(std::string barcode) {
	mLatestBarcode = barcode;
	mSignalBarcodeScanned.emit(mLatestBarcode);
}

const std::string BarcodeScanner::getLatestBarcode() const {
	return mLatestBarcode;
}

ci::signals::Signal<void(std::string)> *BarcodeScanner::getSignalBarcodeScanned() {
	return &mSignalBarcodeScanned;
}

void BarcodeScanner::enable() {
	sendMessage(0xE9);
}

void BarcodeScanner::disable() {
	sendMessage(0xEA);
}

void BarcodeScanner::beep() {
	sendMessage(0xE6, {0x00});
}

void BarcodeScanner::aimOn() {
	sendMessage(0xC5);
}

void BarcodeScanner::aimOff() {
	sendMessage(0xC4);
}

void BarcodeScanner::illuminationOff() {
	sendMessage(0xC0);
}

void BarcodeScanner::illuminationOn() {
	sendMessage(0xC1);
}

void BarcodeScanner::ledOff() {
	sendMessage(0xE8);
}

void BarcodeScanner::ledOn() {
	sendMessage(0xE7);
}

void BarcodeScanner::startSession() {
	sendMessage(0xE4);
}

void BarcodeScanner::stopSession() {
	sendMessage(0xE5);
}

void BarcodeScanner::sleep() {
	sendMessage(0xEB);
}

void BarcodeScanner::wake() {
	if (isConnected()) {
		mSerial->writeByte(0);
	}
}

} // namespace barcode
} // namespace kp
