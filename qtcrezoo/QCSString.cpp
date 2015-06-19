#include "qCSString.h"


qCSString::qCSString():
  Separator(CS_DEFAULT_SEPARATOR)
{
}


qCSString::qCSString(const char *s, const QString &separator):
  QString(s), Separator(separator)
{
}


qCSString::qCSString(const QString &s, const QString &separator):
  QString(s), Separator(separator)
{
}


const qCSString &qCSString::append(const QString &s)
{
  if(size()) QString::append(Separator);
  QString::append(s);
  return(*this);
}
