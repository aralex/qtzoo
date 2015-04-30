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
  ui(new Ui::TestChBFiltMainWindow)
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
  mdlDiets->setup();
  ui->lvDiets->setModel(mdlDiets);
  ui->lvDiets->setModelColumn(COL_DIETS_VISIBLE);

  mdlProdKinds = new QCheckboxFilterModel(mdlProdKindsFull, COL_PROD_KINDS_VISIBLE, COL_PROD_KINDS_ID);
  mdlProdKinds->setObjectName("mdlProdKinds");
  mdlProdKinds->setup();
  ui->lvProdKinds->setModel(mdlProdKinds);
  ui->lvProdKinds->setModelColumn(COL_PROD_KINDS_VISIBLE);

  mdlProducts = new QCheckboxFilterModel(mdlProductsFull, COL_PRODUCTS_VISIBLE, COL_PRODUCTS_ID);
  mdlProducts->setObjectName("mdlProducts");
  mdlProducts->setup();
  ui->lvProducts->setModel(mdlProducts);
  ui->lvProducts->setModelColumn(COL_PRODUCTS_VISIBLE);


  ui->btnCancel->setEnabled(false);


  connect(ui->lvDiets->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_mdlDiets_selection_changed(QItemSelection,QItemSelection)));

  connect(ui->lvProdKinds->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_lvProdKinds_selection_changed(QItemSelection,QItemSelection)));

  connect(mdlProdKinds, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(on_mdlProdKinds_dataChanged(QModelIndex,QModelIndex,QVector<int>)));

//  connect(mdlProducts, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
//          this, SLOT(on_mdlProducts_dataChanged(QModelIndex,QModelIndex,QVector<int>)));

}


TestChBFiltMainWindow::~TestChBFiltMainWindow()
{
  delete ui;
}


void TestChBFiltMainWindow::on_chbProdKinds_toggled(bool checked)
{
  qDebug() << __FUNCTION__;
  mdlProdKinds->setShowCheckboxes(checked);
  mdlProducts->hideAllItems();
}


void TestChBFiltMainWindow::on_chbProducts_toggled(bool checked)
{
  qDebug() << __FUNCTION__;
  mdlProducts->setShowCheckboxes(checked);
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
    mdlProdKinds->hideAllItems();
    mdlProducts->hideAllItems();

    foreach(const QModelIndex& ind, selected.indexes())
    {
      QString ids_prod_kinds = mdlDietsFull->index(ind.row(), COL_DIETS_PROD_KINDS).data(Qt::DisplayRole).toString();
      mdlProdKinds->toggleVisibility(true, ids_prod_kinds);

      QString ids_products = mdlDietsFull->index(ind.row(), COL_DIETS_PRODUCTS).data(Qt::DisplayRole).toString();
      mdlProducts->toggleVisibility(true, ids_products);
    }
  }
  else
  {
    mdlProdKinds->showAllItems();
    mdlProducts->showAllItems();
  }

  mdlProdKinds->invalidate();
  mdlProducts->invalidate();


  /*

  int rows_selected_parent = ui->lvDiets->selectionModel()->selectedRows().count();
  int rows_visible_child = mdlProdKinds->visibleItemsCount();

  if(rows_selected_parent == 0)
  {
    // Если в главной таблице ничего не выделено, показываем все элементы дочерней.
    mdlProdKinds->showAllItems();
  }
  else
  {
    // Если в главной таблице что-то выделено...
    if((rows_visible_child == mdlProdKinds->rowCount()) && (deselected.count() == 0))
    {
      // Если были видны все элементы дочерней таблицы, и в родительской
      // никакое выделение не снято...
      mdlProdKinds->hideAllItems();
      mdlProdKinds->toggleVisibilityReferenced(true, selected, COL_PROD_KINDS_ID);
    }
    else
    {
      mdlProdKinds->toggleVisibilityReferenced(false, deselected, COL_PROD_KINDS_ID);
      mdlProdKinds->toggleVisibilityReferenced(true, selected, COL_PROD_KINDS_ID);
    }
  }

  mdlProdKinds->invalidate();
*/
}


void TestChBFiltMainWindow::on_lvDiets_doubleClicked(const QModelIndex &index)
{
  mdlProdKinds->setShowCheckboxes(true);
  mdlProducts->setShowCheckboxes(true);
  ui->btnCancel->setEnabled(true);
}


void TestChBFiltMainWindow::on_mdlProdKinds_dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
  qDebug() << __FUNCTION__ << topLeft.column() << bottomRight.column();

  int top = topLeft.row();
  int bottom = bottomRight.row();
  int sc = mdlProdKinds->serviceColumn();

  for(int i = top; i <= bottom; ++i)
  {
    if((sc >= topLeft.column()) && (sc <= bottomRight.column()))
    {
      const QModelIndex ind = topLeft.model()->index(i, COL_PROD_KINDS_VISIBLE);
      QString id_str = ind.data(Qt::UserRole).toString();

      int checked = mdlProdKinds->isItemChecked(i);

      qDebug() << "toggle(" << checked << ") ids" << id_str;

      mdlProducts->toggleVisibilityReferenced(checked, id_str, COL_PRODUCTS_REF);
      mdlProducts->invalidate();
    }
  }
}


void TestChBFiltMainWindow::on_mdlProdKinds_selection_changed(const QItemSelection& selected, const QItemSelection& deselected)
{

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
  ui->btnCancel->setEnabled(false);
}
