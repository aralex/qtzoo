#ifndef Q_CHECKBOX_FILTER_MODEL_H
#define Q_CHECKBOX_FILTER_MODEL_H

#include <QSortFilterProxyModel>

#include <qShadowDataModel.h>


class qCheckboxFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

  protected:
    QShadowDataModel srcModel;
    QString Pattern;

    bool ShowCheckboxes;
    QMap<QString, bool> VisibleItems;

    int Checkboxed_Column;
    int Id_Column;

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &ind) const;

    QStringList strToList(const QString& str) const
    {
      QStringList numbers_str = str.split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts);
      return numbers_str;
    }

    QMap<QString, bool> strToMap(const QString& str) const;

    QStandardItem* actualItem(int row) const
    {
      return srcModel.item(row, Checkboxed_Column);
    }

    QString actualItemId(QStandardItem* item) const
    {
      return item? item->data(Qt::UserRole).toString(): QString();
    }

    bool isItemChecked(QStandardItem* item) const;

    void setItemState(QStandardItem *item, bool checked, bool locked);


  public:
    qCheckboxFilterModel(QAbstractItemModel* src_mdl, int visible_col, int id_col, QObject *parent = 0);

    void setup(void);

    void setShowCheckboxes(bool checkable);

    void toggleItems(bool checked, const QString &vals, bool locked);

    void toggleAllItems(bool checkable, bool checked, bool locked, bool change_disabled = true);

    void toggleItemsReferenced(bool checked, const QString& ids, bool locked, int column);

    void brush(const QBrush& brush, const QString& val, int column);

    bool isItemChecked(int row) const
    {
      return isItemChecked(actualItem(row));
    }

    bool isCheckboxesVisible(void) const { return ShowCheckboxes; }

    QString sourceItemData(int row, int column, int role) const;

    void setPattern(const QString& pat){ Pattern = pat; }

    int visibleItemsCount(void) const;

    bool areAllChecked(void) const { return (visibleItemsCount() == srcModel.rowCount()); }

    Qt::CheckState genericCheckState(void) const;

    QString getCheckedIds(void) const;
};

#endif // Q_CHECKBOX_FILTER_MODEL_H
