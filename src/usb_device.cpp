#include "usb_device.h"
#include <codecvt>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include <locale>

// NOLINTNEXTLINE(*deprecated*): No better solution for this atm
using Converter = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>;

// NOLINTNEXTLINE(*deprecated*): No better solution for this atm
static inline std::string wchar_to_string_or_na(const wchar_t* wchar) {
  static Converter converter;// NOLINT(*deprecated*): No better solution for this atm
  std::string converted = converter.to_bytes(wchar);
  return converted.length() > 0 ? converted : "n/a";
}

namespace rzr_ic {
USBDevice USBDevice::from_device_info(const hid_device_info* device_info) {
  return USBDevice{
          .vendor_id = device_info->vendor_id,
          .product_id = device_info->product_id,
          .version_number = device_info->release_number,
          .manufacturer_string = wchar_to_string_or_na(device_info->manufacturer_string),
          .product_string = wchar_to_string_or_na(device_info->product_string),
          .path = device_info->path,
          .serial_number = wchar_to_string_or_na(device_info->serial_number),
  };
}
}// namespace rzr_ic
