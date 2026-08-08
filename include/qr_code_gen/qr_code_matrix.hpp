#ifndef QR_CODE_GEN_QR_CODE_MATRIX_HPP
#define QR_CODE_GEN_QR_CODE_MATRIX_HPP

#include <cstdint>
#include <vector>
namespace qr_code_gen {

typedef enum QrModuleType {
  QRMODULE_UNSET = 0,
  QRMODULE_WHITE = 1,
  QRMODULE_BLACK = 2,
} QrModuleType_t;

class QrMatrix {
public:
  QrMatrix(const std::vector<uint8_t> & data_buffer);
  void display_matrix() const;

private: 
  std::vector<std::vector<QrModuleType_t>> matrix;
};

} // namespace qr_code_gen

#endif // QR_CODE_GEN_QR_CODE_MATRIX_HPP