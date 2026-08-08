#ifndef QR_CODE_GEN_QR_CODE_COMMON_DEFS_HPP
#define QR_CODE_GEN_QR_CODE_COMMON_DEFS_HPP

typedef enum QrModuleReserved {
  QRMODULE_FREE = 0,
  QRMODULE_RESERVED = 1
} QrModuleReserved_t;

typedef enum QrModuleColor {
  QRMODULE_UNSET = 0,
  QRMODULE_WHITE = 1,
  QRMODULE_BLACK = 2,
} QrModuleColor_t;

typedef struct QrModuleType {
  QrModuleColor_t color;
  QrModuleReserved_t reserved;
} QrModuleType_t;

#endif // QR_CODE_GEN_QR_CODE_COMMON_DEFS_HPP