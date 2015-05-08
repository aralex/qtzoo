#ifndef QSPYMODEL_H
#define QSPYMODEL_H

#include "QMirrorModel.h"


class QSpyModel : public QMirrorModel
{
    Q_OBJECT

  protected:

    virtual void recreate(void);
    void transferData(const QModelIndex &ind);

  public:
    QSpyModel(const QAbstractItemModel* src_model,  QObject *parent = 0);

    virtual ~QSpyModel();

};

#endif // QSPYMODEL_H
