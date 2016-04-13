#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    void addToBuffer(float *output, int node, int position, int size);
    void findSmallest(int *row, int *column, float *array, int noOfNodes);
    void removeRow(int row, float *array);
    int numberOfNodes;
    int *colConverter;
    float aggregateWeight;
    float largest;
};

#endif // MAINWINDOW_H
