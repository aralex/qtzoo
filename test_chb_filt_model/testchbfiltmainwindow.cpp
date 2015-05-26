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
  buttonCancelClicked(false),
  FreezeRefs(false),
  ChBProdKindsToggledManually(false)
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

  mdlDiets = new qCheckboxFilterModel(mdlDietsFull, COL_DIETS_VISIBLE, COL_DIETS_ID);
  mdlDiets->setObjectName("mdlDiets");
  mdlDiets->toggleAllItems(false, true, false);
  ui->lvDiets->setModel(mdlDiets);
  ui->lvDiets->setModelColumn(COL_DIETS_VISIBLE);

  mdlProdKinds = new qCheckboxFilterModel(mdlProdKindsFull, COL_PROD_KINDS_VISIBLE, COL_PROD_KINDS_ID);
  mdlProdKinds->setObjectName("mdlProdKinds");
  mdlProdKinds->toggleAllItems(false, true, false);
  ui->lvProdKinds->setModel(mdlProdKinds);
  ui->lvProdKinds->setModelColumn(COL_PROD_KINDS_VISIBLE);

  mdlProducts = new qCheckboxFilterModel(mdlProductsFull, COL_PRODUCTS_VISIBLE, COL_PRODUCTS_ID);
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


void TestChBFiltMainWindow::on_chbProdKinds_stateChanged(int ch_state)
{
  qDebug() << __FUNCTION__;

  if(!ChBProdKindsToggledManually || !mdlProdKinds->isCheckboxesVisible()) return;

  if(ch_state == Qt::Checked)
  {
    ChBProdKindsToggledManually = false;
    mdlProdKinds->toggleAllItems(true, true, false);
  }
  else if(ch_state == Qt::PartiallyChecked)
  {
    ui->chbProdKinds->setCheckState(Qt::Checked);
  }
  else
  {
    ChBProdKindsToggledManually = false;
    mdlProdKinds->toggleAllItems(true, false, false);
  }
}


void TestChBFiltMainWindow::on_chbProducts_stateChanged(int ch_state)
{
  qDebug() << __FUNCTION__;

  if(!ChBProductsToggledManually || !mdlProducts->isCheckboxesVisible()) return;

  if(ch_state == Qt::Checked)
  {
    ChBProductsToggledManually = false;
    mdlProducts->toggleAllItems(true, true, false, false);

    if(mdlProducts->genericCheckState() == Qt::PartiallyChecked)
      ui->chbProducts->setCheckState(Qt::PartiallyChecked);
  }
  else if(ch_state == Qt::PartiallyChecked)
  {
    ui->chbProducts->setCheckState(Qt::Checked);
  }
  else
  {
    ChBProductsToggledManually = false;
    mdlProducts->toggleAllItems(true, false, false, false);

    if(mdlProducts->genericCheckState() == Qt::PartiallyChecked)
      ui->chbProducts->setCheckState(Qt::PartiallyChecked);
  }
}


void TestChBFiltMainWindow::on_lePatternDiets_textChanged(const QString &arg1)
{
  qDebug() << __FUNCTION__;
}

void TestChBFiltMainWindow::on_lePatternProdKinds_textChanged(const QString &arg1)
{
  qDebug() << __FUNCTION__;
  mdlProdKinds->setPattern(arg1);
  mdlProdKinds->invalidate();
}


void TestChBFiltMainWindow::on_mdlDiets_selection_changed(const QItemSelection &selected, const QItemSelection &deselected)
{
  qDebug() << __FUNCTION__;

  ui->lvProdKinds->selectionModel()->clear();

  if(selected.count())
  {
    FreezeRefs = true;
    mdlProdKinds->toggleAllItems(false, false, false);
    mdlProducts->toggleAllItems(false, false, false);
    FreezeRefs = false;

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
  qDebug() << __FUNCTION__ << QString("(%0, %1)-(%2, %3)").arg(topLeft.row()).arg(topLeft.column()).arg(bottomRight.row()).arg(bottomRight.column()) << roles;

  if(buttonCancelClicked)
  {
    qDebug() << "return by buttonCancelClicked";
    return;
  }

  if(FreezeRefs)
  {
    qDebug() << "return by FreezeRefs";
    return;
  }

  if((topLeft.column() != COL_PROD_KINDS_VISIBLE) || (bottomRight.column() != COL_PROD_KINDS_VISIBLE))
  {
    qDebug() << "return by column:" << topLeft.column() << bottomRight.column() << COL_PROD_KINDS_VISIBLE;
    return;
  }

  int top = topLeft.row();
  int bottom = bottomRight.row();

  for(int i = top; i <= bottom; ++i)
  {
    QString id_str = mdlProdKinds->sourceItemData(i, COL_PROD_KINDS_VISIBLE, Qt::UserRole);

    int checked = mdlProdKinds->isItemChecked(i);

    qDebug() << "toggle(" << checked << ") ids" << id_str;

    mdlProducts->toggleItemsReferenced(checked, id_str, checked, COL_PRODUCTS_REF);
    mdlProducts->invalidate();
  }

  ui->chbProdKinds->setCheckState(mdlProdKinds->genericCheckState());
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
  mdlProducts->setPattern(arg1);
  mdlProducts->invalidate();
}


void TestChBFiltMainWindow::on_btnCancel_clicked()
{
  FreezeRefs = true;
  mdlProdKinds->toggleAllItems(false, false, false);
  mdlProducts->toggleAllItems(false, false, false);
  FreezeRefs = false;

  qDebug() << "SelectedProdKinds" << SelectedProdKinds;
  qDebug() << "SelectedProducts" << SelectedProducts;

  mdlProducts->toggleItems(true, SelectedProducts, false);
  mdlProdKinds->toggleItems(true, SelectedProdKinds, false);

  ui->btnCancel->setEnabled(false);
  ui->lvDiets->setEnabled(true);
}


void TestChBFiltMainWindow::on_chbProdKinds_pressed()
{
  qDebug() << __FUNCTION__;
  ChBProdKindsToggledManually = true;
}


void TestChBFiltMainWindow::on_chbProducts_pressed()
{
  qDebug() << __FUNCTION__;
  ChBProductsToggledManually = true;
}
