#include "testchbfiltmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  TestChBFiltMainWindow w;
  w.show();

  return a.exec();
}
