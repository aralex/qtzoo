#ifndef Q_COLUMN_BY_NAME_H
#define Q_COLUMN_BY_NAME_H

#include <QMap>
#include <QString>
#include <QAbstractItemModel>


class QColumnByName
{
  private:
    QMap<QString, int> Column_Number;
    QAbstractItemModel * Model;

  protected:
    void Set_Column_Number(const QString& col_name, int column);

  public:
    explicit QColumnByName(QAbstractItemModel * mdl);

    int Get_Column_Number(const QString& col_name) const;

    QVariant Get_Data(int row, const QString& col_name, Qt::ItemDataRole role = Qt::DisplayRole) const;

    void Translate_Headers(const QString& New_Headers, const QString& Splitter = ",");

};

#endif // Q_COLUMN_BY_NAME_H
