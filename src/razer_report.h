#pragma once
#include "common.h"
#include <fmt/core.h>

namespace rzr_ic {

union transaction_id_t {
  u8 id;
  struct transaction_parts {
    u8 device : 3;
    u8 id : 5;
  };
};

union command_id_t {
  u8 id;
  struct transaction_parts {
    u8 direction : 1;
    u8 id : 7;
  };
};

struct CheckStatusResult {
  bool success;
  const char* message;
};

struct RazerReport {
  u8 id;
  u8 status;
  transaction_id_t transaction_id;
  u16 remaining_packets;
  u8 protocol_type;
  u8 data_size;
  u8 command_class;
  command_id_t command_id;
  u8 arguments[80];
  u8 crc;
  u8 reserved;

  CheckStatusResult check_status() const;
};

}// namespace rzr_ic

template<>
struct fmt::formatter<rzr_ic::RazerReport> {
  template<typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto format(const rzr_ic::RazerReport& report, FormatContext& ctx) {
    fmt::format_to(ctx.out(), "\nID: {:#x}\n", report.id);
    auto status = report.check_status();
    fmt::format_to(ctx.out(), "Status: {:s} - {:s}\n", status.success ? "SUCCESS" : "FAILED", status.message);
    fmt::format_to(ctx.out(), "Transaction ID: {:#x}\n", report.transaction_id.id);
    fmt::format_to(ctx.out(), "Remaining packets: {:d}\n", report.remaining_packets);
    fmt::format_to(ctx.out(), "Protocol type: {:#x}\n", report.protocol_type);
    fmt::format_to(ctx.out(), "Data size: {:d}\n", report.data_size);
    fmt::format_to(ctx.out(), "Data: [");
    const u8 data_size = report.data_size;
    for (u8 i = 0; i < data_size; i++) {
      if (i == data_size - 1)
        fmt::format_to(ctx.out(), "{:d}", report.arguments[i]);
      else
        fmt::format_to(ctx.out(), "{:d}, ", report.arguments[i]);
    }
    fmt::format_to(ctx.out(), "]\n");
    fmt::format_to(ctx.out(), "Command class: {:#x}\n", report.command_class);
    fmt::format_to(ctx.out(), "Command ID: {:#x}\n", report.command_id.id);
    fmt::format_to(ctx.out(), "CRC: {:#x}\n", report.crc);
    return fmt::format_to(ctx.out(), "Reserved: {:#x}\n", report.reserved);
  }
};
