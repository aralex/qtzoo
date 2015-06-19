#include <QDebug>

#include "qCheckboxFilterModel.h"
#include "qCSString.h"


bool qCheckboxFilterModel::filterAcceptsRow(int source_row, const QModelIndex &ind) const
{
  if((source_row >= srcModel.rowCount()) || (source_row < 0)) return false;

  QStandardItem* item = actualSrcItem(source_row);
  QString id = getSrcId(source_row);

  // Если чек-боксы отображаются, видны помеченные элементы. Иначе - перечисленные как видимые в VisibleItems.
  bool Result = ShowCheckboxes || VisibleItems[id];

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


QStandardItem *qCheckboxFilterModel::actualSrcItem(int source_row) const
{
  return srcModel.item(source_row, Checkboxed_Column);
}


void qCheckboxFilterModel::setSrcItemState(int source_row, bool checked, bool locked, QStandardItem *actual_item)
{
  if(!actual_item) actual_item = actualSrcItem(source_row);

  if(ShowCheckboxes)
  {
    actual_item->setData(checked? Qt::Checked: Qt::Unchecked, Qt::CheckStateRole);
  }
  else
  {
    VisibleItems[getSrcId(source_row)] = checked;
    QModelIndex ind = actual_item->index();
    emit sourceDataChanged(ind, ind, QVector<int>());
  }

  actual_item->setEnabled(!locked);
}


qCheckboxFilterModel::qCheckboxFilterModel(QAbstractItemModel *src_mdl, int visible_col, int id_col, QObject *parent):
  QSortFilterProxyModel(parent),
  srcModel(src_mdl, parent),
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
      QStandardItem* item = actualSrcItem(r);
      QString id = getSrcId(r);

      item->setCheckable(true);
      item->setData(VisibleItems[id]? Qt::Checked:Qt::Unchecked, Qt::CheckStateRole);
    }
  }
  else
  {
    VisibleItems.clear();

    for(int r = 0; r < srcModel.rowCount(); ++r)
    {
      QStandardItem* item = actualSrcItem(r);
      QString id = getSrcId(r);

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
    if(ids.contains(getSrcId(r)))
      setSrcItemState(r, checked, locked);
  }
}


void qCheckboxFilterModel::toggleAllItems(bool checkable, bool checked, bool locked, bool change_disabled)
{
  bool changeCheckState = (ShowCheckboxes != checkable);
  ShowCheckboxes = checkable;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* actual_item = actualSrcItem(r);

    if(change_disabled || actual_item->isEnabled())
    {
      actual_item->setCheckable(checkable);

      if(changeCheckState && !ShowCheckboxes)
        actual_item->setData(QVariant(), Qt::CheckStateRole);

      setSrcItemState(r, checked, locked, actual_item);
    }
  }
}


void qCheckboxFilterModel::toggleItemsReferenced(bool checked, const QString &ids_str, bool locked, int column)
{
  qDebug() << objectName() << "toggleItemsReferenced";

  QMap<QString, bool> ids = strToMap(ids_str);

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString ref_id = srcModel.item(r, column)->data(Qt::DisplayRole).toString();

    //qDebug() << "ref_id:" << ref_id << srcModel.item(r, column)->data(Qt::DisplayRole).toString() << srcModel.item(r, 1)->data(Qt::DisplayRole).toString();

    if(ids.contains(ref_id))
      setSrcItemState(r, checked, locked);
  }
}


void qCheckboxFilterModel::brush(const QBrush& brush, const QString& val, int column)
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, column)->data(Qt::DisplayRole).toString();
    if(id == val)
      actualSrcItem(r)->setData(brush, Qt::BackgroundRole);
  }
}


bool qCheckboxFilterModel::isSrcItemChecked(int source_row) const
{
  bool res;

  if(ShowCheckboxes)
  {
    QStandardItem* item = actualSrcItem(source_row);
    res = item? (item->data(Qt::CheckStateRole).toInt() != Qt::Unchecked): false;
  }
  else res = VisibleItems[getSrcId(source_row)];

  return res;
}


bool qCheckboxFilterModel::isItemChecked(int row) const
{
  return isSrcItemChecked(sourceRow(row));
}


int qCheckboxFilterModel::visibleItemsCount() const
{
  int res = 0;

  for(int r = 0; r < srcModel.rowCount(); ++r)
    if(isSrcItemChecked(r)) ++res;

  return res;
}


Qt::CheckState qCheckboxFilterModel::generalCheckState() const
{
  int n_checked = visibleItemsCount();

  return (n_checked == srcModel.rowCount())? Qt::Checked:
                                             (!n_checked? Qt::Unchecked: Qt::PartiallyChecked);
}


QString qCheckboxFilterModel::getCheckedIds(void) const
{
  qCSString s;

  for(int r = 0; r < srcModel.rowCount(); ++r)
    if(isSrcItemChecked(r)) s.append(getSrcId(r));

  return s;
}


QString qCheckboxFilterModel::getEnabledCheckedIds() const
{
  qCSString s;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = actualSrcItem(r);
    if(isSrcItemChecked(r) && item->isEnabled()) s.append(getSrcId(r));
  }

  return s;
}


int qCheckboxFilterModel::sourceRow(int row) const
{
  QModelIndex src_index = mapToSource(index(row, 0));
  return src_index.row();
}


QString qCheckboxFilterModel::getSrcId(int source_row) const
{
  return ((source_row >= 0) && (source_row < srcModel.rowCount()))?
        srcModel.item(source_row, Id_Column)->data(Qt::DisplayRole).toString():
        QString();
}


QString qCheckboxFilterModel::getId(int row) const
{
  return getSrcId(sourceRow(row));
}
