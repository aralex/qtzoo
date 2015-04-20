#include "QTextFile.h"


QTextFile::QTextFile(const QString &file_name, QObject *parent) :
  QFile(file_name, parent),
  ts(this)
{
}


bool QTextFile::read_line(QString &str, qint64 maxlen)
{
  str = ts.readLine(maxlen);
  return(!str.isNull());
}


bool QTextFile::read_nonempty_line(QString &str, qint64 maxlen)
{
  QString s;

  while(read_line(str, maxlen))
  {
    s = str.trimmed();
    if(!s.isEmpty() && (s.at(0) != QChar('#')))
      break;
  }
  return(!str.isEmpty());
}
