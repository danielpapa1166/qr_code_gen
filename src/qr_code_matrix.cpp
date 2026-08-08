#include "qr_code_gen/qr_code_matrix.hpp"
#include <cstdio>

namespace qr_code_gen {

// this file contains the functionalities to
// generate a QR code matrix from encoded data

QrMatrix::QrMatrix(const std::vector<uint8_t> & data_buffer) {
  // generate the QR code matrix from the encoded data
  if(data_buffer.size() != 70) {
    return; 
  }

  // resize the matrix to 29x29 for version 1 QR code
  matrix.resize(
    29, 
    std::vector<QrModuleType_t>(29, QRMODULE_UNSET)
  );

  int bit_index = 0;
  bool upwards = true;

  for (int right = 28; right >= 1; right -= 2) {
    if (right == 6) {
      right--;
    }

    for (int offset = 0; offset < 29; ++offset) {
      const int row = upwards ? 28 - offset : offset;

      for (int column = right; column >= right - 1; --column) {
        if (matrix[row][column] != QRMODULE_UNSET) {
          continue;
        }

        const bool bit = data_buffer[bit_index / 8] & (1 << (7 - (bit_index % 8)));
        bit_index++;
        matrix[row][column] = bit ? QRMODULE_BLACK : QRMODULE_WHITE;
      }
    }

    upwards = !upwards;
  }
}

void QrMatrix::display_matrix() const {
  // display the QR code matrix
  for(const auto & row : matrix) {
    for(const auto & module : row) {
      switch(module) {
        case QRMODULE_UNSET:
          printf("?");
          break;
        case QRMODULE_WHITE:
          printf(" ");
          break;
        case QRMODULE_BLACK:
          printf("#");
          break;
      }
    }
    printf("\n");
  }
}
} // namespace qr_code_gen
