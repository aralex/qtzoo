#ifndef QMIRRORMODEL_H
#define QMIRRORMODEL_H

#include <QStandardItemModel>


class qMirrorModel: public QStandardItemModel
{
    Q_OBJECT

  protected:
    const QAbstractItemModel* srcModel;

  public:
    explicit qMirrorModel(const QAbstractItemModel *src_mdl, QObject *parent = 0);

    virtual void recreate(void);

    void dub(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void dub(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int>& roles);

  signals:
    void sig_recreated(void);
    void sig_changed(void);

  public slots:
    void on_columnsInserted(const QModelIndex &, int first, int last);
    void on_columnsMoved(const QModelIndex &, int, int, const QModelIndex &, int);
    void on_columnsRemoved(const QModelIndex &, int, int);
    void on_dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int> ());
    void on_headerDataChanged(Qt::Orientation orientation, int first, int last);
    void on_modelReset();
    void on_rowsInserted(const QModelIndex &, int first, int last);
    void on_rowsMoved(const QModelIndex &, int, int, const QModelIndex &, int);
    void on_rowsRemoved(const QModelIndex &, int, int);
};

#endif // QMIRRORMODEL_H
