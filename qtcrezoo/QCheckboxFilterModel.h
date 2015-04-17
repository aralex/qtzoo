#ifndef Q_CHECKBOX_FILTER_MODEL_H
#define Q_CHECKBOX_FILTER_MODEL_H

#include <QSortFilterProxyModel>

#include <QShadowDataModel.h>


class q_Checkbox_Filter_Model : public QSortFilterProxyModel
{
  protected:
    QShadowDataModel srcModel;
    QString Pattern;
    bool ShowUnchecked;
    bool ShowCheckboxes;
    int Checkboxed_Column;
    int Id_Column;

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &ind) const;

    QMap<QString, bool> strToMap(const QString& str) const;

  public:
    q_Checkbox_Filter_Model(QAbstractItemModel* src_mdl, int visible_col, int id_col, QObject *parent = 0);

    void setPattern(const QString& pat){ Pattern = pat; }

    void setShowUnchecked(bool state){ ShowUnchecked = state; }

    void setShowCheckboxes(bool state);

    void setChecked(int state, const QMap<QString, bool>& vals);
    void resetChecked(int state, const QMap<QString, bool>& vals);

    void setChecked(int state, const QString &vals = QString());
    void resetChecked(int state, const QString &vals = QString());

    void setFixed(bool fixed);
    void setCheckedAndFixed(int checked, bool fixed, int column, const QString &vals);

    void check_all(void);
    void uncheck_all(void);

    void setShowUncheckedCheckboxes(bool state);
    void hideCheckboxes(void);

    void setMode(bool checkboxes, bool show_unchecked, const QMap<QString, bool> &vals);

    void setMode(bool checkboxes, bool show_unchecked, const QString &vals);

    void setMode(bool checkboxes, bool show_unchecked, bool enabled);

    void Brush(const QBrush& brush, int column, const QString& val);

    Qt::CheckState generalCheckState(void) const;

    QString getCheckedIds(void) const;

    QString getEnabledCheckedIds(void) const;

  signals:

  public slots:

};

#endif // Q_CHECKBOX_FILTER_MODEL_H
