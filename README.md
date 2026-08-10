# qr_code_gen

QR code generator repository. Playground repo on the first place, if it will
work I plan to use this in https://github.com/danielpapa1166/bkk_display.

## Scope

This generator targets QR codes for Wi-Fi connection payloads and short links. It
uses a fixed Version 3, error-correction level M implementation: 42 byte-mode
payload bytes with 26 error-correction codewords, balancing capacity and scan
reliability for these use cases.

## How It Works

The encoder produces 44 byte-mode data codewords, adds 26 Reed-Solomon error
correction codewords, and places the resulting 70 codewords plus seven remainder
bits into a 29 x 29 Version 3 matrix. The matrix builder adds the fixed QR
patterns, format information, and mask pattern 2. The generated matrix remains
in memory for the integrating application to render.

The implementation is in active development and generated codes should be
verified with a QR reader before deployment.

## Build And Run

Install CMake and a C++17 compiler. On Debian or Ubuntu:

```bash
sudo apt install cmake g++
cmake -S . -B build
cmake --build build
./build/test/qr_code_gen_sanity "https://example.com"
```

The sanity application generates and validates an in-memory QR matrix. Use
`QrMatrix::get_matrix()` in an integrating application to render its modules.

## Yocto Integration

Use CMake. A minimal recipe is:

```bitbake
SUMMARY = "Version 3-M QR code generator"
LICENSE = "CLOSED"

SRC_URI = "git://example.com/qr_code_gen.git;protocol=https;branch=main"
SRCREV = "<commit-id>"
S = "${WORKDIR}/git"

inherit cmake

EXTRA_OECMAKE += "-DQR_CODE_GEN_BUILD_SANITY_APP=OFF"
```

The library exposes the generated QR code as an in-memory matrix through
`QrMatrix::get_matrix()`. The Yocto application can render those modules
directly on its display without creating an image file.

Add the resulting package to an image with:

```bitbake
IMAGE_INSTALL:append = " qr-code-gen"
```
