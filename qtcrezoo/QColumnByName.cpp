#include <QDebug>
#include <QStringList>

#include "qColumnByName.h"


qColumnByName::qColumnByName(QAbstractItemModel *mdl):
  Model(mdl)
{
}


int qColumnByName::getColumnNumber(const QString &col_name) const
{
  return(Column_Number.contains(col_name)? Column_Number[col_name]: -1);
}


void qColumnByName::setColumnNumber(const QString &col_name, int column)
{
  Column_Number[col_name] = column;
}


QVariant qColumnByName::getData(int row, const QString &col_name, Qt::ItemDataRole role) const
{
  int column = getColumnNumber(col_name);
  return(Model->index(row, column).data(role));
}


void qColumnByName::translateHeaders(const QString &new_headers, const QString &splitter)
{
  QStringList new_headers_list = new_headers.split(QRegExp(splitter));

  for(int column = 0; column < Model->columnCount(); column++)
  {
    QString new_hdr = new_headers_list.front();
    new_headers_list.removeFirst();
    Model->setHeaderData(column, Qt::Horizontal, new_hdr);
  }
}
