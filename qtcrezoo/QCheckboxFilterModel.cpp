#include "QCheckboxFilterModel.h"
#include "QCSString.h"

#include <QDebug>


bool QCheckboxFilterModel::filterAcceptsRow(int source_row, const QModelIndex &ind) const
{
  /*
  qDebug() << objectName() << "QCheckboxFilterModel::filterAcceptsRow" << source_row <<
              srcModel.item(source_row, Checkboxed_Column)->data(Qt::DisplayRole).toString() <<
              srcModel.item(source_row, Checkboxed_Column)->data(Qt::CheckStateRole).toInt() <<
              srcModel.item(source_row, Service_Column)->data(Qt::DisplayRole).toBool() <<
              srcModel.item(source_row, Service_Column)->data(Qt::CheckStateRole).toInt();
  */

  if((source_row >= srcModel.rowCount()) || (source_row < 0)) return false;

  //qDebug() << "Vizible items count:" << visibleItems.size();

  bool Result = srcModel.item(source_row, Service_Column)->data(Qt::DisplayRole).toBool();

  //qDebug() << "Vizible:" << Result;

  if(Result && Pattern.length())
  {
    QStandardItem* actual_item = srcModel.item(source_row, Checkboxed_Column);
    QString s(actual_item->data(Qt::DisplayRole).toString());
    Result = s.contains(Pattern, Qt::CaseInsensitive);
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


QMap<QString, bool> QCheckboxFilterModel::rangeToMap(const QItemSelection &range, int column) const
{
  qDebug() << __FUNCTION__;

  QMap<QString, bool> ids;

  foreach(const QModelIndex& ind, range.indexes())
    if(ind.column() == column)
    {
      QString id_str = ind.data(Qt::DisplayRole).toString();
      ids[id_str] = true;

      qDebug() << "--" << id_str;
    }

  return ids;
}


QList<int> QCheckboxFilterModel::strToIntList(const QString &str) const
{
  QStringList numbers_str = strToList(str);
  QList<int> numbers;
  int n;
  bool ok = true;

  foreach (const QString& n_str, numbers_str)
  {
    n = n_str.toInt(&ok);
    if(ok) numbers.append(n);
  }

  return numbers;
}


void QCheckboxFilterModel::createServiceColumn()
{
  qDebug() << objectName() << __FUNCTION__;

  QList<QStandardItem*> L;

  // Checkbox values items creation.
  for(int i = 0; i < srcModel.rowCount(); ++i)
  {
    QStandardItem* item = new QStandardItem;
    item->setData(true, Qt::DisplayRole);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    L.append(item);
  }

  srcModel.blockSignals(true);

  Service_Column = srcModel.columnCount();
  srcModel.appendColumn(L);                   // Creation of column for checkbox value.

  srcModel.blockSignals(false);

  qDebug() << objectName() << __FUNCTION__ << "finished!";
}


QCheckboxFilterModel::QCheckboxFilterModel(QAbstractItemModel *src_mdl, int visible_col, int id_col, QObject *parent):
  QSortFilterProxyModel(parent),
  srcModel(src_mdl, visible_col, id_col, parent),
  ShowCheckboxes(false),
  Checkboxed_Column(visible_col),
  Id_Column(id_col),
  Service_Column(-1)
{
  setSourceModel(&srcModel);
  createServiceColumn();
}


void QCheckboxFilterModel::setShowCheckboxes(bool checkable)
{
  qDebug() << objectName() << "QCheckboxFilterModel::setShowCheckboxes" << checkable;

  if(ShowCheckboxes == checkable) return;

  ShowCheckboxes = checkable;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* checkboxed_item = srcModel.item(r, Checkboxed_Column);
    QStandardItem* service_item = srcModel.item(r, Service_Column);
    int state = service_item->data(Qt::CheckStateRole).toInt();

    qDebug() << "--" << checkboxed_item->data(Qt::DisplayRole).toString() << service_item->data(Qt::CheckStateRole).toInt();

    checkboxed_item->setCheckable(checkable);
    if(checkable)
    {
      checkboxed_item->setData(state, Qt::CheckStateRole);
      qDebug() << "state" << state;
    }
    else checkboxed_item->setData(QVariant(), Qt::CheckStateRole);
  }
}


void QCheckboxFilterModel::uncheck_all()
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = srcModel.item(r, Checkboxed_Column);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    item->setData(QBrush(), Qt::BackgroundRole);
  }
}


/*
void QCheckboxFilterModel::setShowUncheckedCheckboxes(bool state)
{
  ShowUnchecked = state;
  ShowCheckboxes = true;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = srcModel.item(r, Checkboxed_Column);
    item->setEnabled(state);
  }
}
*/

void QCheckboxFilterModel::hideCheckboxes()
{
  ShowCheckboxes = false;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = srcModel.item(r, Checkboxed_Column);
    item->setEnabled(true);
    item->setCheckable(false);
    item->setData(QVariant(), Qt::CheckStateRole);
    item->setData(QBrush(), Qt::BackgroundRole);
  }
}


/*
Режимы отображения списков:
- все элементы, без чекбоксов;
- все элементы, все с чекбоксами, некоторые помечены;
- некоторые элементы, с чекбоксами, помечены;
*/

/*
void QCheckboxFilterModel::setMode(bool checkboxes, bool show_unchecked, const QMap<QString, bool>& vals)
{
  QString chb = checkboxes? "show": "hide";
  QString unch = show_unchecked? "show": "hide";

  qDebug() << "setMode" << objectName() << QString("%0 checkboxes, %1 unchecked, choose %2 items").arg(chb).arg(unch).arg(vals.size());

  //ShowUnchecked = show_unchecked;
  ShowCheckboxes = checkboxes;

  if(vals.size()) ShowCheckboxes = true;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = srcModel.item(r, Checkboxed_Column);

    item->setCheckable(ShowCheckboxes);

    if(ShowCheckboxes)
    {
      QString id = srcModel.item(r, Id_Column)->data(Qt::DisplayRole).toString();
      item->setCheckState(vals.contains(id)? Qt::Checked: Qt::Unchecked);
      item->setEnabled(show_unchecked);
    }
    else item->setData(QVariant(), Qt::CheckStateRole);
  }
}

void QCheckboxFilterModel::setMode(bool checkboxes, bool show_unchecked, const QString &vals)
{
  QMap<QString, bool> ids = strToMap(vals);
  setMode(checkboxes, show_unchecked, ids);
}


void QCheckboxFilterModel::setMode(bool checkboxes, bool show_unchecked, bool enabled)
{
  QString chb = checkboxes? "show": "hide";
  QString unch = show_unchecked? "show": "hide";
  QString en = enabled? "enabled": "disabled";

  qDebug() << "setMode" << objectName() << QString("%0 checkboxes, %1 unchecked, %2").arg(chb).arg(unch).arg(en);

  //ShowUnchecked = show_unchecked;
  ShowCheckboxes = checkboxes;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = srcModel.item(r, Checkboxed_Column);

    item->setEnabled(true);

    item->setCheckable(ShowCheckboxes);

    if(!ShowCheckboxes)
      item->setData(QVariant(), Qt::CheckStateRole);

    if(!enabled) item->setEnabled(false);
  }
}

void QCheckboxFilterModel::setChecked(int state, const QMap<QString, bool> &vals)
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, Id_Column)->data(Qt::DisplayRole).toString();
    if(vals.contains(id))
      srcModel.item(r, Checkboxed_Column)->setData(state, Qt::CheckStateRole);
  }
}


void QCheckboxFilterModel::resetChecked(int state, const QMap<QString, bool> &vals)
{
  qDebug() << objectName() << __FUNCTION__ << state << vals;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, Id_Column)->data(Qt::DisplayRole).toString();
    if(vals.contains(id))
      srcModel.item(r, Checkboxed_Column)->setData(state, Qt::CheckStateRole);
    else
      srcModel.item(r, Checkboxed_Column)->setData(Qt::Unchecked, Qt::CheckStateRole);
  }
}

void QCheckboxFilterModel::setChecked(int state, const QString &vals)
{
  qDebug() << objectName() << __FUNCTION__ << state << vals;
  QMap<QString, bool> ids = strToMap(vals);
  setChecked(state, ids);
}


void QCheckboxFilterModel::resetChecked(int state, const QString &vals)
{
  QMap<QString, bool> ids = strToMap(vals);
  resetChecked(state, ids);
}


void QCheckboxFilterModel::setFixed(bool fixed)
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = srcModel.item(r, Checkboxed_Column);
    if(item->data(Qt::CheckStateRole).toInt() != Qt::Unchecked) item->setEnabled(!fixed);
  }
}


void QCheckboxFilterModel::setCheckedAndFixed(int checked, bool fixed, int column, const QString &vals)
{
  qDebug() << objectName() << __FUNCTION__ << checked << fixed << column << vals;

  QMap<QString, bool> ids = strToMap(vals);

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, column)->data(Qt::DisplayRole).toString();
    if(ids.contains(id))
    {
      QStandardItem* item = srcModel.item(r, Checkboxed_Column);
      item->setData(checked, Qt::CheckStateRole);
//      item->setEnabled(!fixed);
    }
  }
}


void QCheckboxFilterModel::check_all()
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
    srcModel.item(r, Checkboxed_Column)->setData(Qt::Checked, Qt::CheckStateRole);
}


void QCheckboxFilterModel::Brush(const QBrush& brush, int column, const QString& val)
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    if(srcModel.item(r, column)->data(Qt::DisplayRole) == val)
      srcModel.item(r, Checkboxed_Column)->setData(brush, Qt::BackgroundRole);
  }
}


Qt::CheckState QCheckboxFilterModel::generalCheckState() const
{
  int nChecked = 0;
  int nPartChecked = 0;
  Qt::CheckState res = Qt::Unchecked;
  int N = srcModel.rowCount();

  for(int r = 0; r < N; ++r)
  {
    Qt::CheckState s = srcModel.item(r, Checkboxed_Column)->checkState();
    if(s == Qt::Checked) nChecked++;
    else if(s == Qt::PartiallyChecked) nPartChecked++;
  }

  if(nChecked == N) res = Qt::Checked;
  else if(nChecked || nPartChecked) res = Qt::PartiallyChecked;

  return res;
}


// Получаем список помеченных элементов модели.
QString QCheckboxFilterModel::getCheckedIds(void) const
{
  QCSString s;

  for(int r = 0; r < rowCount(); r++)
  {
    if(srcModel.item(r, Checkboxed_Column)->checkState() != Qt::Unchecked)
      s.append(srcModel.item(r, Id_Column)->data(Qt::DisplayRole).toString());
  }

  return(s);
}


QString QCheckboxFilterModel::getEnabledCheckedIds() const
{
  qDebug() << objectName() << __FUNCTION__;

  QCSString s;

  for(int r = 0; r < rowCount(); r++)
  {
    QStandardItem * item = srcModel.item(r, Checkboxed_Column);

    if(item->isEnabled() && (item->checkState() != Qt::Unchecked))
      s.append(srcModel.item(r, Id_Column)->data(Qt::DisplayRole).toString());
  }

  return(s);
}

*/

void QCheckboxFilterModel::showAllItems()
{
  qDebug() << objectName() << __FUNCTION__;

  for(int r = 0; r < srcModel.rowCount(); r++)
  {
    srcModel.item(r, Service_Column)->setData(true, Qt::DisplayRole);
    srcModel.item(r, Checkboxed_Column)->setEnabled(true);
  }
}


void QCheckboxFilterModel::hideAllItems()
{
  qDebug() << objectName() << __FUNCTION__;

  for(int r = 0; r < srcModel.rowCount(); r++)
  {
    QStandardItem* srv_item = srcModel.item(r, Service_Column);
    srv_item->setData(false, Qt::DisplayRole);
    srv_item->setData(Qt::Unchecked, Qt::CheckStateRole);
  }
}


void QCheckboxFilterModel::toggleVisibility(bool visible, const QString &vals)
{
  //qDebug() << objectName() << __FUNCTION__ << vals;

  QMap<QString, bool> ids = strToMap(vals);

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, Id_Column)->data(Qt::DisplayRole).toString();
    //qDebug() << "id" << id << srcModel.item(r, Checkboxed_Column)->data(Qt::DisplayRole).toString();
    if(ids[id])
    {
      //qDebug() << "present";
      srcModel.item(r, Service_Column)->setData(visible, Qt::DisplayRole);
      srcModel.item(r, Service_Column)->setData(visible? Qt::Checked: Qt::Unchecked, Qt::CheckStateRole);
    }
  }
}


void QCheckboxFilterModel::toggleVisibilityReferenced(bool visible, const QMap<QString, bool>& ids, int column)
{
  //qDebug() << objectName() << __FUNCTION__ << "(bool visible, const QMap<QString, bool>& ids, int column)";

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, column)->data(Qt::DisplayRole).toString();
    if(ids[id])
    {
      srcModel.item(r, Service_Column)->setData(visible, Qt::DisplayRole);
      srcModel.item(r, Service_Column)->setData(visible? Qt::Checked: Qt::Unchecked, Qt::CheckStateRole);

      srcModel.item(r, Checkboxed_Column)->setEnabled(false);
    }
  }
}


void QCheckboxFilterModel::toggleVisibilityReferenced(bool visible, const QString &vals, int column)
{
  //qDebug() << objectName() << __FUNCTION__ << visible << vals << column;

  QMap<QString, bool> ids = strToMap(vals);
  toggleVisibilityReferenced(visible, ids, column);
}


void QCheckboxFilterModel::toggleVisibilityReferenced(bool visible, const QItemSelection &range, int column)
{
  QMap<QString, bool> ids = rangeToMap(range, column);
  qDebug() << objectName() << __FUNCTION__ << "(bool visible, const QItemSelection &range, int column)";
  toggleVisibilityReferenced(visible, ids, column);
}


void QCheckboxFilterModel::showTheseItemsOnly(const QString &vals)
{
  hideAllItems();
  showTheseItems(vals);
}


void QCheckboxFilterModel::setup()
{
  // Checking for a source model resize.
  connect(&srcModel, SIGNAL(sig_recreated()), this, SLOT(on_srcModel_recreated()));

  // Saving state of checkboxes.
  connect(&srcModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(on_srcModel_dataChanged(QModelIndex,QModelIndex,QVector<int>)));
}


void QCheckboxFilterModel::on_srcModel_recreated()
{
  //qDebug() << objectName() << "QCheckboxFilterModel::on_srcModel_recreated";

  createServiceColumn();
}


void QCheckboxFilterModel::on_srcModel_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
  //qDebug() << objectName() << __FUNCTION__ << topLeft.row() << topLeft.column() << bottomRight.row() << bottomRight.column();

  //if((Service_Column < topLeft.column()) || (Service_Column > bottomRight.column())) return;

  //qDebug() << "process dataChanged";

  if((topLeft.column() != Checkboxed_Column) || (bottomRight.column() != Checkboxed_Column)) return;

  int top = topLeft.row();
  int bottom = bottomRight.row();

  for(int r = top; r <= bottom; ++r)
  {
    QStandardItem* checkboxed_item = srcModel.item(r, Checkboxed_Column);
    QStandardItem* service_item = srcModel.item(r, Service_Column);

    int checked = checkboxed_item->data(Qt::CheckStateRole).toInt();
    int checked_prev = service_item->data(Qt::CheckStateRole).toInt();

    if(checked != checked_prev)
      srcModel.item(r, Service_Column)->setData(checked? Qt::Checked: Qt::Unchecked, Qt::CheckStateRole);
  }
}


void QCheckboxFilterModel::brush(const QBrush& brush, const QString& val, int column)
{
  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QString id = srcModel.item(r, column)->data(Qt::DisplayRole).toString();
    if(id == val)
      srcModel.item(r, Checkboxed_Column)->setData(brush, Qt::BackgroundRole);
  }
}

bool QCheckboxFilterModel::isItemChecked(int row) const
{
  QModelIndex ind_src = mapToSource(index(row, 0));
  return (srcModel.item(ind_src.row(), Service_Column)->data(Qt::CheckStateRole) != Qt::Unchecked);
}
