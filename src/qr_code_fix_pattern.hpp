#ifndef QR_CODE_GEN_QR_CODE_FIX_PATTERN_HPP
#define QR_CODE_GEN_QR_CODE_FIX_PATTERN_HPP

#include "qr_code_gen/qr_code_common_defs.hpp"
#include <vector>
namespace qr_code_gen {

class QrFixPattern {
public:
  QrFixPattern(int xpos, int ypos, std::vector<std::vector<QrModuleType_t>> pattern);
  void apply_fix_pattern(std::vector<std::vector<QrModuleType_t>>& matrix) const;

private:
  int xpos;
  int ypos;
  std::vector<std::vector<QrModuleType_t>> pattern;
};


} // namespace qr_code_gen


#endif // QR_CODE_GEN_QR_CODE_FIX_PATTERN_HPP