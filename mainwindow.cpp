#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dialogwindow.h"
#include <QFile>
#include <QFileDialog>
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
    // Create an instance of QTableWidget
    QTableWidget* tableWidget = ui->tableWidget;
    // Set section resize mode to stretch
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Hide the column storing the image paths
    ui->tableWidget->setColumnHidden(8, true);
    // Initialize the status bar
    statusBar();
    // Load the table data
    loadDataFromFile();
    // Connects
    connect(dialogWindow, SIGNAL(dataSaved(QString,int,int,QString,QString,QString,QString,QString)), this,
            SLOT(saveMovie(QString,int,int,QString,QString,QString,QString,QString)));
    connect(ui->btnAddMovie, &QPushButton::clicked, this, &MainWindow::addMovie);
    connect(ui->btnDeleteMovie, &QPushButton::clicked, this, &MainWindow::deleteMovie);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::editMovie);
    connect(ui->btnClearSearch, &QPushButton::clicked, this, &MainWindow::clearSearch);
    // File menu connects
    connect(ui->actionBackup_Data, &QAction::triggered, this, &MainWindow::backupDataFile);
    connect(ui->actionAdd_Movie, &QAction::triggered, this, &MainWindow::addMovie);
    // Search connects
    connect(ui->leSearchTitle, &QLineEdit::textChanged, this, &MainWindow::updateTableWithSearch);
    connect(ui->leSearchYear, &QLineEdit::textChanged, this, &MainWindow::updateTableWithSearch);
    connect(ui->leSearchLength, &QLineEdit::textChanged, this, &MainWindow::updateTableWithSearch);
    connect(ui->leSearchGenre, &QLineEdit::textChanged, this, &MainWindow::updateTableWithSearch);
    connect(ui->leSearchDirector, &QLineEdit::textChanged, this, &MainWindow::updateTableWithSearch);
    connect(ui->leSearchCast, &QLineEdit::textChanged, this, &MainWindow::updateTableWithSearch);
    connect(ui->leSearchRating, &QLineEdit::textChanged, this, &MainWindow::updateTableWithSearch);
    connect(ui->comboBoxYear, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateTableWithSearch);
    connect(ui->comboBoxLength, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateTableWithSearch);
    connect(ui->comboBoxRating, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateTableWithSearch);
    // Create validators
    QIntValidator* intValidator = new QIntValidator(0, 3000);
    QDoubleValidator* doubleValidator = new QDoubleValidator(0, 10, 1);
    // Set the validators
    ui->leSearchYear->setValidator(intValidator);
    ui->leSearchLength->setValidator(intValidator);
    ui->leSearchRating->setValidator(doubleValidator);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addMovie()
{
    dialogWindow->show();
}


void MainWindow::saveMovie(QString movieName, int movieYear, int movieLength, QString movieGenre,
                             QString movieDirector, QString movieCast, QString movieRating, QString imagePath)
{
    // Get the current row count
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row); // Insert a new row
    // Set the items in the row
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(movieName));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(movieYear)));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(movieLength)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(movieGenre));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(movieDirector));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(movieCast));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(movieRating));
    // Extract the image name from the full path
    QFileInfo fileInfo(imagePath);
    QString imageName = fileInfo.fileName();
    // Call a function that displays the movie cover
    displayMovieCover(row, imageName, imagePath);
    // Update the search results
    updateTableWithSearch();
    // Display a status bar message
    QString statusMessage = "Movie '" + movieName + "' has been added.";
    statusBar()->showMessage(statusMessage, 5000);
    // Save the movie
    saveDataToFile();
}


void MainWindow::displayMovieCover(int row, QString imageName, QString imagePath)
{
    // Set the image name as text in the column 9
    QTableWidgetItem *imageItem = new QTableWidgetItem();
    imageItem->setText(imageName);
    ui->tableWidget->setItem(row, 8, imageItem);
    // Create a label for the image
    QLabel *imageLabel = new QLabel();
    if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
        // Display scaled and centered image if the image exists
        QPixmap originalPixmap(imagePath);
        QPixmap scaledPixmap = originalPixmap.scaledToHeight(100, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        ui->tableWidget->removeCellWidget(row, 7);
    } else {
        // Display a placeholder label if the image doesn't exist
        imageLabel->setText("No Image");
        imageLabel->setAlignment(Qt::AlignCenter);
    }
    // Set the widget in the cell
    ui->tableWidget->setCellWidget(row, 7, imageLabel);
    ui->tableWidget->setRowHeight(row, 100);
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
        for (int col = 0; col < rowData.size() - 1; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(rowData.value(col));
            ui->tableWidget->setItem(row, col, item);
        }
        // Construct the image path from the image name
        QString imageName = rowData.value(7);
        QString imagePath = QCoreApplication::applicationDirPath() + "/resources/movie_covers/" + imageName;
        // Call a function that displays the movie cover
        displayMovieCover(row, imageName, imagePath);
    }
    file.close();
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
    // Open the file for writing
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream stream(&file);
        // Write table data
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QStringList rowData;
            for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
                // Skip writing data for the hidden column
                if (!ui->tableWidget->isColumnHidden(col)) {
                    // If it's the image cover column, get the image file name from the hidden column
                    if (col == 7) {
                        QTableWidgetItem *item = ui->tableWidget->item(row, 8);
                        if (item) {
                            rowData << item->text();
                        } else {
                            rowData << "No Data";
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


void MainWindow::deleteMovie()
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
    // Get the movie cover file name from the hidden column
    QTableWidgetItem *item = ui->tableWidget->item(selectedRow, 8);
    QString imageName;
    if (item) {
        imageName = item->text();
        // Set the path for the movie cover file
        QString coverPath = QCoreApplication::applicationDirPath() + "/resources/movie_covers/" + imageName;
        // Remove the movie cover file
        QFile::remove(coverPath);
    }
    // Read existing data from the file
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/resources/movie_data/movie_data.txt";
    QFile file(filePath);
    QStringList lines;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            lines << line;
        }
        file.close();
    }
    // Remove the corresponding row from the data
    if (selectedRow < lines.size()) {
        lines.removeAt(selectedRow);
    }
    // Save the modified data back to the file
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString &line : lines) {
            out << line << "\n";
        }
        file.close();
    }
    // Remove the selected row from the table
    ui->tableWidget->removeRow(selectedRow);
    // Display a status bar message
    QString statusMessage = "Movie '" + movieName + "' has been deleted.";
    statusBar()->showMessage(statusMessage, 5000);
}


void MainWindow::updateTableWithSearch()
{
    // Clear the current selection
    ui->tableWidget->clearSelection();
    // Get search criteria
    QString title = ui->leSearchTitle->text().trimmed().toLower();
    QString year = ui->leSearchYear->text().trimmed().toLower();
    QString length = ui->leSearchLength->text().trimmed().toLower();
    QString genre = ui->leSearchGenre->text().trimmed().toLower();
    QString director = ui->leSearchDirector->text().trimmed().toLower();
    QString cast = ui->leSearchCast->text().trimmed().toLower();
    QString rating = ui->leSearchRating->text().trimmed().toLower();
    // Iterate through the rows and apply the search criteria
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        bool match = true;
        // Check title
        if (!title.isEmpty()) {
            QString cellText = ui->tableWidget->item(row, 0)->text().toLower();
            match = match && cellText.contains(title);
        }
        // Check year
        if (!year.isEmpty() && ui->comboBoxYear->currentIndex() != -1) {
            QString cellText = ui->tableWidget->item(row, 1)->text();
            int movieYear = cellText.toInt();
            int searchYear = ui->leSearchYear->text().toInt();
            // Compare based on the selected option in the comboBoxYear
            switch (ui->comboBoxYear->currentIndex()) {
            case 0: // Higher Than
                match = match && (movieYear > searchYear);
                break;
            case 1: // Equal To
                match = match && (movieYear == searchYear);
                break;
            case 2: // Lower Than
                match = match && (movieYear < searchYear);
                break;
            }
        }
        // Check length
        if (!length.isEmpty() && ui->comboBoxLength->currentIndex() != -1) {
            QString cellText = ui->tableWidget->item(row, 2)->text();
            int movieLength = cellText.toInt();
            int searchLength = ui->leSearchLength->text().toInt();
            switch (ui->comboBoxLength->currentIndex()) {
            case 0:
                match = match && (movieLength > searchLength);
                break;
            case 1:
                match = match && (movieLength == searchLength);
                break;
            case 2:
                match = match && (movieLength < searchLength);
                break;
            }
        }
        // Check genre
        if (!genre.isEmpty()) {
            QString cellText = ui->tableWidget->item(row, 3)->text().toLower();
            QStringList genres = cellText.split(",", Qt::SkipEmptyParts);
            bool genresMatch = false;
            for (const QString& gen : genres) {
                if (gen.trimmed().contains(genre)) {
                    genresMatch = true;
                    break;
                }
            }
            match = match && genresMatch;
        }
        // Check director
        if (!director.isEmpty()) {
            QString cellText = ui->tableWidget->item(row, 4)->text().toLower();
            QStringList directors = cellText.split(",", Qt::SkipEmptyParts);
            bool directorMatch = false;
            for (const QString& dir : directors) {
                if (dir.trimmed().contains(director)) {
                    directorMatch = true;
                    break;
                }
            }
            match = match && directorMatch;
        }
        // Check cast
        if (!cast.isEmpty()) {
            QString cellText = ui->tableWidget->item(row, 5)->text().toLower();
            QStringList actors = cellText.split(",", Qt::SkipEmptyParts);
            bool castMatch = false;
            for (const QString& actor : actors) {
                if (actor.trimmed().contains(cast)) {
                    castMatch = true;
                    break;
                }
            }
            match = match && castMatch;
        }
        // Check rating
        if (!rating.isEmpty() && ui->comboBoxRating->currentIndex() != -1) {
            QString cellText = ui->tableWidget->item(row, 6)->text();
            cellText.replace(',', '.');
            double movieRating = cellText.toDouble();
            QString searchText = ui->leSearchRating->text();
            searchText.replace(',', '.');
            double searchRating = searchText.toDouble();
            // Compare based on the selected option in the comboBoxYear
            switch (ui->comboBoxRating->currentIndex()) {
            case 0:
                match = match && (movieRating > searchRating);
                break;
            case 1:
                match = match && (movieRating == searchRating);
                break;
            case 2:
                match = match && (movieRating < searchRating);
                break;
            }
        }
        // Check if any search criteria are provided
        bool hasSearchCriteria = !title.isEmpty() || !year.isEmpty() || !length.isEmpty() ||
                                 !genre.isEmpty() || !director.isEmpty() || !cast.isEmpty() || !rating.isEmpty();
        QString statusBarText = "Search Criteria: ";
        // Update status bar text based on the search criteria
        if (hasSearchCriteria) {
            if (!title.isEmpty()) statusBarText += "Title: '" + title + "' | ";
            if (!year.isEmpty()) statusBarText += "Year: '" + year + "' | ";
            if (!length.isEmpty()) statusBarText += "Length: '" + length + "' | ";
            if (!genre.isEmpty()) statusBarText += "Genre: '" + genre + "' | ";
            if (!director.isEmpty()) statusBarText += "Director: '" + director + "' | ";
            if (!cast.isEmpty()) statusBarText += "Cast: '" + cast + "' | ";
            if (!rating.isEmpty()) statusBarText += "Rating: '" + rating + "' | ";
            statusBar()->showMessage(statusBarText);
        } else {
            statusBar()->clearMessage();
        }
        // Show or hide the row based on the match
        ui->tableWidget->setRowHidden(row, !match);
    }
}


void MainWindow::clearSearch()
{
    // Clear the search fields
    ui->leSearchTitle->clear();
    ui->leSearchYear->clear();
    ui->leSearchLength->clear();
    ui->leSearchGenre->clear();
    ui->leSearchDirector->clear();
    ui->leSearchCast->clear();
    ui->leSearchRating->clear();
    // Display the entire list
    displayEntireList();
}


void MainWindow::displayEntireList()
{
    // Iterate through each row in the table
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        // Make sure the row is visible
        ui->tableWidget->setRowHidden(row, false);
    }
}


void MainWindow::editMovie(QTableWidgetItem *item)
{
    if (!item) {
        return;
    }
    // Get the row and column of the changed item
    int row = item->row();
    int col = item->column();
    // Get the updated text
    QString updatedText = item->text();
    // Update the data in the movie_data.txt file
    updateDataInFile(row, col, updatedText);
}


void MainWindow::updateDataInFile(int row, int col, const QString &updatedText)
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/resources/movie_data/movie_data.txt";
    QFile file(filePath);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream stream(&file);
        // Read and update data in the file
        QStringList updatedData;
        int currentRow = 0;
        while (!stream.atEnd()) {
            QString dataLine = stream.readLine();
            QStringList rowData = dataLine.split('\t', Qt::SkipEmptyParts);
            // Update the changed item in the corresponding row and column
            if (currentRow == row && col < rowData.size()) {
                rowData[col] = updatedText.isEmpty() ? "No Data" : updatedText;
            }
            updatedData << rowData.join("\t");
            ++currentRow;
        }
        // Clear the existing data in the file
        file.resize(0);
        // Write the updated data to the file
        stream << updatedData.join("\n");
        file.close();
    }
    QString statusMessage = "The record has been edited.";
    statusBar()->showMessage(statusMessage, 5000);
}


void MainWindow::backupDataFile()
{
    QString sourceFilePath = QCoreApplication::applicationDirPath() + "/resources/movie_data/movie_data.txt";
    QString destinationFilePath = QFileDialog::getSaveFileName(this, "Choose Backup Destination", "", "Text Files (*.txt)");
    if (destinationFilePath.isEmpty()) {
        return;
    }
    // Copy the contents of the source file to the destination file
    QFile sourceFile(sourceFilePath);
    QFile destinationFile(destinationFilePath);
    if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) && destinationFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream in(&sourceFile);
        QTextStream out(&destinationFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            out << line << "\n";
        }
        sourceFile.close();
        destinationFile.close();
        statusBar()->showMessage("Backup completed.", 5000);
    } else {
        statusBar()->showMessage("Error: Backup failed.", 5000);
    }
}


