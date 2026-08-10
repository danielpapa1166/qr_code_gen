#include "qr_code_gen/qr_code_encode.hpp"
#include "qr_code_gen/qr_code_matrix.hpp"

#include <exception>
#include <iostream>

// "WIFI:T:WPA;S:BKK-Display-Setup;;;"
// "http://192.168.4.1:8080"

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("Usage: %s <data_to_encode>\n", argv[0]);
    return 1;
  }

  try {
    qr_code_gen::QrEncode encoder(argv[1]);

    auto encoded_data = encoder.get_encoded_data();
    qr_code_gen::QrMatrix matrix(encoded_data);
    const auto matrix_data = matrix.get_matrix();

    if (matrix_data.empty() || matrix_data.size() != matrix_data.front().size()) {
      std::cerr << "QR matrix is not square\n";
      return 1;
    }
  } catch (const std::exception& error) {
    std::cerr << "QR generation failed: " << error.what() << '\n';
    return 1;
  }

  return 0;
}