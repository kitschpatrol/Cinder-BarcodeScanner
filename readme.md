# Cinder-BarcodeScanner

**[Cinder](https://libcinder.org) block for communicating with barcode scanners using the Simple Serial Interface.**

## Overview

Set settings and receive scan data from an external

For simple applications, most barcode scanners can emulate a keyboard — which is certainly the easiest and most convnient way to go. However, the commands exposed through the serial interface may be useful if you need to turn a barcode scanner on / off programmatically — or perhaps if you'd like to make the scanner beep arbitrarily.

It's been tested only with the [Motorola / Zebra DS457 scanner](https://www.zebra.com/us/en/products/spec-sheets/scanners/fixed-mount-scanners/ds457.html), but will probably work with other models.

This library might work on a Mac if the right serial adapter is used with the scanner, but it's only been tested on Windows with first-party USB → serial drivers.

## Getting Started

First, set up your barcode scanner:

If you're using a USB barcode scanner, obtain the appropriate USB CDC driver from the Downloads tab on [Zebra's DS457 support page](https://www.zebra.com/us/en/support-downloads/scanners/fixed-mount-scanners/ds457.html).

Scan the barcodes in `docs/Barcode Scanner Setup.pdf` to configure your scanner to work with this library. Scanning these codes does the following:

- Resets your scanner to factory settings.
- Tells your scanner to use the SSI over USB CDC protocol.
- Disables acknowledgments. This means you can send any command packets you'd like, but you will only ever receive scanned barcodes values in ASCII from the scanner.

On Windows, find the serial port name of your scanner manually through the Device Manager or other means.

Then run samples/BarcodeBasic, and pass the serial port name of your scanner into the `Scanner::create` function.

## Related documentation

- [Full SSI protocol documentation (PDF, mirrored by BSR)](https://www.bsr.at/wp-content/uploads/DS3600_Simple-Serial-Interface-Programmer-Guide.pdf).
- [DS457 Integration Guide](https://www.zebra.com/content/dam/support-dam/en/documentation/unrestricted/guide/product/ds457-integration-guide-en-us.pdf)
