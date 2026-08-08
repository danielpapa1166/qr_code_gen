#include "qr_code_gen/qr_code_encode.hpp"
#include <stdint.h>
#include <array>

// this file contains the functionalities to 
// encode a string to QR code data

static const uint8_t ENC = 0b0100; 
static const uint8_t END = 0b0000;
static const uint8_t PADDING_BYTES[] = {
  0b11101100, 
  0b00010001
};


namespace qr_code_gen {

static uint8_t gf_multiply(uint8_t a, uint8_t b); 
static std::vector<uint8_t> rs_calculate(const std::vector<uint8_t> & data_codewords);


QrEncode::QrEncode(const std::string & data) {
  // encode the data to QR code data

  if(data.length() > MAX_DATA_LENGTH) {
    return; 
  }

  std::vector<uint8_t> tmp_data_buffer;

  tmp_data_buffer.resize(data.length() + 3);
  tmp_data_buffer[0] = ENC;
  tmp_data_buffer[1] = static_cast<uint8_t>(data.length());

  for(size_t i = 0; i < data.length(); ++i) {
    tmp_data_buffer[i + 2] = static_cast<uint8_t>(data[i]);
  }

  tmp_data_buffer[data.length() + 2] = END;

  data_buffer.resize((tmp_data_buffer.size() + 1));

  // rearrange: shift by one nibble: 
  for(size_t i = 0; i < tmp_data_buffer.size(); i++) {
    data_buffer[i] = (tmp_data_buffer[i] << 4) | (tmp_data_buffer[i + 1] >> 4);
  }

  // cut trailing zeros
  while(!data_buffer.empty() && data_buffer.back() == 0) {
    data_buffer.pop_back();
  }

  // add padding bytes to reach the required length of 44 data codewords
  const int paddig_byte_offset = data_buffer.size() % 2;
  while(data_buffer.size() < 44) {
    data_buffer.push_back(PADDING_BYTES[
      (paddig_byte_offset + data_buffer.size()) % 2
    ]);
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