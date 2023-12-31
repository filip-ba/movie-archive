#include "dialogwindow.h"
#include "ui_dialogwindow.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QFileDialog>
#include <QFile>

DialogWindow::DialogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWindow)
{
    ui->setupUi(this);
    connect(ui->btnCancel, &QPushButton::clicked, this, &DialogWindow::close);
    // Create an integer validator
    QIntValidator* intValidator = new QIntValidator(this);
    intValidator->setBottom(0);  // Set the minimum allowed value
    intValidator->setTop(1000);   // Set the maximum allowed value
    // Set the validators
    //ui->leMovieYear->setValidator(intValidator);
    //ui->leMovieLength->setValidator(intValidator);
    //ui->leMovieRating->setValidator(intValidator);
    ui->leMovieImage->setReadOnly(true);
}

DialogWindow::~DialogWindow()
{
    delete ui;
}

void DialogWindow::on_btnSave_clicked()
{
    // Read user inputs
    QString movieName = ui->leMovieName->text();
    QString movieYearString = ui->leMovieYear->text();
    int movieYear = movieYearString.toInt();
    QString movieLengthString = ui->leMovieLength->text();
    int movieLength = movieLengthString.toInt();
    QString movieDirector = ui->leMovieDirector->text();
    QString movieCast = ui->leMovieCast->text();
    QString movieRatingString = ui->leMovieRating->text();
    int movieRating = movieRatingString.toInt();
    QString movieImage = ui->leMovieImage->text();
    // Check if all fields are filled
    if (movieName.isEmpty()||movieYearString.isEmpty()||movieLengthString.isEmpty()||movieDirector.isEmpty()||movieCast.isEmpty()||movieRatingString.isEmpty()||movieImage.isEmpty()) {
        QMessageBox::warning(this, "Incomplete Fields", "Please fill in all fields before saving.");
        return;
    }
    // Disconnect the signal-slot connection
    disconnect(ui->btnSave, &QPushButton::clicked, this, &DialogWindow::on_btnSave_clicked);
    // Close the dialog and emit the data
    accept();
    emit dataSaved(movieName, movieYear, movieLength, movieDirector, movieCast, movieRating, movieImage);
}


void DialogWindow::on_btnSelectImage_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Choose Cover Image", "", "Images (*.png *.jpg *.jpeg)");
    QString exePath = QCoreApplication::applicationFilePath();
    QString appDirPath = QFileInfo(exePath).path();
    if (!imagePath.isEmpty()) {
        // Get the application's directory
        QString appDir = QCoreApplication::applicationDirPath();
        // Set the destination folder path
        QString destinationFolder = appDir + "/resources/movie_covers/";
        // Check if the destination folder exists, create if not
        QDir destinationDir(destinationFolder);
        if (!destinationDir.exists()) {
            destinationDir.mkpath(destinationFolder);
        }
        // Get the selected cover image file name
        QFileInfo fileInfo(imagePath);
        QString fileName = fileInfo.fileName();
        // Set the destination path for the copy
        QString destinationPath = destinationFolder + fileName;
        // Check if the file already exists
        if (QFile::exists(destinationPath)) {
            QMessageBox::warning(this, "Duplicate Image", "An image with the same name already exists. Please choose a different image.");
            return;
        }
        // Copy the image to the destination folder
        if (!QFile::copy(imagePath, destinationPath)) {
            QMessageBox::critical(this, "Error", "Failed to copy the cover image.");
            return;
        }
        // Display the path
        ui->leMovieImage->setText(destinationPath);
    }
}
