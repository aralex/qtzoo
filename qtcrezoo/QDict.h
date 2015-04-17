#ifndef Q_DICT_H
#define Q_DICT_H


#include <QHash>
#include <QString>

#include "dict.hpp"


#define q_DEFAULT_DELIMETER       " "


class q_Dict: public QHash<QString, QString>
{
  protected:
    QString Delimeter;

  public:
    q_Dict(const QString& delim = q_DEFAULT_DELIMETER, const QString& file_name = QString());

    void Load(const QString& file_name, const QString& delim = q_DEFAULT_DELIMETER);

    void Save(const QString& file_name) const;

    void operator +=(const t_Dict& D);
};

#endif // Q_DICT_H
