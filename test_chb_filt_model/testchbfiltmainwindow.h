#ifndef TESTCHBFILTMAINWINDOW_H
#define TESTCHBFILTMAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "QCheckboxFilterModel.h"
#include "QSpyModel.h"

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

    void on_chbProdKinds_toggled(bool checked);

    void on_chbProducts_toggled(bool checked);

    void on_mdlProdKinds_dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int>());

    void on_lePatternDiets_textChanged(const QString &arg1);

    void on_lePatternProdKinds_textChanged(const QString &arg1);

    void on_lePatternProducts_textChanged(const QString &arg1);

    void on_lvProdKinds_selection_changed(const QItemSelection &selected, const QItemSelection &deselected);

    void on_mdlDiets_selection_changed(const QItemSelection &selected, const QItemSelection &deselected);

    void on_lvDiets_doubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

  private:
    Ui::TestChBFiltMainWindow *ui;

    QStandardItemModel* mdlDietsFull;
    QStandardItemModel* mdlProdKindsFull;
    QStandardItemModel* mdlProductsFull;

    QCheckboxFilterModel* mdlDiets;
    QCheckboxFilterModel* mdlProdKinds;
    QCheckboxFilterModel* mdlProducts;

    QSpyModel* mdlSpy1;
    QSpyModel* mdlSpy2;

    bool buttonCancelClicked;

    QString SelectedProdKinds;
    QString SelectedProducts;

};

#endif // TESTCHBFILTMAINWINDOW_H
