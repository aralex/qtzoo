#include "testchbfiltmainwindow.h"
#include "ui_testchbfiltmainwindow.h"

#include <QFileDialog>
#include <QDebug>


#define COL_DIETS_ID          0
#define COL_DIETS_NAME        1
#define COL_DIETS_PROD_KINDS  2
#define COL_DIETS_PRODUCTS    3
#define COL_DIETS_VISIBLE     COL_DIETS_NAME


#define COL_PROD_KINDS_ID         0
#define COL_PROD_KINDS_NAME       1
#define COL_PROD_KINDS_VISIBLE    COL_PROD_KINDS_NAME


#define COL_PRODUCTS_ID        0
#define COL_PRODUCTS_NAME      1
#define COL_PRODUCTS_REF       2

#define COL_PRODUCTS_VISIBLE   COL_PRODUCTS_NAME


static const QRgb c_Highlited_List_Item_Bg(QRgb(qRgb(255, 249, 166)));


TestChBFiltMainWindow::TestChBFiltMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TestChBFiltMainWindow),
  buttonCancelClicked(false)
{
  ui->setupUi(this);

  mdlDietsFull = new QStandardItemModel(this);
  mdlProdKindsFull = new QStandardItemModel(this);
  mdlProductsFull = new QStandardItemModel(this);

  LoadCsvData(mdlDietsFull, ":/data/diets.csv", ";");
  LoadCsvData(mdlProdKindsFull, ":/data/prod-kinds.csv", ";");
  LoadCsvData(mdlProductsFull, ":/data/products.csv", ";");

  ui->tvDietsFull->setModel(mdlDietsFull);
  ui->tvDietsFull->resizeColumnsToContents();
  ui->tvDietsFull->resizeRowsToContents();
  ui->tvDietsFull->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

  mdlDiets = new QCheckboxFilterModel(mdlDietsFull, COL_DIETS_VISIBLE, COL_DIETS_ID);
  mdlDiets->setObjectName("mdlDiets");
  mdlDiets->toggleAllItems(false, true, false);
  ui->lvDiets->setModel(mdlDiets);
  ui->lvDiets->setModelColumn(COL_DIETS_VISIBLE);

  mdlProdKinds = new QCheckboxFilterModel(mdlProdKindsFull, COL_PROD_KINDS_VISIBLE, COL_PROD_KINDS_ID);
  mdlProdKinds->setObjectName("mdlProdKinds");
  mdlProdKinds->toggleAllItems(false, true, false);
  ui->lvProdKinds->setModel(mdlProdKinds);
  ui->lvProdKinds->setModelColumn(COL_PROD_KINDS_VISIBLE);

  mdlProducts = new QCheckboxFilterModel(mdlProductsFull, COL_PRODUCTS_VISIBLE, COL_PRODUCTS_ID);
  mdlProducts->setObjectName("mdlProducts");
  mdlProducts->toggleAllItems(false, true, false);
  ui->lvProducts->setModel(mdlProducts);
  ui->lvProducts->setModelColumn(COL_PRODUCTS_VISIBLE);


  ui->btnCancel->setEnabled(false);


  connect(ui->lvDiets->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_mdlDiets_selection_changed(QItemSelection,QItemSelection)));

  connect(ui->lvProdKinds->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_lvProdKinds_selection_changed(QItemSelection,QItemSelection)));

  if(connect(mdlProdKinds, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(on_mdlProdKinds_dataChanged(QModelIndex,QModelIndex,QVector<int>))))
    qDebug() << "connected!";
  else qDebug() << "not connected!";

//  connect(mdlProducts, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
//          this, SLOT(on_mdlProducts_dataChanged(QModelIndex,QModelIndex,QVector<int>)));

  //mdlSpy1 = new QSpyModel(mdlProdKinds);
  //ui->tvSpy1->setModel(mdlSpy1);

  //mdlSpy2 = new QSpyModel(mdlProducts->getSrcModel());
  //ui->tvSpy2->setModel(mdlSpy2);
}


TestChBFiltMainWindow::~TestChBFiltMainWindow()
{
  delete ui;
}


void TestChBFiltMainWindow::on_chbProdKinds_toggled(bool checked)
{
  qDebug() << __FUNCTION__;
  // !!!
  //mdlProdKinds->setShowCheckboxes(checked);
  //mdlProducts->hideAllItems();
}


void TestChBFiltMainWindow::on_chbProducts_toggled(bool checked)
{
  qDebug() << __FUNCTION__;
  // !!!
  //mdlProducts->setShowCheckboxes(checked);
}

void TestChBFiltMainWindow::on_lePatternDiets_textChanged(const QString &arg1)
{
  qDebug() << __FUNCTION__;
}

void TestChBFiltMainWindow::on_lePatternProdKinds_textChanged(const QString &arg1)
{
  qDebug() << __FUNCTION__;
}


void TestChBFiltMainWindow::on_mdlDiets_selection_changed(const QItemSelection &selected, const QItemSelection &deselected)
{
  qDebug() << __FUNCTION__;

  ui->lvProdKinds->selectionModel()->clear();

  if(selected.count())
  {
    mdlProducts->toggleAllItems(false, false, false);
    mdlProdKinds->toggleAllItems(false, false, false);

    foreach(const QModelIndex& ind, selected.indexes())
    {
      SelectedProducts = mdlDietsFull->index(ind.row(), COL_DIETS_PRODUCTS).data(Qt::DisplayRole).toString();
      qDebug() << "toggleVisibility mdlProducts" << SelectedProducts;
      mdlProducts->toggleItems(true, SelectedProducts, false);

      SelectedProdKinds = mdlDietsFull->index(ind.row(), COL_DIETS_PROD_KINDS).data(Qt::DisplayRole).toString();
      qDebug() << "toggleVisibility mdlProdKinds" << SelectedProdKinds;
      mdlProdKinds->toggleItems(true, SelectedProdKinds, false);
    }
  }
  else
  {
    mdlProdKinds->toggleAllItems(false, true, false);
    mdlProducts->toggleAllItems(false, true, false);
  }

  mdlProdKinds->invalidate();
  mdlProducts->invalidate();
}


void TestChBFiltMainWindow::on_lvDiets_doubleClicked(const QModelIndex &index)
{
  mdlProdKinds->setShowCheckboxes(true);
  mdlProducts->setShowCheckboxes(true);
  ui->btnCancel->setEnabled(true);
  ui->lvDiets->setEnabled(false);
}


void TestChBFiltMainWindow::on_mdlProdKinds_dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
  qDebug() << __FUNCTION__ << topLeft.column() << bottomRight.column();

  if(buttonCancelClicked) return;

  int top = topLeft.row();
  int bottom = bottomRight.row();
  //int sc = mdlProdKinds->serviceColumn();

  for(int i = top; i <= bottom; ++i)
  {
    if((COL_PROD_KINDS_VISIBLE >= topLeft.column()) && (COL_PROD_KINDS_VISIBLE <= bottomRight.column()))
    {
      const QModelIndex ind = topLeft.model()->index(i, COL_PROD_KINDS_VISIBLE);
      QString id_str = ind.data(Qt::UserRole).toString();

      int checked = mdlProdKinds->isItemChecked(i);

      qDebug() << "toggle(" << checked << ") ids" << id_str;

      mdlProducts->toggleItemsReferenced(checked, id_str, checked, COL_PRODUCTS_REF);
      mdlProducts->invalidate();
    }
  }
}


void TestChBFiltMainWindow::on_lvProdKinds_selection_changed(const QItemSelection& selected, const QItemSelection& deselected)
{
  qDebug() << __FUNCTION__;

  foreach (const QModelIndex& ind, deselected.indexes())
  {
    if(ind.column() != COL_PROD_KINDS_VISIBLE) continue;

    QString id_str = ind.data(Qt::UserRole).toString();
    mdlProducts->brush(QBrush(), id_str, COL_PRODUCTS_REF);
  }

  foreach(const QModelIndex& ind, selected.indexes())
  {
    if(ind.column() != COL_PROD_KINDS_VISIBLE) continue;

    QString id_str = ind.data(Qt::UserRole).toString();
    mdlProducts->brush(QBrush(c_Highlited_List_Item_Bg), id_str, COL_PRODUCTS_REF);
  }
}


void TestChBFiltMainWindow::on_lePatternProducts_textChanged(const QString &arg1)
{
  qDebug() << __FUNCTION__;
}



void TestChBFiltMainWindow::on_btnCancel_clicked()
{
  buttonCancelClicked = true;

  mdlProdKinds->toggleAllItems(false, false, false);
  mdlProducts->toggleAllItems(false, false, false);

  mdlProducts->toggleItems(true, SelectedProducts, false);
  mdlProdKinds->toggleItems(true, SelectedProdKinds, false);

  ui->btnCancel->setEnabled(false);
  buttonCancelClicked = false;
  ui->lvDiets->setEnabled(true);
}
