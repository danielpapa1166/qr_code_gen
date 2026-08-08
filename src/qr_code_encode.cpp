#include "qr_code_gen/qr_code_encode.hpp"

#include <cstddef>
#include <stdexcept>

namespace {

constexpr std::size_t max_payload_bytes = 42;
constexpr std::size_t data_codeword_count = 44;
constexpr uint8_t pad_codewords[] = {
  0xEC,
  0x11,
};

} // namespace

namespace qr_code_gen {

static uint8_t gf_multiply(uint8_t a, uint8_t b); 
static std::vector<uint8_t> rs_calculate(const std::vector<uint8_t> & data_codewords);


QrEncode::QrEncode(const std::string & data)
{
  if (data.size() > max_payload_bytes) {
    throw std::invalid_argument("Version 3-M supports at most 42 payload bytes");
  }

  data_buffer.reserve(data_codeword_count + 26);
  data_buffer.push_back(static_cast<uint8_t>(0x40 | (data.size() >> 4)));
  data_buffer.push_back(static_cast<uint8_t>(data.size() << 4));

  for (std::size_t index = 0; index < data.size(); ++index) {
    const uint8_t current = static_cast<uint8_t>(data[index]);
    const uint8_t next = index + 1 < data.size() ?
        static_cast<uint8_t>(data[index + 1]) : 0;
    data_buffer.back() |= current >> 4;
    data_buffer.push_back(static_cast<uint8_t>(current << 4 | next >> 4));
  }

  if (data_buffer.size() > data_codeword_count) {
    throw std::logic_error("QR payload exceeds Version 3-M data capacity");
  }

  while (data_buffer.size() < data_codeword_count) {
    data_buffer.push_back(
        pad_codewords[(data_buffer.size() - (data.size() + 2)) % 2]);
  }

  const auto rs_remainder = rs_calculate(data_buffer);
  data_buffer.insert(data_buffer.end(), rs_remainder.begin(), rs_remainder.end());
}


const std::vector<uint8_t> & QrEncode::get_encoded_data() const {
  // return the encoded data
  return data_buffer;
}

void QrEncode::display_encoded_data_as_hex() const {
  for(size_t i = 0; i < data_buffer.size(); ++i) {
    printf("%02X ", data_buffer[i]);
  }
  printf("\nPrinted %zu bytes of encoded data\n", data_buffer.size());
}


// multiply two numbers in GF(2^8) using the irreducible polynomial x^8 + x^4 + x^3 + x^2 + 1
static uint8_t gf_multiply(uint8_t a, uint8_t b)
{
  uint8_t result = 0;

  while (b != 0) {
    if ((b & 1) != 0) {
      result ^= a;
    }

    const bool carry = (a & 0x80) != 0;
    a = static_cast<uint8_t>(a << 1);

    if (carry) {
      a ^= 0x1D; // low eight bits of 0x11D after x^8 is discarded
    }

    b >>= 1;
  }
  return result;
}

// calculate the Reed-Solomon error correction codewords for the given data codewords
static std::vector<uint8_t> rs_calculate(
    const std::vector<uint8_t>& data_codewords)
{
  constexpr std::size_t ec_codeword_count = 26;

  std::vector<uint8_t> generator{1};

  for (std::size_t root = 0; root < ec_codeword_count; ++root) {
    std::vector<uint8_t> next(generator.size() + 1, 0);
    uint8_t root_power = 1;

    for (std::size_t exponent = 0; exponent < root; ++exponent) {
      root_power = gf_multiply(root_power, 0x02);
    }

    for (std::size_t index = 0; index < generator.size(); ++index) {
      next[index] ^= generator[index];
      next[index + 1] ^= gf_multiply(generator[index], root_power);
    }

    generator = std::move(next);
  }

  std::vector<uint8_t> remainder(ec_codeword_count, 0);

  for (uint8_t data_byte : data_codewords) {
    const uint8_t factor = data_byte ^ remainder.front();

    for (std::size_t index = 0; index + 1 < remainder.size(); ++index) {
      remainder[index] =
          remainder[index + 1] ^ gf_multiply(generator[index + 1], factor);
    }

    remainder.back() =
        gf_multiply(generator.back(), factor);
  }

  return remainder;
}

} // namespace qr_code_gen