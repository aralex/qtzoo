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


  ui->btnOk->setEnabled(false);
  ui->btnCancel->setEnabled(false);


  connect(ui->lvDiets->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_mdlDiets_selection_changed(QItemSelection,QItemSelection)));

  connect(ui->lvProdKinds->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_lvProdKinds_selection_changed(QItemSelection,QItemSelection)));

  connect(mdlProdKinds, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(on_mdlProdKinds_dataChanged(QModelIndex,QModelIndex,QVector<int>)));

  connect(mdlProdKinds, SIGNAL(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(on_ProdKinds_source_changed(QModelIndex,QModelIndex,QVector<int>)));
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

    if(mdlProducts->generalCheckState() == Qt::PartiallyChecked)
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

    if(mdlProducts->generalCheckState() == Qt::PartiallyChecked)
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


void TestChBFiltMainWindow::on_mdlDiets_selection_changed(const QItemSelection &selected, const QItemSelection &deselected) // ???
{
  qDebug() << __FUNCTION__;

  ui->lvProdKinds->selectionModel()->clear();

  if(selected.count())
  {
    FreezeRefs = true;
    mdlProdKinds->toggleAllItems(false, false, false);
    mdlProducts->toggleAllItems(false, false, false);
    FreezeRefs = false;

    QModelIndexList selected_items = selected.indexes();

    currentDietRow = selected_items.first().row();

    foreach(const QModelIndex& ind, selected_items)
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
  ui->btnOk->setEnabled(true);
  ui->btnCancel->setEnabled(true);
  ui->lvDiets->setEnabled(false);
}


void TestChBFiltMainWindow::on_mdlProdKinds_dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{  
  if(FreezeRefs)
  {
    qDebug() << "on_mdlProdKinds_dataChanged -- skip";
    return;
  }
  else
  {
    qDebug() << "on_mdlProdKinds_dataChanged" << topLeft.data(Qt::DisplayRole).toString() << topLeft.data(Qt::UserRole).toInt();
  }

  if((topLeft.column() != COL_PROD_KINDS_NAME) || (bottomRight.column() != COL_PROD_KINDS_NAME))
  {
    qDebug() << "return by column:" << topLeft.column() << bottomRight.column() << COL_PROD_KINDS_NAME;
    return;
  }

  int top = topLeft.row();
  int bottom = bottomRight.row();

  qDebug() << "top" << top << "bottom" << bottom;

  // Проходимся по списку помеченных регионов и жёстко помечаем принадлежащие им пункты доставки/выдачи.
  for(int row = top; row <= bottom; ++row)
  {
    QString id_str = mdlProdKinds->getId(row);

    int checked = mdlProdKinds->isItemChecked(row);

    qDebug() << QString("Checkstate changed on row: %0; state: %1; ids: '%2'").arg(row).arg(checked).arg(id_str);

    mdlProducts->toggleItemsReferenced(checked, id_str, checked, COL_PRODUCTS_REF);
    mdlProducts->invalidate();
  }

  ui->chbProdKinds->setCheckState(mdlProdKinds->generalCheckState());
}


void TestChBFiltMainWindow::on_lvProdKinds_selection_changed(const QItemSelection& selected, const QItemSelection& deselected)
{
  qDebug() << __FUNCTION__;

  QModelIndexList deselected_items = deselected.indexes();
  foreach (const QModelIndex& ind, deselected_items)
  {
    if(ind.column() != COL_PROD_KINDS_VISIBLE) continue;

    QString id_str = mdlProdKinds->getId(ind.row());
    FreezeRefs = true;
    mdlProducts->brush(QBrush(), id_str, COL_PRODUCTS_REF);
    FreezeRefs = false;
  }

  QModelIndexList selected_items = selected.indexes();
  foreach(const QModelIndex& ind, selected_items)
  {
    if(ind.column() != COL_PROD_KINDS_VISIBLE) continue;

    QString id_str = mdlProdKinds->getId(ind.row());
    FreezeRefs = true;
    mdlProducts->brush(QBrush(c_Highlited_List_Item_Bg), id_str, COL_PRODUCTS_REF);
    FreezeRefs = false;
  }
}


void TestChBFiltMainWindow::on_ProdKinds_source_changed(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
  if(FreezeRefs)
  {
    qDebug() << "q_ARM_Dispatcher::on_ProdKinds_source_changed -- skip";
    return;
  }
  else qDebug() << "q_ARM_Dispatcher::on_ProdKinds_source_changed";

  int top = topLeft.row();
  int bottom = bottomRight.row();

  qDebug() << "top" << top << "bottom" << bottom;

  // Проходимся по списку помеченных регионов и жёстко помечаем принадлежащие им пункты доставки/выдачи.
  for(int row = top; row <= bottom; ++row)
  {
    QString id_str = mdlProdKinds->getSrcId(row);

    int checked = mdlProdKinds->isSrcItemChecked(row);

    qDebug() << QString("Checkstate changed on row: %0; state: %1; ids: '%2'").arg(row).arg(checked).arg(id_str);

    mdlProducts->toggleItemsReferenced(checked, id_str, checked, COL_PRODUCTS_REF);
    mdlProducts->invalidate();
  }

  ui->chbProdKinds->setCheckState(mdlProdKinds->generalCheckState());
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


void TestChBFiltMainWindow::on_btnOk_clicked()
{
  QString prod_kinds = mdlProdKinds->getEnabledCheckedIds();
  QString products = mdlProducts->getEnabledCheckedIds();

  mdlDietsFull->item(currentDietRow, COL_DIETS_PROD_KINDS)->setData(prod_kinds, Qt::DisplayRole);
  mdlDietsFull->item(currentDietRow, COL_DIETS_PRODUCTS)->setData(products, Qt::DisplayRole);

  mdlProdKinds->setShowCheckboxes(false);
  mdlProducts->setShowCheckboxes(false);
  ui->btnOk->setEnabled(false);
  ui->btnCancel->setEnabled(false);
  ui->lvDiets->setEnabled(true);
}
