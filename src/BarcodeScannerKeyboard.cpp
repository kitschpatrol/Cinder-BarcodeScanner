#include "BarcodeScannerKeyboard.h"

#include <regex>

#include "cinder/Log.h"
#include "cinder/Utilities.h"

namespace kp {
namespace barcode {

BarcodeScannerKeyboardRef BarcodeScannerKeyboard::create() {
	return BarcodeScannerKeyboardRef(new BarcodeScannerKeyboard())->shared_from_this();
}

BarcodeScannerKeyboard::BarcodeScannerKeyboard()
		: mLatestBarcode("") {
	mSignalConnectionKeyboard = ci::app::getWindow()->getSignalKeyUp().connect(std::bind(&BarcodeScannerKeyboard::keyUp, this, std::placeholders::_1));
	mKeyTimer.start();
}

BarcodeScannerKeyboard::~BarcodeScannerKeyboard() {
	mSignalConnectionKeyboard.disconnect();
}

void BarcodeScannerKeyboard::keyUp(ci::app::KeyEvent event) {
	std::string test = ci::toString(event.getChar());

	if ((mKeyTimer.getSeconds() < MAX_KEY_INTERVAL) && std::regex_match(test, std::regex("\\d"))) {
		mKeyReadBuffer += event.getChar();

		if (mKeyReadBuffer.size() == BARCODE_LENGTH) {
			mLatestBarcode = mKeyReadBuffer;
			mSignalBarcodeScanned.emit(mLatestBarcode);
			mKeyReadBuffer = "";
		}

	} else {
		mKeyReadBuffer = "";
	}

	// Reset key timer
	mKeyTimer.start();
}

void BarcodeScannerKeyboard::simulateScan(std::string barcode) {
	mLatestBarcode = barcode;
	mSignalBarcodeScanned.emit(mLatestBarcode);
}

const std::string BarcodeScannerKeyboard::getLatestBarcode() const {
	return mLatestBarcode;
}

ci::signals::Signal<void(std::string)> *BarcodeScannerKeyboard::getSignalBarcodeScanned() {
	return &mSignalBarcodeScanned;
}

} // namespace barcode
} // namespace kp