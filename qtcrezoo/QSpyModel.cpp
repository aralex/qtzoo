#include "QSpyModel.h"
#include "QCSString.h"

#include <QDebug>


void QSpyModel::recreate()
{
  clear();

  const int rows = srcModel->rowCount();
  const int columns = srcModel->columnCount();

  qDebug() << "QSpyModel::recreate" << rows << columns;

  setRowCount(rows);
  setColumnCount(columns);

  for(int r = 0; r < rows; ++r)
    for(int c = 0; c < columns; ++c)
    {
      transferData(srcModel->index(r, c));
    }


  emit sig_recreated();
  qDebug() << "QSpyModel::recreate finished!";
}


void QSpyModel::transferData(const QModelIndex &sourceIndex)
{
  QVariant res;
  const int r = sourceIndex.row();
  const int c = sourceIndex.column();

  for(int role = 0; role < Qt::AccessibleDescriptionRole; ++role)
  {
    res.clear();

    switch(role)
    {
      case Qt::DisplayRole:
      case Qt::EditRole:
      {
        QCSString s("", ", ");
        s.append(sourceIndex.data(Qt::DisplayRole).toString());
        s.append(sourceIndex.data(Qt::CheckStateRole).toString());
        s.append(sourceIndex.data(Qt::DecorationRole).toString());
        res = s;
        break;
      }

      case Qt::DecorationRole:
      case Qt::ToolTipRole:
      case Qt::StatusTipRole:
      case Qt::WhatsThisRole:
      case Qt::FontRole:
      case Qt::TextAlignmentRole:
      case Qt::BackgroundRole:
      case Qt::ForegroundRole:
      case Qt::CheckStateRole:
      case Qt::AccessibleTextRole:
      case Qt::AccessibleDescriptionRole:
        break;

      default:
        qDebug() << "Wrong role value" << role << "in QSpyModel::transferData!";
    }

    //qDebug() << "res =" << res.toString();
    setData(index(r, c), res, role);
  }
}


QSpyModel::QSpyModel(const QAbstractItemModel *src_model, QObject *parent):
  QMirrorModel(src_model, parent)
{
  recreate();
}


QSpyModel::~QSpyModel()
{
}
