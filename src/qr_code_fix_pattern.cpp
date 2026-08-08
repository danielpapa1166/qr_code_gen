#include "qr_code_fix_pattern.hpp"


namespace qr_code_gen {

QrFixPattern::QrFixPattern(int xpos, int ypos, std::vector<std::vector<QrModuleType_t>> pattern)
    : xpos(xpos), ypos(ypos), pattern(std::move(pattern)) {

  (void)pattern; // suppress unused variable warning

}

void QrFixPattern::apply_fix_pattern(std::vector<std::vector<QrModuleType_t>>& matrix) const {
  for (std::size_t row = 0; row < pattern.size(); ++row) {
    for (std::size_t col = 0; col < pattern[row].size(); ++col) {
      if(ypos + row >= matrix.size() || xpos + col >= matrix[row].size()
         || ypos + row < 0 || xpos + col < 0) {
        continue; // skip if the position is out of bounds
      }
      matrix[ypos + row][xpos + col].color = pattern[row][col].color;
      matrix[ypos + row][xpos + col].reserved = QRMODULE_RESERVED;
    }
  }

}
} // namespace qr_code_gen