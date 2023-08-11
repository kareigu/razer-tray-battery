#include <QApplication>
#include <QMainWindow>
#include <QStyleFactory>

#include "window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setStyle(QStyleFactory::create("Fusion"));
  app.setApplicationName("razer-tray-battery");
  app.setOrganizationName("razer-tray-battery");

  rzr_ic::Window window;
  window.show();
  return app.exec();
}
