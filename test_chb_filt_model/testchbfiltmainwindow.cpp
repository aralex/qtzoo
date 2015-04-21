#include "testchbfiltmainwindow.h"
#include "ui_testchbfiltmainwindow.h"

#include <QFileDialog>
#include <QDebug>


#define COL_VISIBLE_1   3
#define COL_ID_1        0

#define COL_VISIBLE_2   2
#define COL_ID_2        0
#define COL_REF_2       4


static const QRgb c_Highlited_List_Item_Bg(QRgb(qRgb(255, 249, 166)));


TestChBFiltMainWindow::TestChBFiltMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TestChBFiltMainWindow)
{
  ui->setupUi(this);

  mdl_orig = new QStandardItemModel(this);
  ui->tableView_1->setModel(mdl_orig);

  ui->chb_1->setEnabled(false);
  ui->chb_2->setEnabled(false);

  ui->lePattern_1->setEnabled(false);
  ui->lePattern_2->setEnabled(false);
}


TestChBFiltMainWindow::~TestChBFiltMainWindow()
{
  delete ui;
}


void TestChBFiltMainWindow::on_btnLoad_clicked()
{
  qDebug() << __FUNCTION__;
  QString file_name = QFileDialog::getOpenFileName(this, "Data file", ".");
  if(file_name.isEmpty()) return;

  LoadCsvData(mdl_orig, file_name, ";");
  ui->tableView_1->resizeColumnsToContents();
  ui->tableView_1->resizeRowsToContents();

  mdl_1 = new QCheckboxFilterModel(mdl_orig, COL_VISIBLE_1, COL_ID_1);
  mdl_1->setObjectName("mdl_1");
  mdl_1->setup();
  ui->lv_1->setModel(mdl_1);
  ui->lv_1->setModelColumn(COL_VISIBLE_1);

  mdl_2 = new QCheckboxFilterModel(mdl_orig, COL_VISIBLE_2, COL_ID_2);
  mdl_2->setObjectName("mdl_2");
  mdl_1->setup();
  ui->lv_2->setModel(mdl_2);
  ui->lv_2->setModelColumn(COL_VISIBLE_2);

  ui->chb_1->setEnabled(true);
  ui->chb_2->setEnabled(true);

  ui->lePattern_1->setEnabled(true);
  ui->lePattern_2->setEnabled(true);

  connect(mdl_1, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(on_mdl_1_dataChanged(QModelIndex,QModelIndex,QVector<int>)));
  connect(mdl_2, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(on_mdl_2_dataChanged(QModelIndex,QModelIndex,QVector<int>)));
}


void TestChBFiltMainWindow::on_chb_1_toggled(bool checked)
{
  qDebug() << __FUNCTION__;
  mdl_1->setShowCheckboxes(checked);
  mdl_2->hideAllItems();
}


void TestChBFiltMainWindow::on_chb_2_toggled(bool checked)
{
  qDebug() << __FUNCTION__;
  mdl_2->setShowCheckboxes(checked);
}


void TestChBFiltMainWindow::on_lePattern_1_textChanged(const QString &arg1)
{
  qDebug() << __FUNCTION__;
}


void TestChBFiltMainWindow::on_lePattern_2_textChanged(const QString &arg1)
{
  qDebug() << __FUNCTION__;
}


void TestChBFiltMainWindow::on_mdl_1_dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
  qDebug() << __FUNCTION__;

  int top = topLeft.row();
  int botton = bottomRight.row();

  for(int i = top; i <= botton; ++i)
  {
    if((COL_VISIBLE_1 >= topLeft.column()) && (COL_VISIBLE_1 <= bottomRight.column()))
    {
      const QModelIndex ind = topLeft.model()->index(i, COL_VISIBLE_1);
      QString id_str = ind.data(Qt::UserRole).toString();

      int checked = ind.data(Qt::CheckStateRole).toBool()? Qt::Checked: Qt::Unchecked;
      QBrush brush;

      if(checked) brush = QBrush(c_Highlited_List_Item_Bg);

      mdl_2->toggleVisibilityReferenced(checked, id_str, COL_REF_2);
      mdl_2->invalidate();
      //mdl_2->Brush(brush, COL_REF_2, id_str);
    }
  }
}


void TestChBFiltMainWindow::on_mdl_2_dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
  qDebug() << __FUNCTION__;
}
