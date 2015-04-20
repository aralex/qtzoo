#include "QCheckboxFilterModel.h"
#include "QCSString.h"

#include <QDebug>


bool QCheckboxFilterModel::filterAcceptsRow(int source_row, const QModelIndex &ind) const
{
  //qDebug() << "QCheckboxFilterModel::filterAcceptsRow";

  if((source_row >= srcModel.rowCount()) || (source_row < 0)) return false;

  QStandardItem* actual_item = srcModel.item(source_row, Checkboxed_Column);
  bool Result = ShowUnchecked || (actual_item->data(Qt::CheckStateRole).toInt() == Qt::Checked);

  if(Pattern.length())
  {
    QString s(actual_item->data(Qt::DisplayRole).toString());
    Result = Result && s.contains(Pattern, Qt::CaseInsensitive);
  }

  return Result;
}


QMap<QString, bool> QCheckboxFilterModel::strToMap(const QString &str) const
{
  QStringList Checked_Ids = str.split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts);
  QMap<QString, bool> ids;

  foreach (const QString& id, Checked_Ids)
    ids[id] = true;

  return ids;
}


QCheckboxFilterModel::QCheckboxFilterModel(QAbstractItemModel *src_mdl, int visible_col, int id_col, QObject *parent):
  QSortFilterProxyModel(parent),
  srcModel(src_mdl, visible_col, id_col, parent),
  ShowUnchecked(true),
  ShowCheckboxes(false),
  Checkboxed_Column(visible_col),
  Id_Column(id_col)
{
  setSourceModel(&srcModel);
}


void QCheckboxFilterModel::setShowCheckboxes(bool checkable)
{
  if(ShowCheckboxes == checkable) return;

  ShowCheckboxes = checkable;

  for(int r = 0; r < srcModel.rowCount(); ++r)
  {
    QStandardItem* item = srcModel.item(r, Checkboxed_Column);
    item->setCheckable(checkable);
    if(!checkable) item->setData(QVariant(), Qt::CheckStateRole);
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


void QCheckboxFilterModel::hideCheckboxes()
{
  ShowUnchecked = true;
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
void QCheckboxFilterModel::setMode(bool checkboxes, bool show_unchecked, const QMap<QString, bool>& vals)
{
  QString chb = checkboxes? "show": "hide";
  QString unch = show_unchecked? "show": "hide";

  qDebug() << "setMode" << objectName() << QString("%0 checkboxes, %1 unchecked, choose %2 items").arg(chb).arg(unch).arg(vals.size());

  ShowUnchecked = show_unchecked;
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

  ShowUnchecked = show_unchecked;
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
  QCSString s;

  for(int r = 0; r < rowCount(); r++)
  {
    QStandardItem * item = srcModel.item(r, Checkboxed_Column);

    if(item->isEnabled() && (item->checkState() != Qt::Unchecked))
      s.append(srcModel.item(r, Id_Column)->data(Qt::DisplayRole).toString());
  }

  return(s);
}
