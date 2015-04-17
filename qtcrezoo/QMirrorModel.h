#ifndef QMIRRORMODEL_H
#define QMIRRORMODEL_H

#include <QStandardItemModel>


class QMirrorModel: public QStandardItemModel
{
    Q_OBJECT

  protected:
    QAbstractItemModel* srcModel;

  public:
    explicit QMirrorModel(QAbstractItemModel* src_mdl, QObject *parent = 0);

    virtual void recreate(void);

    void dub(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void dub(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int>& roles);

  signals:

  public slots:
    void on_columnsInserted(const QModelIndex & parent, int first, int last);
    void on_columnsMoved(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int column);
    void on_columnsRemoved(const QModelIndex & parent, int first, int last);
    void on_dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int> ());
    void on_headerDataChanged(Qt::Orientation orientation, int first, int last);
    void on_modelReset();
    void on_rowsInserted(const QModelIndex & parent, int first, int last);
    void on_rowsMoved(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int row);
    void on_rowsRemoved(const QModelIndex & parent, int first, int last);
};

#endif // QMIRRORMODEL_H
