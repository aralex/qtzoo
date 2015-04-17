#include "q_text_file.h"



q_Text_File::q_Text_File(const QString &file_name, QObject *parent) :
  QFile(file_name, parent),
  ts(this)
{
}


bool q_Text_File::read_line(QString &str, qint64 maxlen)
{
  str = ts.readLine(maxlen);
  return(!str.isNull());
}


bool q_Text_File::read_nonempty_line(QString &str, qint64 maxlen)
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
