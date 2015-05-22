#include "QCheckboxFilterModel.h"
#include "QCSString.h"

#include <QDebug>


bool QCheckboxFilterModel::filterAcceptsRow(int source_row, const QModelIndex &ind) const
{
  if((source_row >= srcModel.rowCount()) || (source_row < 0)) return false;

  QStandardItem* item = actualItem(source_row);

  // Если чек-боксы отображаются, видны помеченные элементы. Иначе - перечисленные как видимые в VisibleItems.
  bool Result = ShowCheckboxes || VisibleItems[actualItemId(item)];

  if(Result && Pattern.length())
  {
    Result = item->data(Qt::DisplayRole).toString().contains(Pattern, Qt::CaseInsensitive);
  }

  return Result;
}


QMap<QString, bool> QCheckboxFilterModel::strToMap(const QString &str) const
{
  QStringList numbers_str = strToList(str);
  QMap<QString, bool> ids;

  foreach (const QString& id, numbers_str)
    ids[id] = true;

  return ids;
}


bool QCheckboxFilterModel::isItemChecked(QStandardItem *item) const
{
  bool res = ShowCheckboxes?
        (item? (item->data(Qt::CheckStateRole).toInt() != Qt::Unchecked): false):
        VisibleItems[actualItemId(item)];

  return res;
}


void QCheckboxFilterModel::setItemState(QStandardItem* item, bool checked, bool locked)
{
  QString id = actualItemId(item);

  if(ShowCheckboxes)
  {
    item->setData(checked? Qt::Checked: Qt::Unchecked, Qt::CheckStateRole);
  }
  else
  {
    VisibleItems[id] = checked;
    QModelIndex ind = item->index();
    emit dataChanged(ind, ind);
  }

  item->setEnabled(!locked);
}


void QCheckboxFilterModel::setup()
{
  // Checking for a source model resize.
  //connect(&srcModel, SIGNAL(sig_recreated()), this, SLOT(on_srcModel_recreated()));

  // Saving state of checkboxes.
  //connect(&srcModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(on_srcModel_dataChanged(QModelIndex,QModelIndex,QVector<int>)));
}


QCheckboxFilterModel::QCheckboxFilterModel(QAbstractItemModel *src_mdl, int visible_col, int id_col, QObject *parent):
  QSortFilterProxyModel(parent),
  srcModel(src_mdl, visible_col, id_col, parent),
  ShowCheckboxes(false),
  Checkboxed_Column(visible_col),
  Id_Column(id_col)
{
  setSourceModel(&srcModel);
}


// Переключение режима отображения:
// checkable - с чек-боксами.
void QCheckboxFilterModel::setShowCheckboxes(bool checkable)
{
  ShowCheckboxes = checkable;

  if(checkable)
  {
    for(int r = 0; r < srcModel.rowCount(); ++r)
    {
      QStandardItem* item = actualItem(r);
      QString id = item->data(Qt::UserRole).toString();

      item->setCheckable(true);
      item->setData(VisibleItems[id]? Qt::Checked:Qt::Unchecked, Qt::CheckStateRole);
    }
  }
  else
  {
    VisibleItems.clear();

    for(int r = 0; r < srcModel.rowCount(); ++r)
    {
      QStandardItem* item = actualItem(r);
      QString id = item->data(Qt::UserRole).toString();

      bool checked = (item->data(Qt::CheckStateRole).toInt() == Qt::Checked);
      VisibleItems[id] = checked;

      item->setCheckable(false);
      item->setData(QVariant(), Qt::CheckStateRole);
    }
  }

  invalidate();
}

// Переключение состояния указанных элементов.
// checked - отметить,
// locked - заблокировать.
void QCheckboxFilterModel::toggleItems(bool checked, const QString &vals, bool locked)
{
  if(vals.isEmpty()) return;

  QMap<QString, bool> ids = strToMap(vals);

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = actualItem(r);
    QString id = item->data(Qt::UserRole).toString();

    if(ids.contains(id))
      setItemState(item, checked, locked);
  }
}


void QCheckboxFilterModel::toggleAllItems(bool checkable, bool checked, bool locked)
{
  bool changeCheckState = (ShowCheckboxes != checkable);
  ShowCheckboxes = checkable;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = actualItem(r);
    item->setCheckable(checkable);

    if(changeCheckState && !ShowCheckboxes)
      item->setData(QVariant(), Qt::CheckStateRole);

    setItemState(item, checked, locked);
  }
}


void QCheckboxFilterModel::toggleItemsReferenced(bool checked, const QString &ids, bool locked, int column)
{
  qDebug() << objectName() << "toggleItemsReferenced";

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = actualItem(r);
    QString ref_id = srcModel.item(r, column)->data(Qt::DisplayRole).toString();

    //qDebug() << "id:" << id << "ref_id:" << ref_id << srcModel.item(r, 1)->data(Qt::DisplayRole).toString();

    if(ids.contains(ref_id))
      setItemState(item, checked, locked);
  }
}


void QCheckboxFilterModel::brush(const QBrush& brush, const QString& val, int column)
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, column)->data(Qt::DisplayRole).toString();
    if(id == val)
      actualItem(r)->setData(brush, Qt::BackgroundRole);
  }
}


QString QCheckboxFilterModel::sourceItemData(int row, int column, int role) const
{
  QString v(srcModel.item(row, column)->data(role).toString());
  qDebug() << objectName() << "sourceItemData" << row << column << role << "=" << v;
  return v;
}
