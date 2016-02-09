#pragma once

#include "cinder/Cinder.h"
#include "cinder/Serial.h"
#include "cinder/Signals.h"
#include "cinder/Timer.h"

namespace kp {
namespace barcode {

typedef std::shared_ptr<class BarcodeScanner> BarcodeScannerRef;

class BarcodeScanner : public std::enable_shared_from_this<BarcodeScanner> {
public:
	static BarcodeScannerRef create(std::string serialPortName);
	~BarcodeScanner();

	// See Motorola's documentation for more info on these.
	// https://atgsupportcentral.motorolasolutions.com/content/emb/docs/manuals/4045103a.pdf
	void beep();
	void enable();
	void disable();
	void aimOn();
	void aimOff();
	void illuminationOn();
	void illuminationOff();
	void ledOn();
	void ledOff();
	void startSession();
	void stopSession();
	void sleep();
	void wake();

	void simulateScan(std::string barcode);
	const std::string getLatestBarcode() const;
	ci::signals::Signal<void(std::string)> *getSignalBarcodeScanned();

protected:
	BarcodeScanner(std::string serialPortName);

private:
	ci::SerialRef mSerial;
	void sendMessage(uint8_t opCode, std::vector<uint8_t> parameters = {});
	uint16_t calculateChecksum(std::vector<uint8_t> bytes);

	ci::signals::Signal<void(std::string)> mSignalBarcodeScanned;
	ci::signals::Connection mSignalConnectionUpdate;
	void update();

	ci::Timer mSerialTimer;
	std::string mSerialReadBuffer;
	std::string mLatestBarcode;

	const double MAX_RECEIVE_INTERVAL = 0.01; // seconds
};

} // namespace barcode
} // namespace kp
