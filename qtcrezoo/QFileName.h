#ifndef QFILENAME_H
#define QFILENAME_H

#include <QString>
#include "newtypes.h"


class QFileName : public QString
{
  public:
    QFileName();

    QFileName(const QString& file_name);

    QFileName(const QString& file_path, const QString& file_name);

    void Set(const QString& file_path, const QString& file_name);

    const QFileName operator <<(const QString &file_name) const;

    const QFileName& operator <<=(const QString &file_name);

    const BASE_CHAR_TYPE* c_str(void) const;

    QFileName baseName() const;

    QFileName fileName() const;
};


#endif // QFILENAME_H
