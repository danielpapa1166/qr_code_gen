#ifndef QR_CODE_GEN_QR_CODE_MATRIX_HPP
#define QR_CODE_GEN_QR_CODE_MATRIX_HPP

#include "qr_code_common_defs.hpp"

#include <cstdint>
#include <vector>

namespace qr_code_gen {

class QrMatrix {
public:
  QrMatrix(const std::vector<uint8_t> & data_buffer);
  void display_matrix() const;
  std::vector<std::vector<QrModuleType_t>> get_matrix() const { return matrix; }

private: 
  std::vector<std::vector<QrModuleType_t>> matrix;
};

} // namespace qr_code_gen

#endif // QR_CODE_GEN_QR_CODE_MATRIX_HPP