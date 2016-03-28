#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(30);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit->text() != "") {
        numberOfNodes = ui->lineEdit->text().toInt();
        ui->tableWidget->setColumnCount(numberOfNodes);
        ui->tableWidget->setRowCount(numberOfNodes);
        ui->tableWidget->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        for (int i = 0; i < numberOfNodes; i++) {
            QTableWidgetItem *temp = new QTableWidgetItem;
            temp->setText("-");
            ui->tableWidget->setItem(i, i, temp);
        }

    }
}

void MainWindow::on_pushButton_2_clicked()
{
    disconnect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int)));
    int *currentBuffer = new int[numberOfNodes*numberOfNodes];
    int *from = new int[numberOfNodes];
    int *to = new int[numberOfNodes];
    colConverter = new int[numberOfNodes];
    int iteration = 0;
    int row, column;

    this->removeRow(0, currentBuffer);
    this->addToBuffer(currentBuffer, 0, 0, numberOfNodes);
    colConverter[0] = 0;


    while (iteration < numberOfNodes - 1) {
        this->findSmallest(&row, &column, currentBuffer, iteration + 1);
        to[iteration] = row;
        from[iteration] = column;
        this->removeRow(row, currentBuffer);
        iteration++;
        this->addToBuffer(currentBuffer, row, iteration, numberOfNodes);
        colConverter[iteration] = row;
    }

    QString output = "MST: {   ";
    QString tempString;

    for (int i = 0; i < iteration; i++) {
        qDebug() << "From: " << from[i] + 1 << " To: " << to[i] + 1;
        output.append(tempString.sprintf("%d->%d   ", (from[i] + 1), (to[i] + 1)));
    }
    output.append("}");
    ui->label->setText(output);

    connect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int)));
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    disconnect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int)));
    QTableWidgetItem *NewCell = new QTableWidgetItem;
    NewCell->setText(ui->tableWidget->item(row, column)->text());
    ui->tableWidget->setItem(column, row, NewCell);
    connect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int)));
}

void MainWindow::addToBuffer(int *output, int node, int position, int size) {
    int ii = 0;
    for (int i = size * position; i < (size * (position + 1)); i++ ){
        if (ui->tableWidget->item(ii, node)->text() != "-") {
            output[i] = ui->tableWidget->item(ii, node)->text().toInt();
        } else {
            output[i] = 1000;
        }
        ii++;
    }
}

void MainWindow::findSmallest(int *row, int *column, int *array, int noOfNodes) {
    int smallest = 1000;
    int i;
    int position;
    for (i = 0; i < numberOfNodes*noOfNodes; i++) {
        if (array[i] < smallest && array[i] > 0) {
            position = i;
            smallest = array[i];
        }
    }
    array[position] = 1000;
    *row = (position + numberOfNodes) % numberOfNodes;
    *column = colConverter[(position - *row) / numberOfNodes];
}

void MainWindow::removeRow(int row, int *array) {
    for (int i = 0; i < numberOfNodes; i++) {
        QTableWidgetItem *dash = new QTableWidgetItem;
        dash->setText("-");
        ui->tableWidget->setItem(row, i, dash);
        array[(i*numberOfNodes)+row] = 1000;
    }
}
