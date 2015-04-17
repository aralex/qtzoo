#include <QDebug>
#include <QStringList>

#include "q_column_by_name.h"


q_Column_By_Name::q_Column_By_Name(QAbstractItemModel *mdl):
  Model(mdl)
{
}


int q_Column_By_Name::Get_Column_Number(const QString &col_name) const
{
  return(Column_Number.contains(col_name)? Column_Number[col_name]: -1);
}


void q_Column_By_Name::Set_Column_Number(const QString &col_name, int column)
{
  Column_Number[col_name] = column;
}


QVariant q_Column_By_Name::Get_Data(int row, const QString &col_name, Qt::ItemDataRole role) const
{
  int column = Get_Column_Number(col_name);
  return(Model->index(row, column).data(role));
}


void q_Column_By_Name::Translate_Headers(const QString &New_Headers, const QString &Splitter)
{
  QStringList New_Headers_List = New_Headers.split(QRegExp(Splitter));

  for(int column = 0; column < Model->columnCount(); column++)
  {
    QString new_hdr = New_Headers_List.front();
    New_Headers_List.removeFirst();
    Model->setHeaderData(column, Qt::Horizontal, new_hdr);
  }
}
