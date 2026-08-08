#ifndef QR_CODE_GEN_QR_CODE_DRAWER_HPP
#define QR_CODE_GEN_QR_CODE_DRAWER_HPP
#include <cstdint>
#include <vector>
#include <string>
namespace qr_code_gen {

typedef struct {
  std::string output_file_path;
  int foreground_color;
  int background_color;
} QrDrawerConfig_t;


class QrDrawer {
public:
  QrDrawer(const QrDrawerConfig_t & config = QrDrawerConfig_t{});
  void draw(const std::vector<std::vector<uint8_t>>& matrix) const;

private:
  QrDrawerConfig_t config;
};

} // namespace qr_code_gen


#endif // QR_CODE_GEN_QR_CODE_DRAWER_HPP