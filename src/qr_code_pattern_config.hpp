#ifndef QR_CODE_PATTERN_CONFIG_HPP
#define QR_CODE_PATTERN_CONFIG_HPP

#include "qr_code_gen/qr_code_common_defs.hpp"
#include <vector>

namespace qr_code_gen {

using Pattern = std::vector<std::vector<QrModuleType_t>>;

constexpr QrModuleType_t black{
  QRMODULE_BLACK,
  QRMODULE_RESERVED
};

constexpr QrModuleType_t white{
  QRMODULE_WHITE,
  QRMODULE_RESERVED
};

const Pattern finder_pattern{
  {white, white, white, white, white, white, white, white, white},
  {white, black, black, black, black, black, black, black, white},
  {white, black, white, white, white, white, white, black, white},
  {white, black, white, black, black, black, white, black, white},
  {white, black, white, black, black, black, white, black, white},
  {white, black, white, black, black, black, white, black, white},
  {white, black, white, white, white, white, white, black, white},
  {white, black, black, black, black, black, black, black, white},
  {white, white, white, white, white, white, white, white, white},
};

const Pattern alignment_pattern{
  {black, black, black, black, black},
  {black, white, white, white, black},
  {black, white, black, white, black},
  {black, white, white, white, black},
  {black, black, black, black, black},
};

const Pattern timing_pattern_vertical{
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}, 
  {white}, 
  {black}
};

const Pattern timing_pattern_horizontal{
    {
        black, 
        white, 
        black, 
        white, 
        black, 
        white, 
        black, 
        white, 
        black, 
        white, 
        black, 
        white, 
        black
    }
};

}


#endif // QR_CODE_PATTERN_CONFIG_HPP