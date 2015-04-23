#include "model_tools.h"
#include "QTextFile.h"


void LoadCsvData(QStandardItemModel* model, const QString& file_name, const QString& separator)
{
  QTextFile f(file_name);
  QString str;
  int n = 0;

  f.setCodec("UTF-8");

  if(f.open(QIODevice::ReadOnly)) while(f.readLine(str))
  {
    QStringList L = str.split(separator, QString::KeepEmptyParts, Qt::CaseSensitive);
    QList<QStandardItem *> items;

    if(!n++) model->setColumnCount(L.count());

    foreach(const QString& s, L)
      items.append(new QStandardItem(s));

    model->appendRow(items);
  }
}
