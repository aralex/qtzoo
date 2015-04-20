#ifndef TESTCHBFILTMAINWINDOW_H
#define TESTCHBFILTMAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "QCheckboxFilterModel.h"

#include "model_tools.h"


namespace Ui {
  class TestChBFiltMainWindow;
}

class TestChBFiltMainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit TestChBFiltMainWindow(QWidget *parent = 0);
    ~TestChBFiltMainWindow();

  private slots:
    void on_btnLoad_clicked();

    void on_chb_1_toggled(bool checked);

    void on_chb_2_toggled(bool checked);

    void on_lePattern_1_textChanged(const QString &arg1);

    void on_lePattern_2_textChanged(const QString &arg1);

    void on_mdl_1_dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int>());

    void on_mdl_2_dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int>());

  private:
    Ui::TestChBFiltMainWindow *ui;

    QStandardItemModel* mdl_orig;
    QCheckboxFilterModel* mdl_1;
    QCheckboxFilterModel* mdl_2;

};

#endif // TESTCHBFILTMAINWINDOW_H
