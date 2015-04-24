#include "QMirrorModel.h"

#include <QDebug>


void QMirrorModel::dub(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
  for(int r = topLeft.row(); r <= bottomRight.row(); ++r)
  {
    for(int c = topLeft.column(); c <= bottomRight.column(); ++c)
    {
      QVariant item = srcModel->index(r, c).data(Qt::DisplayRole);
      QModelIndex  host_index = srcModel->index(r, c);

      if(host_index.data(Qt::DisplayRole) != item)
        setData(index(r, c), item);
    }
  }
}


void QMirrorModel::dub(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
  if(roles.contains(Qt::DisplayRole))
    dub(topLeft, bottomRight);
}


void QMirrorModel::on_columnsInserted(const QModelIndex &parent, int first, int last)
{
  qDebug() << "QMirrorModel::on_columnsInserted";
  recreate();
}

void QMirrorModel::on_columnsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int column)
{
  qDebug() << "QMirrorModel::on_columnsMoved";
  recreate();
}

void QMirrorModel::on_columnsRemoved(const QModelIndex &parent, int first, int last)
{
  qDebug() << "QMirrorModel::on_columnsRemoved";
  recreate();
}

void QMirrorModel::on_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
  qDebug() << "QMirrorModel::on_dataChanged";
  dub(topLeft, bottomRight, roles);
}

void QMirrorModel::on_headerDataChanged(Qt::Orientation orientation, int first, int last)
{
  qDebug() << "QMirrorModel::on_headerDataChanged";
  for(int i = first; i <= last; ++i)
  {
    QVariant hdr = srcModel->headerData(i, orientation, Qt::DisplayRole);

    if(headerData(i, orientation) != hdr)
      setHeaderData(i, orientation, Qt::DisplayRole);
  }
}

void QMirrorModel::on_modelReset()
{
  qDebug() << "QMirrorModel::on_modelReset";
  recreate();
}

void QMirrorModel::on_rowsInserted(const QModelIndex &parent, int first, int last)
{
  qDebug() << "QMirrorModel::on_rowsInserted";
  recreate();
}

void QMirrorModel::on_rowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
  qDebug() << "QMirrorModel::on_rowsMoved";
  recreate();
}

void QMirrorModel::on_rowsRemoved(const QModelIndex &parent, int first, int last)
{
  qDebug() << "QMirrorModel::on_rowsRemoved";
  recreate();
}


QMirrorModel::QMirrorModel(QAbstractItemModel *src_mdl, QObject *parent):
  srcModel(src_mdl),
  QStandardItemModel(parent)
{
  recreate();

  if(src_mdl)
  {
    connect(src_mdl, SIGNAL(columnsInserted(QModelIndex,int,int)), this, SLOT(on_columnsInserted(QModelIndex,int,int)));
    connect(src_mdl, SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(on_columnsMoved(QModelIndex,int,int,QModelIndex,int)));
    connect(src_mdl, SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(on_columnsRemoved(QModelIndex,int,int)));
    connect(src_mdl, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(on_dataChanged(QModelIndex,QModelIndex)));
    connect(src_mdl, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(on_dataChanged(QModelIndex,QModelIndex,QVector<int>)));
    connect(src_mdl, SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(on_headerDataChanged(Qt::Orientation,int,int)));
    connect(src_mdl, SIGNAL(modelReset()), this, SLOT(on_modelReset()));
    connect(src_mdl, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(on_rowsInserted(QModelIndex,int,int)));
    connect(src_mdl, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(on_rowsMoved(QModelIndex,int,int,QModelIndex,int)));
    connect(src_mdl, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(on_rowsRemoved(QModelIndex,int,int)));
  }
}


void QMirrorModel::recreate()
{
  clear();

  int rows = srcModel->rowCount();
  int columns = srcModel->columnCount();

  qDebug() << "QMirrorModel::recreate" << rows << columns;

  setRowCount(rows);
  setColumnCount(columns);

  for(int r = 0; r < rows; ++r)
    for(int c = 0; c < columns; ++c)
    {
      QVariant v = srcModel->index(r, c).data(Qt::DisplayRole);
      setData(index(r, c), v, Qt::DisplayRole);
    }


  emit sig_recreated();
  qDebug() << "QMirrorModel::recreate finished!";
}
