#include <QDir>

#include "qFileName.h"


static const QString c_Qt_Separator("/");


qFileName::qFileName()
{
}


qFileName::qFileName(const QString &file_name): QString(file_name)
{
}


qFileName::qFileName(const QString &file_path, const QString &file_name):
  QString(file_path)
{
  append(c_Qt_Separator);
  append(file_name);
}


void qFileName::set(const QString &file_path, const QString &file_name)
{
  clear();
  append(file_path);
  append(c_Qt_Separator);
  append(file_name);
}


const qFileName qFileName::operator <<(const QString &file_name) const
{
  qFileName fn(*this, file_name);
  return(fn);
}


const qFileName &qFileName::operator <<=(const QString &file_name)
{
  append(c_Qt_Separator);
  append(file_name);
  return(*this);
}


qFileName qFileName::baseName() const
{
  return(QFileInfo(*this).baseName());
}


qFileName qFileName::fileName() const
{
  return(QFileInfo(*this).fileName());
}
