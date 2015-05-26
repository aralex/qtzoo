#ifndef Q_DB_QUERY_MODEL_H
#define Q_DB_QUERY_MODEL_H

#include <QStandardItemModel>
#include <QtSql/QSqlQuery>
#include <QSemaphore>

#include "qColumnByName.h"


#define c_No_Data           "<нет данных>"


extern QSemaphore DB_Sem;


class qDBQueryModel : public QStandardItemModel, public qColumnByName
{
  Q_OBJECT

  protected:
    QString Query_Str;

    void setAndGet(const QString& SQL)
    {
      if(SQL.size())
      {
        Query_Str = SQL;
        getData();
      }
    }

    QString getSPName(void) const;

    void getColumnNames(const QSqlRecord& rec);

    QString unquote(const QString& str) const;

  public:
    qDBQueryModel(const char * SQL = 0, QObject *parent = 0):
      QStandardItemModel(parent),
      qColumnByName(this)
    {
      setAndGet(SQL);
    }


    qDBQueryModel(const QString& SQL, QObject *parent = 0):
      QStandardItemModel(parent),
      qColumnByName(this)
    {
      setAndGet(SQL);
    }

    qDBQueryModel(const QString& SQL, bool Do_Get_Data, QObject *parent = 0):
      QStandardItemModel(parent),
      qColumnByName(this)
    {
      if(SQL.size()) Query_Str = SQL;
      if(Do_Get_Data) getData();
    }

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void load(const QString& fname);

    void save(const QString& fname) const;

    virtual void getData(void);
    virtual void getData(QString New_Query_Str);

    bool b_error;
    QString s_error;

  public slots:
    virtual void update(void);

  signals:
    void sig_data_loaded();

  protected:
    virtual void prepare_query(QSqlQuery& query);
};


#endif // Q_DB_QUERY_MODEL_H
