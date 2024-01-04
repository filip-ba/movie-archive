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
    intValidator->setBottom(0);
    intValidator->setTop(1000);
    // Set the validators
    //ui->leMovieYear->setValidator(intValidator);
    //ui->leMovieLength->setValidator(intValidator);
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
    QString movieRating = ui->leMovieRating->text();
    QString movieImage = ui->leMovieImage->text();
    // Check if all fields are filled
    if (movieName.isEmpty() || movieYearString.isEmpty() || movieLengthString.isEmpty() ||
        movieDirector.isEmpty() || movieCast.isEmpty() || movieRating.isEmpty() ||
        movieImage.isEmpty()) {
        QMessageBox::warning(this, "Incomplete Fields", "Please fill in all fields before saving.");
        return;
    }
    // Get the application's directory
    QString appDir = QCoreApplication::applicationDirPath();
    // Set the destination folder path
    QString destinationFolder = appDir + "/resources/movie_covers/";
    // Get the selected cover image file name
    QFileInfo fileInfo(movieImage);
    QString fileName = fileInfo.fileName();
    // Set the destination path for the copy
    QString destinationPath = destinationFolder + fileName;
    // Check if the file already exists
    if (QFile::exists(destinationPath)) {
        QMessageBox::warning(this, "Duplicate Image", "An image with the same name already exists. Please choose a different image.");
        return;
    }
    // Copy the image to the destination folder
    if (!QFile::copy(movieImage, destinationPath)) {
        QMessageBox::critical(this, "Error", "Failed to copy the cover image.");
        return;
    }
    // Clear the search fields
    ui->leMovieName->clear();
    ui->leMovieYear->clear();
    ui->leMovieLength->clear();
    ui->leMovieDirector->clear();
    ui->leMovieCast->clear();
    ui->leMovieRating->clear();
    ui->leMovieImage->clear();
    // Close the dialog and emit the data
    emit dataSaved(movieName, movieYear, movieLength, movieDirector, movieCast, movieRating, destinationPath);
    accept();
}


void DialogWindow::on_btnSelectImage_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Choose Cover Image", "", "Images (*.png *.jpg *.jpeg)");
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
        // Display the path to the image
        ui->leMovieImage->setText(imagePath);
    }
}
