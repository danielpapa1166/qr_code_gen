#include "qr_code_gen/qr_code_drawer.hpp"

#include <png.h>

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace qr_code_gen {

namespace {

constexpr std::size_t module_size = 10;
constexpr std::size_t quiet_zone_size = 4;

void set_pixel(std::vector<png_byte>& pixels, std::size_t image_size,
               std::size_t row, std::size_t column, int color)
{
  const std::size_t pixel_index = (row * image_size + column) * 3;
  pixels[pixel_index] = static_cast<png_byte>((color >> 16) & 0xFF);
  pixels[pixel_index + 1] = static_cast<png_byte>((color >> 8) & 0xFF);
  pixels[pixel_index + 2] = static_cast<png_byte>(color & 0xFF);
}

} // namespace

QrDrawer::QrDrawer(const QrDrawerConfig_t & config)
    : config(config)
{
}

void QrDrawer::draw(
  const std::vector<std::vector<QrModuleType_t>>& matrix) const
{
  if (config.output_file_path.empty()) {
    throw std::invalid_argument("PNG output path must not be empty");
  }

  if (matrix.empty() || matrix.front().empty()) {
    throw std::invalid_argument("QR matrix must not be empty");
  }

  const std::size_t matrix_size = matrix.size();
  for (const auto& row : matrix) {
    if (row.size() != matrix_size) {
      throw std::invalid_argument("QR matrix must be square");
    }

    for (QrModuleType_t module : row) {
      if (module != QRMODULE_UNSET && module != QRMODULE_WHITE &&
          module != QRMODULE_BLACK) {
        throw std::invalid_argument("QR matrix contains an invalid module");
      }
    }
  }

  const std::size_t image_size =
      (matrix_size + 2 * quiet_zone_size) * module_size;
  std::vector<png_byte> pixels(image_size * image_size * 3);

  for (std::size_t row = 0; row < image_size; ++row) {
    for (std::size_t column = 0; column < image_size; ++column) {
      set_pixel(pixels, image_size, row, column, config.background_color);
    }
  }

  for (std::size_t row = 0; row < matrix_size; ++row) {
    for (std::size_t column = 0; column < matrix_size; ++column) {
      if (matrix[row][column] == QRMODULE_WHITE) {
        continue;
      }

      const std::size_t first_row = (row + quiet_zone_size) * module_size;
      const std::size_t first_column =
          (column + quiet_zone_size) * module_size;

      for (std::size_t pixel_row = first_row;
           pixel_row < first_row + module_size; ++pixel_row) {
        for (std::size_t pixel_column = first_column;
             pixel_column < first_column + module_size; ++pixel_column) {
          set_pixel(pixels, image_size, pixel_row, pixel_column,
                    config.foreground_color);
        }
      }
    }
  }

  png_image image{};
  image.version = PNG_IMAGE_VERSION;
  image.width = static_cast<png_uint_32>(image_size);
  image.height = static_cast<png_uint_32>(image_size);
  image.format = PNG_FORMAT_RGB;

  if (png_image_write_to_file(&image, config.output_file_path.c_str(), 0,
                              pixels.data(), 0, nullptr) == 0) {
    throw std::runtime_error("Unable to write PNG: " + config.output_file_path);
  }
}


} // namespace qr_code_gen