#include "window.h"
#include "common.h"
#include "razer_report.h"
#include "usb_device.h"
#include <QDebug>
#include <QIcon>
#include <QVBoxLayout>
#include <chrono>
#include <cmath>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include <hidapi.h>


namespace rzr_ic {
Window::Window() {
  this->resize(800, 600);
  auto main_widget = new QWidget(this);
  auto layout = new QVBoxLayout();
  main_widget->setLayout(layout);
  setCentralWidget(main_widget);

  auto err = hid_init();
  qInfo() << "hidapi initialised with code: " << err;

  m_close_button = new QPushButton(main_widget);
  m_close_button->setFixedSize(200, 50);
  m_close_button->setText("Close");
  layout->addWidget(m_close_button, 0, Qt::Alignment::enum_type::AlignCenter);
  connect(m_close_button, &QPushButton::pressed, this, &Window::close_window);

  m_test_button = new QPushButton(main_widget);
  m_test_button->setFixedSize(200, 50);
  m_test_button->setText("Test");
  layout->addWidget(m_test_button, 0, Qt::Alignment::enum_type::AlignCenter);
  connect(m_test_button, &QPushButton::pressed, this, [err]() {
    qDebug() << "test";
    if (err)
      return;

    constexpr auto RAZER_VENDOR_ID = 0x1532;
    constexpr auto VIPER_WIRELESS_PID = 0x007B;

    auto hids = hid_enumerate(RAZER_VENDOR_ID, VIPER_WIRELESS_PID);

    for (hid_device_info* device_info = hids; device_info; device_info = device_info->next) {
      if (!device_info) {
        qWarning() << "nullptr found";
        continue;
      }

      // if (device_info->product_id != VIPER_PID)
      //   continue;

      qInfo() << fmt::format("{:_^20}", "_").c_str();

      auto device = hid_open_path(device_info->path);

      if (!device) {
        qWarning() << fmt::format(L"No device handle: {}", hid_error(nullptr));
        continue;
      }


      // qInfo() << "Manufacturer string: " << fmt::format(L"{}", device_info->manufacturer_string) << "\n"
      //         << "Vendor ID:" << device_info->vendor_id << "\n"
      //         << "Product ID: " << device_info->product_id << "\n"
      //         << "Product string: " << fmt::format(L"{}", device_info->product_string) << "\n"
      //         << "Path: " << device_info->path << "\n\n";

      /* std::vector<uint8_t> data = {0x00, 0x1f, 0x00, 0x00, 0x00, 0x02, 0x07, 0x80}; */
      /* uint8_t crc = 0; */
      /* for (uint8_t i = 2; i < data.size(); i++) */
      /*   crc = crc ^ data[i]; */
      /**/
      /* for (uint8_t i = 0; i < 80; i++) */
      /*   data.push_back(0x00); */
      /**/
      /* data.push_back(crc); */
      /* data.push_back(0x00); */


      constexpr u8 COMMAND_CLASS = 0x07;
      constexpr u8 COMMAND_ID = 0x80;
      constexpr u8 DATA_SIZE = 0x02;

      RazerReport request{0};
      memset(&request, 0, sizeof(RazerReport));
      request.id = 0x00;
      request.status = 0x00;
      request.transaction_id.id = 0x00;
      request.remaining_packets = 0x00;
      request.protocol_type = 0x00;
      request.command_class = COMMAND_CLASS;
      request.command_id.id = COMMAND_ID;
      request.data_size = DATA_SIZE;

      request.transaction_id.id = 0xff;

      {
        u8 crc = 0;
        u8* _request = reinterpret_cast<u8*>(&request);
        for (size_t i = 3; i < 88; i++)
          crc ^= _request[i];

        request.crc = crc;
      }


      auto res = hid_send_feature_report(device, reinterpret_cast<const u8*>(&request), sizeof(RazerReport));

      if (res == -1) {
        qWarning() << fmt::format(L"Error writing to HID device: {}", hid_error(device));
        hid_close(device);
        continue;
      }
      qDebug() << fmt::format("Sent request:\n{}", request).c_str();

      auto usb_device = USBDevice::from_device_info(device_info);
      qInfo() << fmt::format("{}", usb_device).c_str();

      std::this_thread::sleep_for(std::chrono::milliseconds(600));

      RazerReport response{};
      memset(&response, 0, sizeof(RazerReport));
      response.id = 0x00;

      auto len = hid_get_feature_report(device, reinterpret_cast<u8*>(&response), sizeof(RazerReport));

      if (len == -1) {
        qWarning() << fmt::format(L"Error reading from HID device: {}", hid_error(device));
        hid_close(device);
        continue;
      }

      qInfo() << "Response length = " << len;

      qDebug() << fmt::format("Response received:\n{}", response).c_str();

      if (response.remaining_packets != request.remaining_packets || response.command_class != request.command_class || response.command_id.id != request.command_id.id) {
        qDebug() << "Reports not matching";
      }

      hid_close(device);
      qInfo() << fmt::format("{:_^20}", "_").c_str();
    }
    hid_free_enumeration(hids);
  });

  m_tray_icon = new TrayIcon(this);
}

void Window::close_window() {
  qInfo() << "Closing window";
  this->close();
}
}// namespace rzr_ic
