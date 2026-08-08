#include "qr_code_gen/qr_code_encode.hpp"
#include "qr_code_gen/qr_code_matrix.hpp"
#include "qr_code_gen/qr_code_drawer.hpp"

#include <iostream>

int main()
{
  qr_code_gen::QrEncode encoder("www.wikipedia.org");
  encoder.display_encoded_data_as_hex();
  printf("Encoded data: %s\n", encoder.get_encoded_data().c_str());
  return 0;
}