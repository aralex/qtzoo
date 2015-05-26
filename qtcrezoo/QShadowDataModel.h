#ifndef QSHADOWDATAMODEL_H
#define QSHADOWDATAMODEL_H

#include "qMirrorModel.h"

/*
The model stores a value from shadowColumn/DisplayRole in targetColumn/UserRole.
*/

class QShadowDataModel : public qMirrorModel
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
    void sig_Recreated(void);

  public slots:

};

#endif // QSHADOWDATAMODEL_H
