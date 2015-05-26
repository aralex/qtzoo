//      QSelectProxy.h
//      Прокси-модель для выборки столбцов из оригинальной модели.
//
//      Copyright ЗАО "Атлас-Карт", 2012
//
//      Разработчики:
//                    Артём Н. Алексеев <alekseev@atlas-kard.ru>
//--------------------------------------------------------------------

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifndef Q_SELECT_PROXY_H_INCLUDED
#define Q_SELECT_PROXY_H_INCLUDED

#include <QAbstractProxyModel>
#include <QString>
#include <QMap>

#include "qColumnByName.h"


class qSelectProxy : public QAbstractProxyModel, public qColumnByName
{
  public:
    explicit qSelectProxy(const QString& Visible_Columns, QObject *parent);

    virtual void setSourceModel(QAbstractItemModel *sourceModel);

    virtual int rowCount(const QModelIndex &parent= QModelIndex()) const;

    virtual int columnCount(const QModelIndex &parent= QModelIndex()) const;

    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent= QModelIndex()) const;

    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::UserRole);

    //virtual QVariant data(const QModelIndex &proxyIndex, int role) const;

    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

  private:
    int Column_Count;

  protected:
    QStringList Visible_Columns;

    QMap<int, int> Orig_Column;
    QMap<int, int> Dest_Column;

    QMap<int, QVariant> Horisontal_Header;
    QMap<int, QVariant> Vertical_Header;

    QMap<QString, int> Column_Number;

  public slots:
    void on_src_model_changed();
};



#endif
