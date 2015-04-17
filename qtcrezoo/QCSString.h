#ifndef Q_CS_STRING_H
#define Q_CS_STRING_H

#include <QString>

class q_CS_String : public QString
{
  protected:
    QString Separator;

  public:
    q_CS_String();

    q_CS_String(const char* s, const QString& separator = ","):
      QString(s), Separator(separator){}

    q_CS_String(const QString& s, const QString& separator = ","):
      QString(s), Separator(separator){}

    const q_CS_String& append(const QString& s)
    {
      if(size()) QString::append(Separator);
      QString::append(s);
      return(*this);
    }

};

#endif // Q_CS_STRING_H
