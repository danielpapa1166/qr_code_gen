#include "qr_code_gen/qr_code_encode.hpp"
#include "qr_code_gen/qr_code_matrix.hpp"
#include "qr_code_gen/qr_code_drawer.hpp"

#include <iostream>

int main()
{
  qr_code_gen::QrEncode encoder("https://example.com");
  encoder.display_encoded_data_as_hex();

  auto encoded_data = encoder.get_encoded_data();
  qr_code_gen::QrMatrix matrix(encoded_data);
  matrix.display_matrix();

  qr_code_gen::QrDrawer drawer({
    .output_file_path = "qr_code.png",
    .foreground_color = 0x000000,
    .background_color = 0xFFFFFF
  });
  auto qr_matrix = matrix.get_matrix(); 
  drawer.draw(qr_matrix);


  return 0;
}