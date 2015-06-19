#ifndef Q_CS_STRING_H
#define Q_CS_STRING_H

#include <QString>


#define CS_DEFAULT_SEPARATOR    ","


class qCSString : public QString
{
  protected:
    QString Separator;

  public:
    qCSString();

    qCSString(const char* s, const QString& separator = CS_DEFAULT_SEPARATOR);

    qCSString(const QString& s, const QString& separator = CS_DEFAULT_SEPARATOR);

    const qCSString& append(const QString& s);
};

#endif // Q_CS_STRING_H
