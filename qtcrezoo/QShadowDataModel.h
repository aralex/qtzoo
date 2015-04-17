#ifndef QSHADOWDATAMODEL_H
#define QSHADOWDATAMODEL_H

#include "QMirrorModel.h"


class QShadowDataModel : public QMirrorModel
{
    Q_OBJECT

  protected:

    int targetColumn;
    int shadowColumn;

    void makeShadow(void);

    virtual void recreate(void);

  public:
    explicit QShadowDataModel(QAbstractItemModel* src_mdl, int target_col, int shadow_col, QObject *parent = 0);

  signals:

  public slots:

};

#endif // QSHADOWDATAMODEL_H
