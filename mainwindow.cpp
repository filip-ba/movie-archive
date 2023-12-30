#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dialogwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogWindow = new class DialogWindow(this);
    // Connects
    connect(dialogWindow , SIGNAL(dataSaved(QString, int, int, QString, QString, int)), this, SLOT(on_data_saved(QString, int, int, QString, QString, int)));

    // Create an instance of QTableWidget
    QTableWidget* tableWidget = ui->tableWidget; // Replace with your actual QTableWidget name
    // Make the tableWidget not editable
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // Set section resize mode to stretch
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Set selection behavior to select entire rows
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAddMovie_clicked()
{
    dialogWindow->show();
}

void MainWindow::on_data_saved(QString movieName, int movieYear, int movieLength, QString movieDirector, QString movieCast, int movieRating)
{
    // Assuming you have a QTableWidget named "tableWidget"
    QTableWidgetItem *itemMovieName = new QTableWidgetItem(movieName);
    QTableWidgetItem *itemMovieYear = new QTableWidgetItem(QString::number(movieYear));
    QTableWidgetItem *itemMovieLength = new QTableWidgetItem(QString::number(movieLength));
    QTableWidgetItem *itemMovieDirector = new QTableWidgetItem(movieDirector);
    QTableWidgetItem *itemMovieCast = new QTableWidgetItem(movieCast);
    QTableWidgetItem *itemMovieRating = new QTableWidgetItem(QString::number(movieRating));
    int row = ui->tableWidget->rowCount(); // Get the current row count
    ui->tableWidget->insertRow(row); // Insert a new row
    // Set the items in the row
    ui->tableWidget->setItem(row, 0, itemMovieName);
    ui->tableWidget->setItem(row, 1, itemMovieYear);
    ui->tableWidget->setItem(row, 2, itemMovieLength);
    ui->tableWidget->setItem(row, 3, itemMovieDirector);
    ui->tableWidget->setItem(row, 4, itemMovieCast);
    ui->tableWidget->setItem(row, 5, itemMovieRating);
}


