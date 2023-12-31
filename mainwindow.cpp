#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dialogwindow.h"
#include <iostream>
#include <QFile>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogWindow = new class DialogWindow(this);
    // Connects
    connect(dialogWindow , SIGNAL(dataSaved(QString, int, int, QString, QString, int, QString)), this, SLOT(on_data_saved(QString, int, int, QString, QString, int, QString)));

    // Create an instance of QTableWidget
    QTableWidget* tableWidget = ui->tableWidget;
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

void MainWindow::on_data_saved(QString movieName, int movieYear, int movieLength, QString movieDirector, QString movieCast, int movieRating, QString movieImage)
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
    // Display the cover image
    // Display scaled and centered image in the new column (assuming column index 6 for the image)
    QLabel *imageLabel = new QLabel();
    QPixmap originalPixmap(movieImage);
    QPixmap scaledPixmap = originalPixmap.scaledToHeight(100, Qt::SmoothTransformation);
    imageLabel->setPixmap(scaledPixmap);
    imageLabel->setAlignment(Qt::AlignCenter);
    ui->tableWidget->setCellWidget(row, 6, imageLabel);
    ui->tableWidget->setRowHeight(row, scaledPixmap.height());
}


