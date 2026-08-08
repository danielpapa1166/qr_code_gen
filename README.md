# qr_code_gen

QR code generator repository. 
Playground repo on the first place, if it will work I plan to use this in https://github.com/danielpapa1166/bkk_display

## Scope

This generator targets QR codes for Wi-Fi connection payloads and short links. It
uses a fixed Version 3, error-correction level M implementation: 42 byte-mode
payload bytes with 26 error-correction codewords, balancing capacity and scan
reliability for these use cases.
