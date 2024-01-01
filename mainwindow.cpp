#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dialogwindow.h"
#include <QFile>
#include <QLabel>
#include <QCloseEvent>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogWindow = new class DialogWindow(this);
    // Initialize the status bar
    statusBar();
    // Load the table data
    loadDataFromFile();
    // Connects
    connect(dialogWindow , SIGNAL(dataSaved(QString, int, int, QString, QString, int, QString)), this, SLOT(on_data_saved(QString, int, int, QString, QString, int, QString)));
    // Create an instance of QTableWidget
    QTableWidget* tableWidget = ui->tableWidget;
    // Set section resize mode to stretch
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Hide the column storing the image paths
    ui->tableWidget->setColumnHidden(7, true);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAddMovie_clicked()
{
    dialogWindow->show();
}

void MainWindow::on_data_saved(QString movieName, int movieYear, int movieLength, QString movieDirector, QString movieCast, int movieRating, QString imagePath)
{
    int row = ui->tableWidget->rowCount(); // Get the current row count
    ui->tableWidget->insertRow(row); // Insert a new row
    // Set the items in the row
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(movieName));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(movieYear)));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(movieLength)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(movieDirector));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(movieCast));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(movieRating)));
    // Extract the image name from the full path
    QFileInfo fileInfo(imagePath);
    QString imageName = fileInfo.fileName();
    // Set the image name as text in the column 7
    QTableWidgetItem *imageItem = new QTableWidgetItem();
    imageItem->setText(imageName);
    ui->tableWidget->setItem(row, 7, imageItem);
    // Display scaled and centered image in the 6th column
    QLabel *imageLabel = new QLabel();
    QPixmap originalPixmap(imagePath);
    QPixmap scaledPixmap = originalPixmap.scaledToHeight(100, Qt::SmoothTransformation);
    imageLabel->setPixmap(scaledPixmap);
    imageLabel->setAlignment(Qt::AlignCenter);
    // Set the widget in the cell
    ui->tableWidget->setCellWidget(row, 6, imageLabel);
    // Adjust row height to match the scaled image height
    ui->tableWidget->setRowHeight(row, scaledPixmap.height());
    // Display a status bar message
    QString statusMessage = "Movie '" + movieName + "' has been added.";
    statusBar()->showMessage(statusMessage, 7000); // Show status bar message
}


void MainWindow::saveDataToFile()
{
    // Get the application's directory
    QString appDir = QCoreApplication::applicationDirPath();
    // Set the folder path for saving data
    QString folderPath = appDir + "/resources/movie_data";
    // Create the folder if it doesn't exist
    QDir folderDir(folderPath);
    if (!folderDir.exists()) {
        folderDir.mkpath(folderPath);
    }
    // Set the file path for saving data
    QString filePath = folderPath + "/movie_data.txt";
    // Open the file for writing (with Truncate mode)
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream stream(&file);
        // Write table data
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QStringList rowData;
            for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
                // Skip writing data for the hidden column
                if (!ui->tableWidget->isColumnHidden(col)) {
                    // If it's the image cover column(6th column), get the image file name from the hidden 7th column
                    if (col == 6) {
                        QTableWidgetItem *item = ui->tableWidget->item(row, 7);
                        if (item) {
                            rowData << item->text();
                        } else {
                            rowData << ""; // Handle the case where the item is null
                        }
                    } else {
                        QTableWidgetItem *item = ui->tableWidget->item(row, col);
                        if (item) {
                            rowData << item->text();
                        } else {
                            QWidget *widget = ui->tableWidget->cellWidget(row, col);
                            if (widget) {
                                QLabel *label = qobject_cast<QLabel *>(widget);
                                if (label) {
                                    rowData << label->text();
                                }
                            }
                        }
                    }
                }
            }
            stream << rowData.join("\t") << "\n";
        }
        file.close();
    }
}


void MainWindow::loadDataFromFile()
{
    QString movieDataFile = QCoreApplication::applicationDirPath() + "/resources/movie_data/movie_data.txt";
    QFile file(movieDataFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open the data file.");
        return;
    }
    QTextStream stream(&file);
    // Clear existing data in the table
    ui->tableWidget->setRowCount(0);
    // Read and populate the table data
    while (!stream.atEnd()) {
        QString dataLine = stream.readLine();
        QStringList rowData = dataLine.split('\t', Qt::SkipEmptyParts);
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        // Fills in the first 6 fields
        for (int col = 0; col < rowData.size()-1; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(rowData.value(col));
            ui->tableWidget->setItem(row, col, item);
        }
        // Construct the image path from the image name
        QString imageName = rowData.value(6);
        QString imagePath = QCoreApplication::applicationDirPath() + "/resources/movie_covers/" + imageName;
        // Set the image name as text in the column 7
        QTableWidgetItem *imageItem = new QTableWidgetItem();
        imageItem->setText(imageName);
        ui->tableWidget->setItem(row, 7, imageItem);
        // Display scaled and centered image in the 6th column
        QLabel *imageLabel = new QLabel();
        QPixmap originalPixmap(imagePath);
        QPixmap scaledPixmap = originalPixmap.scaledToHeight(100, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        ui->tableWidget->removeCellWidget(row, 6); // Clear existing content
        // Set the widget in the cell
        ui->tableWidget->setCellWidget(row, 6, imageLabel);
        // Adjust row height to match the scaled image height
        ui->tableWidget->setRowHeight(row, scaledPixmap.height());
    }
    file.close();
}


void MainWindow::on_btnDeleteMovie_clicked()
{
    // Get the selected row
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::information(this, "No Selection", "Please select a movie to delete.");
        return;
    }
    QString movieName = ui->tableWidget->item(selectedRow, 0)->text();
    // Confirm deletion with the user
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete the selected movie?",
                                         QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::No) {
        return;
    }
    // Remove the selected row
    ui->tableWidget->removeRow(selectedRow);
    // Display a status bar message
    QString statusMessage = "Movie '" + movieName + "' has been deleted.";
    statusBar()->showMessage(statusMessage, 7000); // Show status bar message
}




