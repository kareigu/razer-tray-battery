#include "razer_report.h"
#include "common.h"
#include <QDebug>
#include <fmt/core.h>

constexpr u8 CMD_BUSY = 0x01;
constexpr u8 CMD_FAILURE = 0x03;
constexpr u8 CMD_NOT_SUPPORTED = 0x05;
constexpr u8 CMD_TIMEOUT = 0x04;

namespace rzr_ic {
CheckStatusResult RazerReport::check_status() const {
  switch (this->status) {
    case CMD_BUSY:
      return {.success = false, .message = "Device busy"};
    case CMD_NOT_SUPPORTED:
      return {.success = false, .message = "Command not supported"};
    case CMD_FAILURE:
      return {.success = false, .message = "Command failure"};
    case CMD_TIMEOUT:
      return {.success = false, .message = "Timed out"};
    default:
      return {.success = true, .message = "Command success"};
  }
}
}// namespace rzr_ic
