#ifndef QR_CODE_GEN_QR_CODE_DRAWER_HPP
#define QR_CODE_GEN_QR_CODE_DRAWER_HPP

#include "qr_code_common_defs.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace qr_code_gen {

typedef struct {
  std::string output_file_path;
  int foreground_color = 0x000000;
  int background_color = 0xFFFFFF;
} QrDrawerConfig_t;


class QrDrawer {
public:
  QrDrawer(const QrDrawerConfig_t & config = QrDrawerConfig_t{});
  void draw(const std::vector<std::vector<QrModuleType_t>>& matrix) const;

private:
  QrDrawerConfig_t config;
};

} // namespace qr_code_gen


#endif // QR_CODE_GEN_QR_CODE_DRAWER_HPP