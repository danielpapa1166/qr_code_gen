#include "qr_code_gen/qr_code_matrix.hpp"
#include "qr_code_fix_pattern.hpp"
#include <cstdio>
#include <vector>

namespace qr_code_gen {

using Pattern = std::vector<std::vector<QrModuleType_t>>;

constexpr QrModuleType_t black{
  QRMODULE_BLACK,
  QRMODULE_RESERVED
};

constexpr QrModuleType_t white{
  QRMODULE_WHITE,
  QRMODULE_RESERVED
};

const Pattern finder_pattern{
  {white, white, white, white, white, white, white, white, white},
  {white, black, black, black, black, black, black, black, white},
  {white, black, white, white, white, white, white, black, white},
  {white, black, white, black, black, black, white, black, white},
  {white, black, white, black, black, black, white, black, white},
  {white, black, white, black, black, black, white, black, white},
  {white, black, white, white, white, white, white, black, white},
  {white, black, black, black, black, black, black, black, white},
  {white, white, white, white, white, white, white, white, white},
};

const Pattern alignment_pattern{
  {black, black, black, black, black},
  {black, white, white, white, black},
  {black, white, black, white, black},
  {black, white, white, white, black},
  {black, black, black, black, black},
};

const Pattern timing_pattern_vertical{
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}
};

const Pattern timing_pattern_horizontal{
    {
        black, 
        white, 
        black, 
        white, 
        black, 
        white, 
        black, 
        white, 
        black, 
        white, 
        black, 
        white, 
        black
    }
};


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
    std::vector<QrModuleType_t>(
        29, {QRMODULE_UNSET, QRMODULE_FREE}
    )
  );

  QrFixPattern top_left(-1, -1, finder_pattern);
  top_left.apply_fix_pattern(matrix);

  QrFixPattern top_right(21, -1, finder_pattern);
  top_right.apply_fix_pattern(matrix);

  QrFixPattern bottom_left(0, 21, finder_pattern);
  bottom_left.apply_fix_pattern(matrix);

  QrFixPattern alignment(20, 20, alignment_pattern);
  alignment.apply_fix_pattern(matrix);

  QrFixPattern timing_vertical(6, 8, timing_pattern_vertical);
  timing_vertical.apply_fix_pattern(matrix);

  QrFixPattern timing_horizontal(8, 6, timing_pattern_horizontal);
  timing_horizontal.apply_fix_pattern(matrix);

  int bit_index = 0;
  bool upwards = true;

  for (int right = 28; right >= 1; right -= 2) {
    if (right == 6) {
      right--;
    }

    for (int offset = 0; offset < 29; ++offset) {
      const int row = upwards ? 28 - offset : offset;

      for (int column = right; column >= right - 1; --column) {
        if (matrix[row][column].reserved == QRMODULE_RESERVED) {
          continue;
        }

        const bool bit = data_buffer[bit_index / 8] & (1 << (7 - (bit_index % 8)));
        bit_index++;
        matrix[row][column].color = QRMODULE_WHITE;
      }
    }

    upwards = !upwards;
  }
}

void QrMatrix::display_matrix() const {
  // display the QR code matrix
  for(const auto & row : matrix) {
    for(const auto & module : row) {
      switch(module.color) {
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
