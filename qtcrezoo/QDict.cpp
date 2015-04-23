#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

#include "QDict.h"
#include "QTextFile.h"


QDict::QDict(const QString &delim, const QString &file_name):
  Delimeter(delim)
{
  if(!file_name.isNull())
    Load(file_name);
}


void QDict::Load(const QString &file_name, const QString& delim)
{
  if((Delimeter != delim) && (delim != q_DEFAULT_DELIMETER))
    Delimeter = delim;

  QTextFile f(file_name);
  QString s;
  QStringList pairs;

  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    throw QString("can not open file '%0'").arg(file_name);

  while(f.readNonemptyLine(s))
  {
    pairs = s.trimmed().split(QRegExp(Delimeter));
    if(pairs.size() != 2)
      throw QString("invalid line in dictionary file: '%0'").arg(s);

    insert(pairs[0], pairs[1]);
  }
}


void QDict::Save(const QString &file_name) const
{
  QFile f(file_name);

  if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    throw QString("can not open file '%0'").arg(file_name);

  QTextStream out(&f);

  for(const_iterator i = begin(); i != end(); ++i)
    out << i.key() << " = " << i.value() << "\n";

  f.close();
}
