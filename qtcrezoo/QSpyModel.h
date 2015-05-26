#ifndef QSPYMODEL_H
#define QSPYMODEL_H

#include "qMirrorModel.h"


class qSpyModel : public qMirrorModel
{
    Q_OBJECT

  protected:

    virtual void recreate(void);
    void transferData(const QModelIndex &ind);

  public:
    qSpyModel(const QAbstractItemModel* src_model,  QObject *parent = 0);

    virtual ~qSpyModel();

};

#endif // QSPYMODEL_H
