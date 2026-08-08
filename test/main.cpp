#include "qr_code_gen/qr_code_encode.hpp"
#include "qr_code_gen/qr_code_matrix.hpp"
#include "qr_code_gen/qr_code_drawer.hpp"

#include <iostream>

// "WIFI:T:WPA;S:BKK-Display-Setup;;;"
// "http://192.168.4.1:8080"

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("Usage: %s <data_to_encode>\n", argv[0]);
    return 1;
  }

  qr_code_gen::QrEncode encoder(argv[1]);

  auto encoded_data = encoder.get_encoded_data();
  qr_code_gen::QrMatrix matrix(encoded_data);

  qr_code_gen::QrDrawer drawer({
    .output_file_path = "doc/qr_code.png",
    .foreground_color = 0x000000,
    .background_color = 0xFFFFFF
  });
  auto qr_matrix = matrix.get_matrix(); 
  drawer.draw(qr_matrix);


  return 0;
}