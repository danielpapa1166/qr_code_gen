#include "qr_code_fix_pattern.hpp"


namespace qr_code_gen {

QrFixPattern::QrFixPattern(int xpos, int ypos, std::vector<std::vector<QrModuleType_t>> pattern)
    : xpos(xpos), ypos(ypos), pattern(std::move(pattern)) {

  (void)pattern; // suppress unused variable warning

}

void QrFixPattern::apply_fix_pattern(std::vector<std::vector<QrModuleType_t>>& matrix) const {
  for (std::size_t row = 0; row < pattern.size(); ++row) {
    for (std::size_t col = 0; col < pattern[row].size(); ++col) {
      const int target_y = ypos + static_cast<int>(row);
      const int target_x = xpos + static_cast<int>(col);

      if (target_y < 0 || target_x < 0 ||
          target_y >= static_cast<int>(matrix.size()) ||
          target_x >= static_cast<int>(matrix[target_y].size())) {
        continue;
      }

      matrix[target_y][target_x] = pattern[row][col];
    }
  }

}
} // namespace qr_code_gen