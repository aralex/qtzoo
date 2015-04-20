#ifndef Q_DICT_H
#define Q_DICT_H


#include <QHash>
#include <QString>


#define q_DEFAULT_DELIMETER       " "


class QDict: public QHash<QString, QString>
{
  protected:
    QString Delimeter;

  public:
    QDict(const QString& delim = q_DEFAULT_DELIMETER, const QString& file_name = QString());

    void Load(const QString& file_name, const QString& delim = q_DEFAULT_DELIMETER);

    void Save(const QString& file_name) const;
};

#endif // Q_DICT_H
