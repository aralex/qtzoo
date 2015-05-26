#ifndef QFILENAME_H
#define QFILENAME_H

#include <QString>


class qFileName : public QString
{
  public:
    qFileName();

    qFileName(const QString& file_name);

    qFileName(const QString& file_path, const QString& file_name);

    void set(const QString& file_path, const QString& file_name);

    const qFileName operator <<(const QString &file_name) const;

    const qFileName& operator <<=(const QString &file_name);

    qFileName baseName() const;

    qFileName fileName() const;
};


#endif // QFILENAME_H
