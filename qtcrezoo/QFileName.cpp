#include <QDir>

#include "QFileName.h"


static const QString c_Qt_Separator("/");


QFileName::QFileName()
{
}


QFileName::QFileName(const QString &file_name): QString(file_name)
{
}


QFileName::QFileName(const QString &file_path, const QString &file_name):
  QString(file_path)
{
  append(c_Qt_Separator);
  append(file_name);
}


void QFileName::Set(const QString &file_path, const QString &file_name)
{
  clear();
  append(file_path);
  append(c_Qt_Separator);
  append(file_name);
}


const QFileName QFileName::operator <<(const QString &file_name) const
{
  QFileName fn(*this, file_name);
  return(fn);
}


const QFileName &QFileName::operator <<=(const QString &file_name)
{
  append(c_Qt_Separator);
  append(file_name);
  return(*this);
}


const BASE_CHAR_TYPE *QFileName::c_str() const
{
  const BASE_CHAR_TYPE * p =
#ifdef UNICODE
    toStdWString().c_str();
#else
    toStdString().c_str();
#endif // !UNICODE
  return(p);
}


QFileName QFileName::baseName() const
{
  return(QFileInfo(*this).baseName());
}


QFileName QFileName::fileName() const
{
  return(QFileInfo(*this).fileName());
}
