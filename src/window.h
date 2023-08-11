#pragma once
#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QSystemTrayIcon>

#include "tray_icon.h"

namespace rzr_ic {
class Window : public QMainWindow {
public:
  Window();
  ~Window() = default;

public slots:
  void close_window();

private:
  QPointer<QPushButton> m_test_button = nullptr;
  QPointer<QPushButton> m_close_button = nullptr;
  QPointer<TrayIcon> m_tray_icon = nullptr;
};
}// namespace rzr_ic
