#include "testchbfiltmainwindow.h"
#include "ui_testchbfiltmainwindow.h"

#include <QFileDialog>
#include <QDebug>


#define COL_VISIBLE_1   1
#define COL_ID_1        0

#define COL_VISIBLE_2   1
#define COL_ID_2        0
#define COL_REF_2       2


static const QRgb c_Highlited_List_Item_Bg(QRgb(qRgb(255, 249, 166)));


TestChBFiltMainWindow::TestChBFiltMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TestChBFiltMainWindow)
{
  ui->setupUi(this);

  mdl_1_orig = new QStandardItemModel(this);
  mdl_2_orig = new QStandardItemModel(this);

  ui->tableView_1->setModel(mdl_1_orig);
  LoadCsvData(mdl_1_orig, ":/data/regions.csv", ";");
  ui->tableView_1->resizeColumnsToContents();
  ui->tableView_1->resizeRowsToContents();
  ui->tableView_1->horizontalHeader()->setStretchLastSection(true);

  ui->tableView_2->setModel(mdl_2_orig);
  LoadCsvData(mdl_2_orig, ":/data/towns.csv", ";");
  ui->tableView_2->resizeColumnsToContents();
  ui->tableView_2->resizeRowsToContents();
  ui->tableView_2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

  //QString file_name = QFileDialog::getOpenFileName(this, "Data file", ".");
  //if(file_name.isEmpty()) return;

  //LoadCsvData(mdl_orig, file_name, ";");

  mdl_1 = new QCheckboxFilterModel(mdl_1_orig, COL_VISIBLE_1, COL_ID_1);
  mdl_1->setObjectName("mdl_1");
  mdl_1->setup();
  ui->lv_1->setModel(mdl_1);
  ui->lv_1->setModelColumn(COL_VISIBLE_1);

  mdl_2 = new QCheckboxFilterModel(mdl_2_orig, COL_VISIBLE_2, COL_ID_2);
  mdl_2->setObjectName("mdl_2");
  mdl_1->setup();
  ui->lv_2->setModel(mdl_2);
  ui->lv_2->setModelColumn(COL_VISIBLE_2);

  connect(mdl_1, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(on_mdl_1_dataChanged(QModelIndex,QModelIndex,QVector<int>)));

  connect(mdl_2, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(on_mdl_2_dataChanged(QModelIndex,QModelIndex,QVector<int>)));

  connect(ui->lv_1->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_lv_1_selection_changed(QItemSelection,QItemSelection)));
}


TestChBFiltMainWindow::~TestChBFiltMainWindow()
{
  delete ui;
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
  int bottom = bottomRight.row();

  for(int i = top; i <= bottom; ++i)
  {
    if((COL_VISIBLE_1 >= topLeft.column()) && (COL_VISIBLE_1 <= bottomRight.column()))
    {
      const QModelIndex ind = topLeft.model()->index(i, COL_VISIBLE_1);
      QString id_str = ind.data(Qt::UserRole).toString();

      int checked = ind.data(Qt::CheckStateRole).toBool()? Qt::Checked: Qt::Unchecked;

      mdl_2->toggleVisibilityReferenced(checked, id_str, COL_REF_2);
      mdl_2->invalidate();
    }
  }
}


void TestChBFiltMainWindow::on_mdl_2_dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
  qDebug() << __FUNCTION__;
}


void TestChBFiltMainWindow::on_le_regionPattern_textChanged(const QString &arg1)
{

}


void TestChBFiltMainWindow::on_le_townPattern_textChanged(const QString &arg1)
{

}


void TestChBFiltMainWindow::on_lv_1_selection_changed(const QItemSelection& selected, const QItemSelection& deselected)
{
  qDebug() << __FUNCTION__;

  foreach (const QModelIndex& ind, deselected.indexes())
  {
    if(ind.column() != COL_VISIBLE_1) continue;

    QString id_str = ind.data(Qt::UserRole).toString();
    mdl_2->brush(QBrush(), id_str, COL_REF_2);
  }

  foreach(const QModelIndex& ind, selected.indexes())
  {
    if(ind.column() != COL_VISIBLE_1) continue;

    QString id_str = ind.data(Qt::UserRole).toString();
    mdl_2->brush(QBrush(c_Highlited_List_Item_Bg), id_str, COL_REF_2);
  }
}
