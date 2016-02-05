#include "cinder/Log.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "BarcodeScanner.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace kp::barcode;

class BarcodeBasicApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
	BarcodeScannerRef mBarcodeScanner;
};

void BarcodeBasicApp::setup() {
	// Change "COM3" to the serial port your scanner is on.
	mBarcodeScanner = BarcodeScanner::create("COM3");

	mBarcodeScanner->getSignalBarcodeScanned()->connect([](std::string barcode) { //
		CI_LOG_V("Scanned: " + barcode);																						//
	});
}

void BarcodeBasicApp::mouseDown(MouseEvent event) {
	mBarcodeScanner->beep();
}

void BarcodeBasicApp::keyDown(cinder::app::KeyEvent event) {
	switch (event.getChar()) {
		case KeyEvent::KEY_b:
			CI_LOG_V("Beep");
			mBarcodeScanner->beep();
			break;
		case KeyEvent::KEY_e:
			CI_LOG_V("Enable");
			mBarcodeScanner->enable();
			break;
		case KeyEvent::KEY_d:
			CI_LOG_V("Disable");
			mBarcodeScanner->disable();
			break;
		case KeyEvent::KEY_a:
			CI_LOG_V("Aim On");
			mBarcodeScanner->aimOn();
			break;
		case KeyEvent::KEY_o:
			CI_LOG_V("Aim Off");
			mBarcodeScanner->aimOff();
			break;
		case KeyEvent::KEY_i:
			CI_LOG_V("Illumination On");
			mBarcodeScanner->illuminationOn();
			break;
		case KeyEvent::KEY_f:
			CI_LOG_V("Illumination Off");
			mBarcodeScanner->illuminationOff();
			break;
		case KeyEvent::KEY_k:
			CI_LOG_V("LED Off");
			mBarcodeScanner->ledOff();
			break;
		case KeyEvent::KEY_l:
			CI_LOG_V("LED On");
			mBarcodeScanner->ledOn();
			break;
		case KeyEvent::KEY_s:
			CI_LOG_V("Start Session");
			mBarcodeScanner->startSession();
			break;
		case KeyEvent::KEY_n:
			CI_LOG_V("StopSession");
			mBarcodeScanner->stopSession();
			break;
		case KeyEvent::KEY_z:
			CI_LOG_V("Sleep");
			mBarcodeScanner->sleep();
			break;
		case KeyEvent::KEY_w:
			CI_LOG_V("Wake");
			mBarcodeScanner->wake();
			break;
	}
}

void BarcodeBasicApp::update() {
}

void BarcodeBasicApp::draw() {
	gl::clear(Color(0, 0, 0));
}

CINDER_APP(BarcodeBasicApp, RendererGl)
