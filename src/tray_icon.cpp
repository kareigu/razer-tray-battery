#include "tray_icon.h"
#include <QDebug>
#include <QIcon>
#include <QtLogging>


namespace rzr_ic {
TrayIcon::TrayIcon(QObject* parent) : QSystemTrayIcon(parent) {
  QIcon tray_icon_image(":/icon.png");
  m_icons.append(std::move(tray_icon_image));
  setToolTip("Razer Battery");
  setIcon(m_icons[0]);
  connect(this, &QSystemTrayIcon::activated, this, &TrayIcon::process_activation);

  show();
}

void TrayIcon::process_activation(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::Unknown:
      qCritical() << "Unknown activation on tray icon";
      break;
    case QSystemTrayIcon::Context:
      qInfo() << "Open context menu";
      break;
    case QSystemTrayIcon::DoubleClick:
      qInfo() << "Double clicked";
      break;
    default:
      qWarning() << "Unimplemented";
      break;
  }
}
}// namespace rzr_ic
