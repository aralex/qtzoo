#ifndef Q_CS_STRING_H
#define Q_CS_STRING_H

#include <QString>

class QCSString : public QString
{
  protected:
    QString Separator;

  public:
    QCSString();

    QCSString(const char* s, const QString& separator = ","):
      QString(s), Separator(separator){}

    QCSString(const QString& s, const QString& separator = ","):
      QString(s), Separator(separator){}

    const QCSString& append(const QString& s)
    {
      if(size()) QString::append(Separator);
      QString::append(s);
      return(*this);
    }

};

#endif // Q_CS_STRING_H
