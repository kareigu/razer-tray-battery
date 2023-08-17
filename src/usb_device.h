#include <fmt/format.h>
#include <hidapi.h>
#include <string>


namespace rzr_ic {
struct USBDevice {
  uint16_t vendor_id;
  uint16_t product_id;
  uint16_t version_number;
  std::string manufacturer_string;
  std::string product_string;
  std::string path;
  std::string serial_number;

  static USBDevice from_device_info(const hid_device_info* device_info);
};

}// namespace rzr_ic

template<>
struct fmt::formatter<rzr_ic::USBDevice> {
  template<typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto format(const rzr_ic::USBDevice& value, FormatContext& ctx) {
    fmt::format_to(ctx.out(), "\nVendor ID: {:#x}\n", value.vendor_id);
    fmt::format_to(ctx.out(), "Product ID: {:#x}\n", value.product_id);
    fmt::format_to(ctx.out(), "Version Number: {:#x}\n", value.version_number);
    fmt::format_to(ctx.out(), "Manufacturer String: {:s}\n", value.manufacturer_string);
    fmt::format_to(ctx.out(), "Product String: {:s}\n", value.product_string);
    fmt::format_to(ctx.out(), "Path: {:s}\n", value.path);
    return fmt::format_to(ctx.out(), "Serial Number: {:s}\n", value.serial_number);
  }
};
