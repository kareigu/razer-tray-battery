#include "window.h"
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

    constexpr auto VIPER_PID = 0x007B;

    auto hids = hid_enumerate(RAZER_VENDOR_ID, 0);

    for (hid_device_info* device_info = hids; device_info; device_info = device_info->next) {
      if (!device_info) {
        qWarning() << "nullptr found";
        continue;
      }

      // if (device_info->product_id != VIPER_PID)
      //   continue;

      auto device = hid_open_path(device_info->path);

      if (!device) {
        qWarning() << fmt::format(L"No device handle: {}", hid_error(nullptr));
        continue;
      }

      auto usb_device = USBDevice::from_device_info(device_info);
      qInfo() << fmt::format("{}", usb_device).c_str();

      // qInfo() << "Manufacturer string: " << fmt::format(L"{}", device_info->manufacturer_string) << "\n"
      //         << "Vendor ID:" << device_info->vendor_id << "\n"
      //         << "Product ID: " << device_info->product_id << "\n"
      //         << "Product string: " << fmt::format(L"{}", device_info->product_string) << "\n"
      //         << "Path: " << device_info->path << "\n\n";

      std::vector<uint8_t> data = {0x00, 0x1f, 0x00, 0x00, 0x00, 0x02, 0x07, 0x80};
      uint8_t crc = 0;
      for (uint8_t i = 2; i < data.size(); i++)
        crc = crc ^ data[i];

      for (uint8_t i = 0; i < 80; i++)
        data.push_back(0x00);

      data.push_back(crc);
      data.push_back(0x00);

      auto res = hid_write(device, data.data(), data.size());

      if (res == -1) {
        qWarning() << fmt::format(L"Error writing to HID device: {}", hid_error(device));
        hid_close(device);
        continue;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(500));

      QList<uint8_t> buf = {0x00};
      buf.resize(100);
      auto len = hid_read(device, buf.data(), 100);

      if (len == -1) {
        qWarning() << fmt::format(L"Error reading from HID device", hid_error(device));
        hid_close(device);
        continue;
      }
      buf.resize(len);
      qInfo() << buf;


      hid_close(device);
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
