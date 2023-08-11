#pragma once
#include <QIcon>
#include <QList>
#include <QSystemTrayIcon>


namespace rzr_ic {
class TrayIcon : public QSystemTrayIcon {
public:
  TrayIcon(QObject* parent);
  ~TrayIcon() = default;

private slots:
  void process_activation(QSystemTrayIcon::ActivationReason reason);

private:
  QList<QIcon> m_icons;
};
}// namespace rzr_ic
