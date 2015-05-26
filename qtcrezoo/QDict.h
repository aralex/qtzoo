#ifndef Q_DICT_H
#define Q_DICT_H


#include <QHash>
#include <QString>


#define q_DEFAULT_DELIMETER       " "


class qDict: public QHash<QString, QString>
{
  protected:
    QString Delimeter;

  public:
    qDict(const QString& delim = q_DEFAULT_DELIMETER, const QString& file_name = QString());

    void load(const QString& file_name, const QString& delim = q_DEFAULT_DELIMETER);

    void save(const QString& file_name) const;
};

#endif // Q_DICT_H
