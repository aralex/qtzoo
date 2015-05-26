#include "qCheckboxFilterModel.h"
#include "qCSString.h"

#include <QDebug>


bool qCheckboxFilterModel::filterAcceptsRow(int source_row, const QModelIndex &ind) const
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


QMap<QString, bool> qCheckboxFilterModel::strToMap(const QString &str) const
{
  QStringList numbers_str = strToList(str);
  QMap<QString, bool> ids;

  foreach (const QString& id, numbers_str)
    ids[id] = true;

  return ids;
}


bool qCheckboxFilterModel::isItemChecked(QStandardItem *item) const
{
  bool res = ShowCheckboxes?
        (item? (item->data(Qt::CheckStateRole).toInt() != Qt::Unchecked): false):
        VisibleItems[actualItemId(item)];

  return res;
}


void qCheckboxFilterModel::setItemState(QStandardItem* item, bool checked, bool locked)
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


void qCheckboxFilterModel::setup()
{
  // Checking for a source model resize.
  //connect(&srcModel, SIGNAL(sig_recreated()), this, SLOT(on_srcModel_recreated()));

  // Saving state of checkboxes.
  //connect(&srcModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(on_srcModel_dataChanged(QModelIndex,QModelIndex,QVector<int>)));
}


qCheckboxFilterModel::qCheckboxFilterModel(QAbstractItemModel *src_mdl, int visible_col, int id_col, QObject *parent):
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
void qCheckboxFilterModel::setShowCheckboxes(bool checkable)
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
void qCheckboxFilterModel::toggleItems(bool checked, const QString &vals, bool locked)
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


void qCheckboxFilterModel::toggleAllItems(bool checkable, bool checked, bool locked, bool change_disabled)
{
  bool changeCheckState = (ShowCheckboxes != checkable);
  ShowCheckboxes = checkable;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = actualItem(r);

    if(change_disabled || item->isEnabled())
    {
      item->setCheckable(checkable);

      if(changeCheckState && !ShowCheckboxes)
        item->setData(QVariant(), Qt::CheckStateRole);

      setItemState(item, checked, locked);
    }
  }
}


void qCheckboxFilterModel::toggleItemsReferenced(bool checked, const QString &ids, bool locked, int column)
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


void qCheckboxFilterModel::brush(const QBrush& brush, const QString& val, int column)
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, column)->data(Qt::DisplayRole).toString();
    if(id == val)
      actualItem(r)->setData(brush, Qt::BackgroundRole);
  }
}


QString qCheckboxFilterModel::sourceItemData(int row, int column, int role) const
{
  QString v(srcModel.item(row, column)->data(role).toString());
  qDebug() << objectName() << "sourceItemData" << row << column << role << "=" << v;
  return v;
}


int qCheckboxFilterModel::visibleItemsCount() const
{
  int res = 0;

  if(ShowCheckboxes)
  {
    for(int r = 0; r < srcModel.rowCount(); ++r)
    {
      if(isItemChecked(actualItem(r))) ++res;
    }
  }
  else
  {
    foreach (bool v, VisibleItems)
    {
      if(v) ++res;
    }
  }
  return res;
}


Qt::CheckState qCheckboxFilterModel::genericCheckState() const
{
  int n_checked = visibleItemsCount();

  return (n_checked == srcModel.rowCount())? Qt::Checked:
                                             (!n_checked? Qt::Unchecked: Qt::PartiallyChecked);

}


QString qCheckboxFilterModel::getCheckedIds(void) const
{
  qCSString s;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = actualItem(r);
    if(isItemChecked(item)) s.append(actualItemId(item));
  }

  return s;
}
