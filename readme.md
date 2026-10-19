<!-- title({ prefix: "Cinder-" }) -->

# Cinder-BarcodeScanner

<!-- /title -->

<!-- badges -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/license/mit)

<!-- /badges -->

<!-- short-description -->

**CinderBlock for communicating with barcode scanners using the Simple Serial Interface.**

<!-- /short-description -->

## Overview

A [CinderBlock](https://libcinder.org/docs/guides/cinder-blocks/index.html) for sending commands to and receiving scan data from an external barcode scanner over a serial connection, using the Simple Serial Interface (SSI).

For simple applications, most barcode scanners can emulate a keyboard — which is certainly the easiest and most convenient way to go. However, the commands exposed through the serial interface may be useful if you need to turn a barcode scanner on / off programmatically — or perhaps if you'd like to make the scanner beep arbitrarily.

The block also includes `BarcodeScannerKeyboard`, which picks barcodes out of the key events of a scanner in keyboard emulation mode and offers the same signal-based interface.

## Getting started

### Dependencies

Requires [Cinder](https://libcinder.org/) and a barcode scanner that supports SSI. The block uses Cinder's own `ci::Serial` class, so there are no other libraries to install.

It's been tested only with the [Motorola / Zebra DS457 scanner](https://www.zebra.com/us/en/products/spec-sheets/scanners/fixed-mount-scanners/ds457.html), but will probably work with other models.

This library might work on a Mac if the right serial adapter is used with the scanner, but it's only been tested on Windows with first-party USB → serial drivers.

Most of the block was written in early 2016, and its code was last changed in June 2017, against Cinder's master branch (0.9.2dev, between the 0.9.1 and 0.9.2 releases). The sample includes projects for Visual Studio 2015 and Xcode. It hasn't been tested with later Cinder releases.

### Installation

From your Cinder `blocks` directory:

```sh
git clone https://github.com/kitschpatrol/Cinder-BarcodeScanner.git
```

Then add **BarcodeScanner** to a project with [TinderBox](https://libcinder.org/docs/guides/tinderbox/index.html).

Next, set up your barcode scanner:

If you're using a USB barcode scanner, obtain the appropriate USB CDC driver from the Downloads tab on [Zebra's DS457 support page](https://www.zebra.com/us/en/support-downloads/scanners/fixed-mount-scanners/ds457.html).

Scan the barcodes in [`docs/Barcode Scanner Setup.pdf`](docs/Barcode%20Scanner%20Setup.pdf) to configure your scanner to work with this library. Scanning these codes does the following:

- Resets your scanner to factory settings.
- Tells your scanner to use the SSI over USB CDC protocol.
- Disables acknowledgments. This means you can send any command packets you'd like, but you will only ever receive scanned barcode values in ASCII from the scanner.

Unplug and replug the scanner after scanning the last code.

On Windows, find the serial port name of your scanner manually through the Device Manager or other means. A DS457 connected over USB shows up under "Ports (COM & LPT)" as a "Motorola CDC USB Device", as in [this screenshot](docs/Com%20Port%20in%20Device%20Manager.png).

Then run [`samples/BarcodeBasic`](samples/BarcodeBasic), and pass the serial port name of your scanner into the `BarcodeScanner::create` function.

## Usage

Create a `BarcodeScanner` with the scanner's serial port name and connect to its barcode signal:

```cpp
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Log.h"

#include "BarcodeScanner.h"

using namespace kp::barcode;

class MyApp : public ci::app::App {
public:
	void setup() override {
		mBarcodeScanner = BarcodeScanner::create("COM3"); // Your scanner's serial port
		mBarcodeScanner->getSignalBarcodeScanned()->connect([](std::string barcode) {
			CI_LOG_I("Scanned: " << barcode);
		});
	}

private:
	BarcodeScannerRef mBarcodeScanner;
};

CINDER_APP(MyApp, ci::app::RendererGl)
```

The scanner hooks into the app's update signal on its own, so there's nothing to call in `update()`. It keeps trying to open the port until the scanner is available, and reconnects if the scanner is unplugged and plugged back in. Passing an empty port name disables serial communication, which is useful for testing with `simulateScan()` when no scanner is attached.

The rest of the [BarcodeScanner.h](src/BarcodeScanner.h) API:

| Method                                   | Description                                                                 |
| ---------------------------------------- | --------------------------------------------------------------------------- |
| `getLatestBarcode()`                     | Return the most recently scanned barcode.                                   |
| `isConnected()`                          | Return whether the serial port is open.                                     |
| `simulateScan(barcode)`                  | Emit a barcode through the signal as if it had been scanned.                |
| `beep()`                                 | Sound one short, high beep.                                                 |
| `enable()` / `disable()`                 | Permit or prevent scanning.                                                 |
| `aimOn()` / `aimOff()`                   | Turn the aim pattern on or off.                                             |
| `illuminationOn()` / `illuminationOff()` | Turn the illumination on or off.                                            |
| `ledOn()` / `ledOff()`                   | Turn the scanner's LEDs on or off.                                          |
| `startSession()` / `stopSession()`       | Start or abort a decode attempt.                                            |
| `sleep()` / `wake()`                     | Put the scanner into low power mode, or wake it by sending a single `NULL`. |

Not all scanners will support all of these commands.

For a scanner in keyboard emulation mode, create a `BarcodeScannerKeyboard` instead with `BarcodeScannerKeyboard::create()`, after the app's window exists. It has the same `getSignalBarcodeScanned()`, `getLatestBarcode()`, and `simulateScan()` methods, and a `beep()` that only writes "Beep!" to the log.

## Examples

### BarcodeBasic

The [BarcodeBasic sample](samples/BarcodeBasic/src/BarcodeBasicApp.cpp) connects to a scanner on `COM3`, logs each scan, and shows a parameter panel with the latest barcode, the connection status, and a button for each command. Change `"COM3"` in `setup()` to your scanner's serial port, then open [BarcodeBasic.sln](samples/BarcodeBasic/vc2015/BarcodeBasic.sln) in Visual Studio or [BarcodeBasic.xcodeproj](samples/BarcodeBasic/xcode/BarcodeBasic.xcodeproj) in Xcode.

Each button also has a keyboard shortcut:

| Key       | Action                                                   |
| --------- | -------------------------------------------------------- |
| `b`       | Beep                                                     |
| `e` / `d` | Enable / disable scanning                                |
| `a` / `f` | Aim pattern on / off                                     |
| `i` / `n` | Illumination on / off                                    |
| `l` / `o` | LEDs on / off                                            |
| `s` / `p` | Start / stop a decode session                            |
| `z` / `w` | Sleep / wake                                             |
| `r`       | Simulate a scan of a random nine-digit barcode, and beep |

## Background

### Implementation notes

`BarcodeScanner` opens the port at 9600 baud, the scanner's default. Because acknowledgments are turned off during setup, it doesn't parse SSI packets from the scanner. It treats incoming bytes as ASCII barcode text, and emits a barcode once no new bytes have arrived for 10 milliseconds.

Commands are sent as SSI packets with the host as the message source, a "temporary change" status, and a checksum.

`BarcodeScannerKeyboard` listens for key-up events on the window that was active when it was created. It collects digits that arrive less than 0.1 seconds apart, and emits a barcode once it has 19 of them.

Related documentation:

- [Full SSI protocol documentation (PDF, mirrored by BSR)](https://www.bsr.at/wp-content/uploads/DS3600_Simple-Serial-Interface-Programmer-Guide.pdf)
- [DS457 Integration Guide](https://www.zebra.com/content/dam/support-dam/en/documentation/unrestricted/guide/product/ds457-integration-guide-en-us.pdf)

### Known issues

- Cinder's serial device discovery doesn't find virtual serial devices on Windows ([forum thread](https://web.archive.org/web/20140807195749/http://forum.libcinder.org/topic/rfc-windows-serial-device-discovery-rewrite)), so the port name has to be passed in manually.
- Errors opening or reading the serial port aren't logged. While the scanner is disconnected, the block tries to reopen the port on every frame.
- `BarcodeScannerKeyboard` only handles all-digit barcodes, and expects them to be 19 digits long. It emits a barcode whenever it has collected 19 digits, so shorter barcodes are never emitted and longer ones are cut down or split into 19-digit pieces. Any non-digit key clears what it has collected.
- `BarcodeScannerKeyboard` discards the first digit that arrives more than 0.1 seconds after the previous key, so a scan is only recognized if the scanner sends another key immediately before the barcode.
- The SSI checksum is computed from the message's byte sum modulo 255 instead of the full 16-bit sum, so it would be wrong for messages whose bytes sum to 255 or more. None of the built-in commands do.

## Maintainers

[kitschpatrol](https://github.com/kitschpatrol)

<!-- contributing -->

## Contributing

[Issues](https://github.com/kitschpatrol/Cinder-BarcodeScanner/issues) are welcome and appreciated.

Please open an issue to discuss changes before submitting a pull request. Unsolicited PRs (especially AI-generated ones) are unlikely to be merged.

This repository uses [MDAT](https://github.com/kitschpatrol/mdat) for readme placeholder expansion.

<!-- /contributing -->

<!-- license -->

## License

[MIT](license.txt) © Eric Mika

<!-- /license -->
