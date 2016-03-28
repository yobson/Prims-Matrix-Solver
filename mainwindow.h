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

private:
    Ui::MainWindow *ui;
    void addToBuffer(int *output, int node, int position, int size);
    void findSmallest(int *row, int *column, int *array, int noOfNodes);
    void removeRow(int row, int *array);
    int numberOfNodes;
    int *colConverter;
};

#endif // MAINWINDOW_H
