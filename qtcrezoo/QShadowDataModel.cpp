#include "QShadowDataModel.h"


void QShadowDataModel::makeShadow()
{
  for(int r = 0; r < rowCount(); ++r)
  {
    QVariant v = item(r, shadowColumn)->data(Qt::DisplayRole);
    item(r, targetColumn)->setData(v, Qt::UserRole);
  }
}

void QShadowDataModel::recreate()
{
  QMirrorModel::recreate();
  makeShadow();
}


QShadowDataModel::QShadowDataModel(QAbstractItemModel *src_mdl, int target_col, int shadow_col, QObject *parent) :
  QMirrorModel(src_mdl, parent),
  targetColumn(target_col),
  shadowColumn(shadow_col)
{
  makeShadow();
}
