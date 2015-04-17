//      q_select_proxy.cpp
//      Прокси-модель для выборки столбцов из оригинальной модели.
//
//      Copyright ЗАО "Атлас-Карт", 2012
//
//      Разработчики:
//                    Артём Н. Алексеев <alekseev@atlas-kard.ru>
//--------------------------------------------------------------------

#include <QStringList>
#include <QRegExp>
#include <QDebug>

#include "q_select_proxy.h"


q_Select_Proxy::q_Select_Proxy(const QString &Visible_Columns_List, QObject *parent=0):
  QAbstractProxyModel(parent),
  q_Column_By_Name(this)
{
  QString cols(Visible_Columns_List);
  Visible_Columns = cols.split(QRegExp("\\s*,\\s*"));
  Column_Count = Visible_Columns.size();
  //connect(this, SIGNAL(sourceModelChanged()), this, SLOT(on_src_model_changed()));
}


void q_Select_Proxy::setSourceModel(QAbstractItemModel *sourceModel)
{
  int Src_Column_Count = sourceModel->columnCount();
  int N_Dest_Col;
  QString Col_Name;


  for(int i = 0; i < Src_Column_Count; i++)
  {
    Col_Name = sourceModel->headerData(i, Qt::Horizontal).toString();
    N_Dest_Col = Visible_Columns.indexOf(Col_Name);
    Column_Number[Col_Name] = N_Dest_Col;
    Set_Column_Number(Col_Name, N_Dest_Col);

    //qDebug() << "Column" << Col_Name << "(" << N_Dest_Col << ")";

    // Если имя столбца исходной модели есть в списке, настраиваем его отображение.
    if(N_Dest_Col >= 0)
    {
      Orig_Column[N_Dest_Col] = i;
      Dest_Column[i] = N_Dest_Col;

      //qDebug() << "Dest_Column[" << i << "] =" << N_Dest_Col;
    }
  }

  QAbstractProxyModel::setSourceModel(sourceModel);
}


int q_Select_Proxy::rowCount(const QModelIndex &) const
{
  //qDebug() << "rowCount" << sourceModel()->rowCount();
  return sourceModel()->rowCount();
}


int q_Select_Proxy::columnCount(const QModelIndex &) const
{
  //qDebug() << "columnCount" << Column_Count;
  return Column_Count;
}


QModelIndex q_Select_Proxy::parent(const QModelIndex &) const
{
  return QModelIndex();
}


QModelIndex q_Select_Proxy::index(int row, int column, const QModelIndex &) const
{
  //qDebug() << "index " << row << column;
  if(
     (row < 0) ||
     (row >= sourceModel()->rowCount()) ||
     (column < 0) ||
     (column >= Column_Count)) return QModelIndex();
  else return createIndex(row, column);
}


QModelIndex q_Select_Proxy::mapFromSource(const QModelIndex &sourceIndex) const
{
  //qDebug() << "mapFromSource " << sourceIndex.row() << sourceIndex.column();

  if(!sourceIndex.isValid()) return QModelIndex();

  return Dest_Column.contains(sourceIndex.column())?
    createIndex(sourceIndex.row(), Dest_Column[sourceIndex.column()]):
    QModelIndex();
}


QModelIndex q_Select_Proxy::mapToSource(const QModelIndex &proxyIndex) const
{
  //qDebug() << "mapToSource " << proxyIndex.row() << proxyIndex.column();

  if(!proxyIndex.isValid()) return QModelIndex();

  return Orig_Column.contains(proxyIndex.column())?
    sourceModel()->index(proxyIndex.row(), Orig_Column[proxyIndex.column()]):
    QModelIndex();
}


QVariant q_Select_Proxy::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role == Qt::DisplayRole)
  {
    if(orientation == Qt::Vertical)
      return QVariant(section + 1);
    else if(Horisontal_Header.contains(section))
    {
      return Horisontal_Header[section];
    }
    else
    {
      section = Orig_Column.contains(section)? Orig_Column[section]: -1;
      return sourceModel()->headerData(section, orientation, role);
    }
  }
  else return sourceModel()->headerData(section, orientation, role);
}


bool q_Select_Proxy::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
  switch(orientation)
  {
    case Qt::Horizontal:
      Horisontal_Header[section] = value;
      break;
    case Qt::Vertical:
      Vertical_Header[section] = value;
  }

  sourceModel()->setHeaderData(section, orientation, value, Qt::DisplayRole);
  sourceModel()->setHeaderData(section, orientation, value, Qt::EditRole);

  return true;
}


void q_Select_Proxy::sort(int column, Qt::SortOrder order)
{
  int orig_column = Orig_Column[column];
  if(orig_column >= 0)
  {
    sourceModel()->sort(orig_column, order);
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
  }
}


void q_Select_Proxy::on_src_model_changed()
{
  qDebug() << "q_Select_Proxy::on_src_model_changed";
  emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

//QVariant q_Select_Proxy::data(const QModelIndex &proxyIndex, int role) const
//{
//  return mapToSource(proxyIndex).data(role);
//}
