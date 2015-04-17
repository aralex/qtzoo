#ifndef Q_TEXT_FILE_H
#define Q_TEXT_FILE_H


#include <QFile>
#include <QTextStream>


class q_Text_File : public QFile
{
    Q_OBJECT

  protected:

    QTextStream ts;

  public:
    enum t_Flags
    {
      Trim_Left,
      Trim_Right
    };

    explicit q_Text_File(const QString &file_name, QObject *parent = 0);

    bool read_line(QString& str, qint64 maxlen = 0);

    bool read_nonempty_line(QString& str, qint64 maxlen = 0);

  signals:

  public slots:

};

#endif // Q_TEXT_FILE_H
