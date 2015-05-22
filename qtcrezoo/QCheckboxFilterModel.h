#ifndef Q_CHECKBOX_FILTER_MODEL_H
#define Q_CHECKBOX_FILTER_MODEL_H

#include <QSortFilterProxyModel>
#include <QBitArray>
#include <QItemSelection>

#include <QShadowDataModel.h>


class QCheckboxFilterModel : public QSortFilterProxyModel
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
    QCheckboxFilterModel(QAbstractItemModel* src_mdl, int visible_col, int id_col, QObject *parent = 0);

    void setup(void);

    void setShowCheckboxes(bool checkable);

    void toggleItems(bool checked, const QString &vals, bool locked);

    void toggleAllItems(bool checkable, bool checked, bool locked);

    void toggleItemsReferenced(bool checked, const QString& ids, bool locked, int column);

    void brush(const QBrush& brush, const QString& val, int column);

    bool isItemChecked(int row) const
    {
      return isItemChecked(actualItem(row));
    }

    QString sourceItemData(int row, int column, int role) const;

};

#endif // Q_CHECKBOX_FILTER_MODEL_H


/*
 *
    int Service_Column;
    bool ShowUnchecked;
    QList<int> strToIntList(const QString& str) const;

    QMap<QString, bool> rangeToMap(const QItemSelection& range, int column) const;



    void createServiceColumn();

    int serviceColumn(){ return Service_Column; }

    void setPattern(const QString& pat){ Pattern = pat; }

    void setShowCheckboxes(bool checkable);
    void setShowUnchecked(bool state);

    void setChecked(int state, const QMap<QString, bool>& vals);
    void resetChecked(int state, const QMap<QString, bool>& vals);

    void setChecked(int state, const QString &vals = QString());
    void resetChecked(int state, const QString &vals = QString());

    void setFixed(bool fixed);
    void setCheckedAndFixed(int checked, bool fixed, int column, const QString &vals);

    void check_all(void);
    void uncheck_all(void);

    //void setShowUncheckedCheckboxes(bool state);
    void hideCheckboxes(void);

    //void setMode(bool checkboxes, bool show_unchecked, const QMap<QString, bool> &vals);
    //void setMode(bool checkboxes, bool show_unchecked, const QString &vals);
    //void setMode(bool checkboxes, bool show_unchecked, bool enabled);

    void Brush(const QBrush& brush, int column, const QString& val);

    Qt::CheckState generalCheckState(void) const;

    QString getCheckedIds(void) const;

    QString getEnabledCheckedIds(void) const;

    void showAllItems(void);

    void hideAllItems(void);

    void toggleVisibility(bool visible, const QString& vals);
    void toggleVisibilityReferenced(bool visible, const QMap<QString, bool> &ids, int column);
    void toggleVisibilityReferenced(bool visible, const QString& vals, int column);
    void toggleVisibilityReferenced(bool visible, const QItemSelection& range, int column);

    void showTheseItems(const QString& vals){ toggleVisibility(true, vals); }
    void hideTheseItems(const QString& vals){ toggleVisibility(false, vals); }

    void showTheseReferencedItems(const QString& vals, int column){ toggleVisibilityReferenced(true, vals, column); }
    void showTheseItemsOnly(const QString& vals);


    void brush(const QBrush& brush, const QString& val, int column);

    bool isItemChecked(int row) const;
    //bool isItemVisible(int src_row) const;

    //int visibleItemsCount(void){ return visibleItems.count(); }

    const QAbstractItemModel* getSrcModel() const { return &srcModel; }
    void hideUnchecked();

  signals:

  public slots:
    void on_srcModel_recreated(void);

    void on_srcModel_dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

*/
