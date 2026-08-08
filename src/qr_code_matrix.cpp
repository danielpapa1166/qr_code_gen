#include "qr_code_gen/qr_code_matrix.hpp"
#include "qr_code_fix_pattern.hpp"
#include "qr_code_pattern_config.hpp"
#include <cstdio>
#include <stdexcept>
#include <vector>

namespace qr_code_gen {

static void set_module(std::vector<std::vector<QrModuleType_t>> &matrix, 
    int row, int column, bool is_black);
static uint16_t make_format_bits(int mask);
static bool bit(uint16_t bits, int index);
static void write_format_bits(std::vector<std::vector<QrModuleType_t>> &matrix, uint16_t bits);
static void apply_mask(std::vector<std::vector<QrModuleType_t>> &matrix,
  int mask);

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

  QrFixPattern bottom_left(-1, 21, finder_pattern);
  bottom_left.apply_fix_pattern(matrix);

  QrFixPattern alignment(20, 20, alignment_pattern);
  alignment.apply_fix_pattern(matrix);

  QrFixPattern timing_vertical(6, 8, timing_pattern_vertical);
  timing_vertical.apply_fix_pattern(matrix);

  QrFixPattern timing_horizontal(8, 6, timing_pattern_horizontal);
  timing_horizontal.apply_fix_pattern(matrix);

  constexpr int selected_mask = 2;
  const uint16_t format_bits = make_format_bits(selected_mask);
  write_format_bits(matrix, format_bits);

  int bit_index = 0;
  bool upwards = true;

  for (int right = 28; right >= 1; right -= 2) {
    if (right == 6) {
      right--;
    }

    for (int offset = 0; offset < 29; offset++) {
      const int row = upwards ? 28 - offset : offset;

      for (int column = right; column >= right - 1; --column) {
        if (matrix[row][column].reserved == QRMODULE_RESERVED) {
          continue;
        }

        if (bit_index >= 567) {
          throw std::logic_error("QR data placement exceeds 567 bits");
        }

        const bool bit = bit_index < 560 &&
            (data_buffer[bit_index / 8] &
             (1 << (7 - (bit_index % 8)))) != 0;
        bit_index++;
        matrix[row][column].color = bit ? QRMODULE_BLACK : QRMODULE_WHITE;
      }
    }

    upwards = !upwards;
  }

  if (bit_index != 567) {
    throw std::logic_error("QR data placement did not consume 567 bits");
  }

  apply_mask(matrix, selected_mask);
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


static void set_module(std::vector<std::vector<QrModuleType_t>> &matrix, 
    int row, int column, bool is_black) {
  // set the module at the specified position to black or white
  if (row < 0 || row >= 29 || column < 0 || column >= 29) {
    return;
  }

  matrix[row][column].color = is_black ? QRMODULE_BLACK : QRMODULE_WHITE;
  matrix[row][column].reserved = QRMODULE_RESERVED;
}

static uint16_t make_format_bits(int mask) {
  constexpr uint16_t ec_level_m = 0b00;
  uint16_t data = static_cast<uint16_t>((ec_level_m << 3) | mask);
  uint16_t remainder = data;

  for (int bit = 0; bit < 10; ++bit) {
    remainder = static_cast<uint16_t>(
        (remainder << 1) ^ ((remainder & 0x200) ? 0x537 : 0));
  }

  return static_cast<uint16_t>(((data << 10) | remainder) ^ 0x5412);
}


static bool bit(uint16_t bits, int index) {
  return (bits >> (index)) & 1;
}


static void write_format_bits(std::vector<std::vector<QrModuleType_t>> &matrix, uint16_t bits) {
  for (int index = 0; index <= 5; ++index) {
    set_module(matrix, index, 8, bit(bits, index));
  }

  set_module(matrix, 7, 8, bit(bits, 6));
  set_module(matrix, 8, 8, bit(bits, 7));
  set_module(matrix, 8, 7, bit(bits, 8));

  for (int index = 9; index <= 14; ++index) {
    set_module(matrix, 8, 14 - index, bit(bits, index));
  }

  for (int index = 0; index <= 7; ++index) {
    set_module(matrix, 8, 28 - index, bit(bits, index));
  }

  for (int index = 8; index <= 14; ++index) {
    set_module(matrix, 14 + index, 8, bit(bits, index));
  }

  set_module(matrix, 21, 8, true);  // Fixed dark module for Version 3.
}

static void apply_mask(std::vector<std::vector<QrModuleType_t>> &matrix,
    int mask)
{
  if (mask != 2) {
    throw std::invalid_argument("Only QR mask pattern 2 is supported");
  }

  for (std::size_t row = 0; row < matrix.size(); ++row) {
    for (std::size_t column = 0; column < matrix[row].size(); ++column) {
      QrModuleType_t& module = matrix[row][column];
      if (module.reserved == QRMODULE_RESERVED || column % 3 != 0) {
        continue;
      }

      module.color = module.color == QRMODULE_BLACK ? QRMODULE_WHITE :
          QRMODULE_BLACK;
    }
  }
}


} // namespace qr_code_gen
