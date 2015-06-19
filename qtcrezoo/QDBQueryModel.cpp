#include <QDebug>
#include <QFile>
#include <QRegExp>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

#include <QMessageBox>

#include "qDBQueryModel.h"
#include "qCSString.h"


QSemaphore DB_Sem(1);


void qDBQueryModel::prepare_query(QSqlQuery& query)
{
  query.prepare(Query_Str);
}


void qDBQueryModel::getData(QString New_Query_Str)
{
  Query_Str = New_Query_Str;
  getData();
}


QString qDBQueryModel::getSPName() const
{
  QRegExp pat_exec("EXEC\\sdbo\\.(\\w+)", Qt::CaseInsensitive);

  QString sp_name;

  if(pat_exec.indexIn(Query_Str) >= 0)
    sp_name = pat_exec.cap(1);
  else
  {
    qDebug() << "SP name not found at:" << Query_Str;
  }

  return(sp_name);
}


void qDBQueryModel::getColumnNames(const QSqlRecord &rec)
{
  int columns = rec.count();

  setColumnCount(columns);

  for(int col = 0; col < columns; col++)
  {
    const QString& col_name = rec.fieldName(col);
    setHeaderData(col, Qt::Horizontal, col_name);
    setColumnNumber(col_name, col);
    // qDebug() << "col_name" << col_name << headerData(col, Qt::Horizontal);
  }
}


QString qDBQueryModel::unquote(const QString &str) const
{
  QString s = ((str[0] == '"') && (str[str.size() - 1] == '"'))? str.mid(1, str.size() - 2): str;
  qDebug() << "Unquoted" << s;
  return(s);
}


QVariant qDBQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant res = (orientation == Qt::Vertical)?
     section:
     QStandardItemModel::headerData(section, orientation, role);
  //return(res);
  return(QStandardItemModel::headerData(section, orientation, role));
}


void qDBQueryModel::load(const QString &fname)
{
  QFile f(fname);
  if(!f.open(QIODevice::ReadOnly))
  {
    QMessageBox::critical(0, "Ошибка!", QString("Ошибка открытия файла '%0'!").arg(fname), QMessageBox::Ok);
    return;
  }

  QList<QStandardItem *> items;
  QString s, header_str;
  QStringList l;
  bool first = true;

  clear();

  while(true)
  {
    char buf[1024];
    if(f.readLine(buf, sizeof(buf)) == -1) break;

    s = buf;
    l = s.split(QRegExp(","));

    if(first)
    {
      header_str = unquote(s);
      first = false;
    }
    else
    {
      for(int col = 0; col < l.size(); col++)
        items << new QStandardItem(unquote(l[col].trimmed()));

      appendRow(items);
      items.clear();
    }
  }

  l = header_str.split(QRegExp(","));
  for(int col = 0; col < l.count(); col++)
  {
    QString col_name = l[col].trimmed();
    setHeaderData(col, Qt::Horizontal, col_name);
    setColumnNumber(col_name, col);
  }

  f.close();
}


void qDBQueryModel::save(const QString &fname) const
{
  QFile f(fname);
  if(!f.open(QIODevice::WriteOnly))
  {
    QMessageBox::critical(0, "Ошибка!", QString("Ошибка открытия файла '%0'!").arg(fname), QMessageBox::Ok);
    return;
  }

  qCSString s;

  for(int col = 0; col < columnCount(); col++)
    s.append(headerData(col, Qt::Horizontal).toString());

  QTextStream(&f) << s << "\n";


  for(int row = 0; row < rowCount(); row++)
  {
    s.clear();

    for(int col = 0; col < columnCount(); col++)
      s.append("\"" + index(row, col).data().toString() + "\"");

    QTextStream(&f) << s << "\n";
  }

  f.close();
  qDebug() << "File" << fname << "saved";
}


void qDBQueryModel::getData(void)
{
  qDebug() << "QDBQueryModel::getData" << Query_Str;

#ifdef QUERY_RESULT_LOAD
  Load(Get_SP_Name() + ".csv");
#else


  clear();

  DB_Sem.acquire();

  do
  {
    QSqlQuery query;

    query.setForwardOnly(true);

    if(!query.exec(Query_Str))
    {
      b_error = true;
      s_error.clear();
      s_error.append(query.lastError().text());

      qDebug() << "Error while query execution " << query.lastError().text();
      break;
    }

    qDebug() << "Query_Str" << Query_Str;

    if(query.isSelect())
    {
      if(query.size())
      {
        // Получаем данные и имена столбцов.
        QList<QStandardItem *> items;
        QStandardItem* item = NULL;

        for(int row = 0; (row? query.next(): query.first()); ++row)
        {
          QSqlRecord rec = query.record();

          // При обработке первой строки, получаем имена столбцов.
          if(!row) getColumnNames(rec);

          for(int col = 0; col < rec.count(); col++)
          {
            QString value = rec.value(col).toString();
            item = rec.value(col).isNull()? new QStandardItem: new QStandardItem(value);

            //qDebug() << QString("%0; %1; %2").arg(row).arg(col).arg(value);

            items << item;
          }

          insertRow(row, items);
          items.clear();
        }
      }
      else
      {
        // Получаем имена столбцов, если данных нет.
        qDebug() << "No data returned!";
        QSqlRecord rec = query.record();
        getColumnNames(rec);
      }
    }
  } while(false);


  DB_Sem.release();

#endif


#ifdef QUERY_RESULT_SAVE
  Save(Get_SP_Name() + ".csv");
#endif

  b_error = false;

  emit sig_data_loaded();
}


void qDBQueryModel::update(void)
{
  //qDebug() << "QDBQueryModel::update";
  getData();
}
