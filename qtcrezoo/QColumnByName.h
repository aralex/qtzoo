#ifndef Q_COLUMN_BY_NAME_H
#define Q_COLUMN_BY_NAME_H

#include <QMap>
#include <QString>
#include <QAbstractItemModel>


class qColumnByName
{
  private:
    QMap<QString, int> Column_Number;
    QAbstractItemModel * Model;

  protected:
    void setColumnNumber(const QString& col_name, int column);

  public:
    explicit qColumnByName(QAbstractItemModel * mdl);

    int getColumnNumber(const QString& col_name) const;

    QVariant getData(int row, const QString& col_name, Qt::ItemDataRole role = Qt::DisplayRole) const;

    void translateHeaders(const QString& new_headers, const QString& splitter = ",");

};

#endif // Q_COLUMN_BY_NAME_H
