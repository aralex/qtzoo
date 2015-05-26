#ifndef Q_TEXT_FILE_H
#define Q_TEXT_FILE_H


#include <QFile>
#include <QTextStream>


class qTextFile : public QFile
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

    explicit qTextFile(const QString &file_name, QObject *parent = 0);

    bool readLine(QString& str, qint64 maxlen = 0);

    bool readNonemptyLine(QString& str, qint64 maxlen = 0);

    void setCodec(const char * codecName) { ts.setCodec(codecName); }

  signals:

  public slots:

};

#endif // Q_TEXT_FILE_H
