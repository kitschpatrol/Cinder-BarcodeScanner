#pragma once

#include "cinder/Cinder.h"
#include "cinder/Log.h"
#include "cinder/Timer.h"
#include "cinder/app/App.h"

namespace kp {
namespace barcode {

typedef std::shared_ptr<class BarcodeScannerKeyboard> BarcodeScannerKeyboardRef;

class BarcodeScannerKeyboard : public std::enable_shared_from_this<BarcodeScannerKeyboard> {
public:
	static BarcodeScannerKeyboardRef create();
	~BarcodeScannerKeyboard();

	void simulateScan(std::string barcode);
	const std::string getLatestBarcode() const;
	ci::signals::Signal<void(std::string)> *getSignalBarcodeScanned();

protected:
	BarcodeScannerKeyboard();

private:
	ci::signals::Signal<void(std::string)> mSignalBarcodeScanned;

	ci::signals::Connection mSignalConnectionKeyboard;
	void keyUp(ci::app::KeyEvent event);

	ci::Timer mKeyTimer;
	std::string mKeyReadBuffer;
	std::string mLatestBarcode;

	const double MAX_KEY_INTERVAL = 0.1; // seconds
	const int BARCODE_LENGTH = 19;
};

} // namespace barcode
} // namespace kp