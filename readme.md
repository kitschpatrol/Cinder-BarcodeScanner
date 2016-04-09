# Cinder-BarcodeScanner

**[Cinder](https://libcinder.org) block for communicating with barcode scanners using the Simple Serial Interface.**

##Overview

Set settings and receive scan data from an external 

For simple applications, most barcode scanners can emulate a keyboard — which is certainly the easiest and most convnient way to go. However, the commands exposed through the serial interface may be useful if you need to turn a barcode scanner on / off programmatically — or perhaps if you'd like to make the scanner beep arbitrarily.

It's been tested only with the [Motorola / Zebra DS457 scanner](https://www.zebra.com/us/en/products/scanners/fixed-mount-scanners/ds457/ds457-spec-sheet-en.pdf), but will probably work with other models.

This library might work on a Mac if the right serial adapter is used with the scanner, but it's only been tested on Windows with first-party USB → serial drivers.

##Getting Started

First, set up your barcode scanner:

If you're using a USB barcode scanner, install the [latest USB CDC driver from Motorola's support site](https://portal.motorolasolutions.com/Support/US-EN/Resolution?solutionId=5201&redirectForm=search&searchQuery=%3fsearchType=simple&searchTerm=usb%2520cdc%2520scanner%2520driver).

Scan the barcodes in `docs/Barcode Scanner Setup.pdf` to configure your scanner to work with this library.  Scanning these codes does the following:

- Resets your scanner to factory settings. 
- Tells your scanner to use the SSI over USB CDC protocol.
- Disables acknowledgments. This means you can send any command packets you'd like, but you will only ever receive scanned barcodes values in ASCII from the scanner.

Due to [issues in Cinder with virtual device discovery on Windows](https://forum.libcinder.org/topic/rfc-windows-serial-device-discovery-rewrite), you'll need to find the serial port name of your scanner manually through the Device Manager or other means.

Then run samples/BarcodeBasic, and pass the serial port name of your scanner into the `Scanner::create` function.

##Related documentation

- [Full SSI protocol documentation](https://atgsupportcentral.motorolasolutions.com/content/emb/docs/manuals/4045103a.pdf).
- [DS 457 Integration Guide](https://atgsupportcentral.motorolasolutions.com/content/emb/docs/manuals/14436104a.pdf)