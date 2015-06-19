#ifndef Q_CHECKBOX_FILTER_MODEL_H
#define Q_CHECKBOX_FILTER_MODEL_H

#include <QSortFilterProxyModel>

#include "qMirrorModel.h"


class qCheckboxFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

  protected:
    qMirrorModel srcModel;
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

    QStandardItem* actualSrcItem(int source_row) const;

    void setSrcItemState(int source_row, bool checked, bool locked, QStandardItem* actual_item = NULL);

    int sourceRow(int row) const;


  public:
    qCheckboxFilterModel(QAbstractItemModel* src_mdl, int visible_col, int id_col, QObject *parent = 0);

    void setShowCheckboxes(bool checkable);

    void toggleItems(bool checked, const QString &vals, bool locked = false);

    void toggleAllItems(bool checkable, bool checked, bool locked = false, bool change_disabled = true);

    void toggleItemsReferenced(bool checked, const QString& ids_str, bool locked, int column);

    void brush(const QBrush& brush, const QString& val, int column);

    bool isSrcItemChecked(int source_row) const;

    bool isItemChecked(int row) const;

    bool isCheckboxesVisible(void) const { return ShowCheckboxes; }

    void setPattern(const QString& pat){ Pattern = pat; }

    int visibleItemsCount(void) const;

    Qt::CheckState generalCheckState(void) const;

    QString getCheckedIds(void) const;

    QString getEnabledCheckedIds(void) const;

    QString getSrcId(int source_row) const;

    QString getId(int row) const;

  signals:
    void sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
};

#endif // Q_CHECKBOX_FILTER_MODEL_H
