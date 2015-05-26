#ifndef TESTCHBFILTMAINWINDOW_H
#define TESTCHBFILTMAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "qCheckboxFilterModel.h"
#include "qSpyModel.h"

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

    void on_chbProdKinds_stateChanged(int ch_state);

    void on_chbProducts_stateChanged(int ch_state);

    void on_mdlProdKinds_dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int>());

    void on_lePatternDiets_textChanged(const QString &arg1);

    void on_lePatternProdKinds_textChanged(const QString &arg1);

    void on_lePatternProducts_textChanged(const QString &arg1);

    void on_lvProdKinds_selection_changed(const QItemSelection &selected, const QItemSelection &deselected);

    void on_mdlDiets_selection_changed(const QItemSelection &selected, const QItemSelection &deselected);

    void on_lvDiets_doubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_chbProdKinds_pressed();

    void on_chbProducts_pressed();

  private:
    Ui::TestChBFiltMainWindow *ui;

    QStandardItemModel* mdlDietsFull;
    QStandardItemModel* mdlProdKindsFull;
    QStandardItemModel* mdlProductsFull;

    qCheckboxFilterModel* mdlDiets;
    qCheckboxFilterModel* mdlProdKinds;
    qCheckboxFilterModel* mdlProducts;

    qSpyModel* mdlSpy1;
    qSpyModel* mdlSpy2;

    bool buttonCancelClicked;
    bool FreezeRefs;

    QString SelectedProdKinds;
    QString SelectedProducts;

    bool ChBProdKindsToggledManually;
    bool ChBProductsToggledManually;

};

#endif // TESTCHBFILTMAINWINDOW_H
