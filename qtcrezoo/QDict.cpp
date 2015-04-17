#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

#include "q_dict.h"
#include "q_text_file.h"


q_Dict::q_Dict(const QString &delim, const QString &file_name):
  Delimeter(delim)
{
  if(!file_name.isNull())
    Load(file_name);
}


void q_Dict::Load(const QString &file_name, const QString& delim)
{
  if((Delimeter != delim) && (delim != q_DEFAULT_DELIMETER))
    Delimeter = delim;

  q_Text_File f(file_name);
  QString s;
  QStringList pairs;

  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    throw QString("can not open file '%0'").arg(file_name);

  while(f.read_nonempty_line(s))
  {
    pairs = s.trimmed().split(QRegExp(Delimeter));
    if(pairs.size() != 2)
      throw QString("invalid line in dictionary file: '%0'").arg(s);

    insert(pairs[0], pairs[1]);
  }
}


void q_Dict::Save(const QString &file_name) const
{
  QFile f(file_name);

  if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    throw QString("can not open file '%0'").arg(file_name);

  QTextStream out(&f);

  for(const_iterator i = begin(); i != end(); ++i)
    out << i.key() << " = " << i.value() << "\n";

  f.close();
}


void q_Dict::operator +=(const t_Dict &D)
{
  for(t_Dict::const_iterator i = D.begin(); i != D.end(); ++i)
  {
    (*this)[QString::fromStdWString((*i).first)] = QString::fromStdWString((*i).second);
  }
}
