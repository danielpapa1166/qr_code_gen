#ifndef QR_CODE_GEN_QR_CODE_ENCODE_HPP
#define QR_CODE_GEN_QR_CODE_ENCODE_HPP

#include <stdint.h>
#include <string>
#include <vector>
#define MAX_DATA_LENGTH 100

namespace qr_code_gen {

class QrEncode {
public:
  QrEncode(const std::string &data);
  std::string get_encoded_data() const;
  void display_encoded_data_as_hex() const;

private:
  std::vector<uint8_t> data_buffer;
};

} // namespace qr_code_gen

#endif // QR_CODE_GEN_QR_CODE_ENCODE_HPP