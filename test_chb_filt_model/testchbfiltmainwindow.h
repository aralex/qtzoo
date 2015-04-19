#ifndef TESTCHBFILTMAINWINDOW_H
#define TESTCHBFILTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class TestChBFiltMainWindow;
}

class TestChBFiltMainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit TestChBFiltMainWindow(QWidget *parent = 0);
    ~TestChBFiltMainWindow();

  private:
    Ui::TestChBFiltMainWindow *ui;
};

#endif // TESTCHBFILTMAINWINDOW_H
