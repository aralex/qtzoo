#ifndef Q_CS_STRING_H
#define Q_CS_STRING_H

#include <QString>

class qCSString : public QString
{
  protected:
    QString Separator;

  public:
    qCSString();

    qCSString(const char* s, const QString& separator = ","):
      QString(s), Separator(separator){}

    qCSString(const QString& s, const QString& separator = ","):
      QString(s), Separator(separator){}

    const qCSString& append(const QString& s)
    {
      if(size()) QString::append(Separator);
      QString::append(s);
      return(*this);
    }

};

#endif // Q_CS_STRING_H
