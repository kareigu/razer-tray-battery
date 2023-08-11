#include "usb_device.h"
#include <codecvt>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include <locale>


namespace rzr_ic {
USBDevice USBDevice::from_device_info(const hid_device_info* device_info) {
  // NOLINTNEXTLINE(*deprecated*): No better solution for this atm
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
  return USBDevice{
          .vendor_id = device_info->vendor_id,
          .product_id = device_info->vendor_id,
          .version_number = device_info->release_number,
          .manufacturer_string = converter.to_bytes(fmt::format(L"", device_info->manufacturer_string)),
          .product_string = converter.to_bytes(fmt::format(L"{}", device_info->product_string)),
          .path = device_info->path,
          .serial_number = converter.to_bytes(fmt::format(L"{}", device_info->serial_number)),
  };
}
}// namespace rzr_ic
