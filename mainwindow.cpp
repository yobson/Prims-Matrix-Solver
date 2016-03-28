#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(30); //Set up table to have squares so it feels more like a matrix
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(30);
    largest = 1000000000; //set a number larger than any inputted data
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit->text() != "") { //check there is stuff in the number of nodes text field
        numberOfNodes = ui->lineEdit->text().toInt();  //set numberOfNodes as a global variable in this application
        ui->tableWidget->setColumnCount(numberOfNodes); //set table to correct size
        ui->tableWidget->setRowCount(numberOfNodes);
        ui->tableWidget->setEnabled(true); //enable the table and button for editing/using
        ui->pushButton_2->setEnabled(true);
        aggregateWeight = 0; //initalise the acumalitive weight variable
        for (int i = 0; i < numberOfNodes; i++) { //add dashes across the diagonal so the user can't accidently add a distance between the same node
            QTableWidgetItem *temp = new QTableWidgetItem;
            temp->setText("-");
            ui->tableWidget->setItem(i, i, temp);
        }

    }
}

void MainWindow::on_pushButton_2_clicked()
{
    disconnect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int))); //disconnect the auto update tabel fundtion
    float *currentBuffer = new float[numberOfNodes*numberOfNodes]; //this is our array. It is 2d but will represent the 3d one
    int *from = new int[numberOfNodes]; // to and from are where we store the answers
    int *to = new int[numberOfNodes];
    colConverter = new int[numberOfNodes]; //the array will not be put into the matrix in order, this will be used to convert it back to the correct order
    int iteration = 0;
    int row, column;

    this->removeRow(0, currentBuffer); //start prim's by removing the first row
    this->addToBuffer(currentBuffer, 0, 0, numberOfNodes); //adds the first column into the array. This is the array it searches. nodes get added when they are selected in prims
    colConverter[0] = 0; //column 0 = 0


    while (iteration < numberOfNodes - 1) {
        this->findSmallest(&row, &column, currentBuffer, iteration + 1); //looks for the smallest weight in currentBuffer and tells us the row and column
        to[iteration] = row; //adds to answer
        from[iteration] = column;
        this->removeRow(row, currentBuffer); //crosses out that row of the table
        iteration++;
        this->addToBuffer(currentBuffer, row, iteration, numberOfNodes); //adds that node to the active array
        colConverter[iteration] = row; //maps the nex set of weights in the array to their position on the table
    }

    QString output = "MST: {   "; //constricts the answer
    QString tempString;

    for (int i = 0; i < iteration; i++) {
        qDebug() << "From: " << from[i] + 1 << " To: " << to[i] + 1;
        output.append(tempString.sprintf("%d->%d   ", (from[i] + 1), (to[i] + 1)));
    }
    output.append(tempString.sprintf("} Total Weight = %f", aggregateWeight));
    ui->label->setText(output); //prints answer to label

    connect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int))); //reconnects the autocompleate function of the table
}

void MainWindow::on_tableWidget_cellChanged(int row, int column) //autocomplete function
{
    disconnect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int))); //disconnects the function to prevent autocompleate function loop and stack overflow
    QTableWidgetItem *NewCell = new QTableWidgetItem; //creates and empty table cell
    NewCell->setText(ui->tableWidget->item(row, column)->text()); //set the text of the empty object to the text of the newly updated cell
    ui->tableWidget->setItem(column, row, NewCell); //set the corresponding weight to the object
    connect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int))); //reconnect the autocompleate funtion
}

void MainWindow::addToBuffer(float *output, int node, int position, int size) {
    int ii = 0; //badly named iterator for the loop
    for (int i = size * position; i < (size * (position + 1)); i++ ){ //set i to fill in the 2d representation of the matrix at 0,i
        if (ui->tableWidget->item(ii, node)->text() != "-") { //don't try to convert a '-' to float
            output[i] = ui->tableWidget->item(ii, node)->text().toFloat(); //add selected cell to the array
        } else {
            output[i] = largest; //if the cell contains a dash, add our large number instead
        }
        ii++; //add one to our badly named iterator XD
    }
}

void MainWindow::findSmallest(int *row, int *column, float *array, int noOfNodes) {
    float smallest = largest; //bit oxymoronic... but it sets the current smallest value to largest so all values it compares will be smaller
    int position; //this will store the index of the smallest weight in the active array
    for (int i = 0; i < numberOfNodes*noOfNodes; i++) {
        if (array[i] < smallest && array[i] > 0) { //sees if the currect index of the array is smaller than the previous smallest value
            position = i;
            smallest = array[i];
        }
    }
    aggregateWeight += array[position]; //appends the total weight variable acordingly
    *row = (position + numberOfNodes) % numberOfNodes; //finds the row by getting the remader when dividing by the number of nodes
    *column = colConverter[(position - *row) / numberOfNodes]; //uses the converter becuase the nth column is probably not in the nth position of the active array
}

void MainWindow::removeRow(int row, float *array) {
    for (int i = 0; i < numberOfNodes; i++) {
        QTableWidgetItem *dash = new QTableWidgetItem; //make empty table cell
        dash->setText("-");
        ui->tableWidget->setItem(row, i, dash); //replace the whole row withe the dashes
        array[(i*numberOfNodes)+row] = largest; // removes the data in this row from the array
    }
}
