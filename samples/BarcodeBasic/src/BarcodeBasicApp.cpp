#include "cinder/Log.h"
#include "cinder/Rand.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"

#include "BarcodeScanner.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace kp::barcode;

class BarcodeBasicApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
	BarcodeScannerRef mBarcodeScanner;
	ci::params::InterfaceGlRef mParams;
};

void BarcodeBasicApp::setup() {
	// Change "COM3" to the serial port your scanner is on.
	mBarcodeScanner = BarcodeScanner::create("COM3");

	mBarcodeScanner->getSignalBarcodeScanned()->connect([](std::string barcode) { //
		CI_LOG_V("Scanned: " + barcode);																						//
	});

	// Not all scanners will support all of these options.
	mParams = params::InterfaceGl::create("Params", ivec2(300, 400));
	mParams->addText("Status");
	mParams->addParam<std::string>("Barcode", [](std::string value) {}, [&]() { return mBarcodeScanner->getLatestBarcode(); });
	mParams->addParam<std::string>("Connected", [](std::string value) {}, [&]() { return mBarcodeScanner->isConnected() ? "Yes" : "No"; });
	mParams->addSeparator();
	mParams->addText("Controls");
	mParams->addButton("Beep", std::bind(&BarcodeScanner::beep, mBarcodeScanner), "key=b");
	mParams->addButton("Enable", std::bind(&BarcodeScanner::enable, mBarcodeScanner), "key=e");
	mParams->addButton("Disable", std::bind(&BarcodeScanner::disable, mBarcodeScanner), "key=d");
	mParams->addButton("Aim On", std::bind(&BarcodeScanner::aimOn, mBarcodeScanner), "key=a");
	mParams->addButton("Aim Off", std::bind(&BarcodeScanner::aimOff, mBarcodeScanner), "key=f");
	mParams->addButton("Illumination On", std::bind(&BarcodeScanner::illuminationOn, mBarcodeScanner), "key=i");
	mParams->addButton("Illumination Off", std::bind(&BarcodeScanner::illuminationOff, mBarcodeScanner), "key=n");
	mParams->addButton("LED On", std::bind(&BarcodeScanner::ledOn, mBarcodeScanner), "key=l");
	mParams->addButton("LED Off", std::bind(&BarcodeScanner::ledOff, mBarcodeScanner), "key=o");
	mParams->addButton("Start Session", std::bind(&BarcodeScanner::startSession, mBarcodeScanner), "key=s");
	mParams->addButton("Stop Session", std::bind(&BarcodeScanner::stopSession, mBarcodeScanner), "key=p");
	mParams->addButton("Sleep", std::bind(&BarcodeScanner::sleep, mBarcodeScanner), "key=z");
	mParams->addButton("Wake", std::bind(&BarcodeScanner::wake, mBarcodeScanner), "key=w");
	mParams->addButton("Simulate",
										 [&]() {
											 std::string randomBarcode = std::to_string(randInt(100000000, 999999999));
											 mBarcodeScanner->simulateScan(randomBarcode);
											 mBarcodeScanner->beep();
										 },
										 "key=b");
}

void BarcodeBasicApp::update() {
	// Barcode reader hooks into the update signal on its own as necessary
}

void BarcodeBasicApp::draw() {
	gl::clear(Color(0, 0, 0));
	mParams->draw();
}

CINDER_APP(BarcodeBasicApp, RendererGl)
