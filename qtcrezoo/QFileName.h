#ifndef QFILENAME_H
#define QFILENAME_H

#include <QString>


class QFileName : public QString
{
  public:
    QFileName();

    QFileName(const QString& file_name);

    QFileName(const QString& file_path, const QString& file_name);

    void Set(const QString& file_path, const QString& file_name);

    const QFileName operator <<(const QString &file_name) const;

    const QFileName& operator <<=(const QString &file_name);

    QFileName baseName() const;

    QFileName fileName() const;
};


#endif // QFILENAME_H
