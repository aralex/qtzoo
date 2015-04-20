#ifndef Q_DB_QUERY_MODEL_H
#define Q_DB_QUERY_MODEL_H

#include <QStandardItemModel>
#include <QtSql/QSqlQuery>
#include <QSemaphore>

#include "QColumnByName.h"


#define c_No_Data           "<нет данных>"


extern QSemaphore DB_Sem;


class QDBQueryModel : public QStandardItemModel, public QColumnByName
{
  Q_OBJECT

  protected:
    QString Query_Str;

    void Set_N_Get(const QString& SQL)
    {
      if(SQL.size())
      {
        Query_Str = SQL;
        getData();
      }
    }

    QString Get_SP_Name(void) const;

    void Get_Column_Names(const QSqlRecord& rec);

    QString Unquote(const QString& str) const;

  public:
    QDBQueryModel(const char * SQL = 0, QObject *parent = 0):
      QStandardItemModel(parent),
      QColumnByName(this)
    {
      Set_N_Get(SQL);
    }


    QDBQueryModel(const QString& SQL, QObject *parent = 0):
      QStandardItemModel(parent),
      QColumnByName(this)
    {
      Set_N_Get(SQL);
    }

    QDBQueryModel(const QString& SQL, bool Do_Get_Data, QObject *parent = 0):
      QStandardItemModel(parent),
      QColumnByName(this)
    {
      if(SQL.size()) Query_Str = SQL;
      if(Do_Get_Data) getData();
    }

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void Load(const QString& fname);

    void Save(const QString& fname) const;

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
