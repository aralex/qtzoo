#include "testchbfiltmainwindow.h"
#include "ui_testchbfiltmainwindow.h"

TestChBFiltMainWindow::TestChBFiltMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TestChBFiltMainWindow)
{
  ui->setupUi(this);
}

TestChBFiltMainWindow::~TestChBFiltMainWindow()
{
  delete ui;
}
