#include "qTextFile.h"


qTextFile::qTextFile(const QString &file_name, QObject *parent) :
  QFile(file_name, parent),
  ts(this)
{
  ts.setAutoDetectUnicode(true);
}


bool qTextFile::readLine(QString &str, qint64 maxlen)
{
  str = ts.readLine(maxlen);
  return(!str.isNull());
}


bool qTextFile::readNonemptyLine(QString &str, qint64 maxlen)
{
  QString s;

  while(readLine(str, maxlen))
  {
    s = str.trimmed();
    if(!s.isEmpty() && (s.at(0) != QChar('#')))
      break;
  }
  return(!str.isEmpty());
}
