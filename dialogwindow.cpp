#include "dialogwindow.h"
#include "ui_dialogwindow.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QFileDialog>
#include <QFile>
#include <iostream>
#include <cmath>
#include <iomanip>


DialogWindow::DialogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWindow)
{
    ui->setupUi(this);
    setFixedSize(650, 650);
    // Connects
    connect(ui->btnSave, &QPushButton::clicked, this, &DialogWindow::saveMovie);
    connect(ui->btnCancel, &QPushButton::clicked, this, &DialogWindow::close);
    connect(ui->btnCancel, &QPushButton::clicked, this, &DialogWindow::setDefaultValues);
    connect(ui->btnSelectImage, &QPushButton::clicked, this, &DialogWindow::selectImage);
    // Display the slider value in a QLineEdit
    connect(ui->sliderEnjoyment, &QSlider::valueChanged, this, [this](){ onSliderValueChanged(ui->sliderEnjoyment, ui->leEnjoyment); });
    connect(ui->sliderWriting, &QSlider::valueChanged, this, [this](){ onSliderValueChanged(ui->sliderWriting, ui->leWriting); });
    connect(ui->sliderActing, &QSlider::valueChanged, this, [this](){ onSliderValueChanged(ui->sliderActing, ui->leActing); });
    connect(ui->sliderVisuals, &QSlider::valueChanged, this, [this](){ onSliderValueChanged(ui->sliderVisuals, ui->leVisuals); });
    connect(ui->sliderScore, &QSlider::valueChanged, this, [this](){ onSliderValueChanged(ui->sliderScore, ui->leScore); });
    // Rating is calculated every time a slider changes value
    connect(ui->sliderEnjoyment, &QSlider::valueChanged, this, &DialogWindow::calculateRating);
    connect(ui->sliderWriting, &QSlider::valueChanged, this, &DialogWindow::calculateRating);
    connect(ui->sliderActing, &QSlider::valueChanged, this, &DialogWindow::calculateRating);
    connect(ui->sliderVisuals, &QSlider::valueChanged, this, &DialogWindow::calculateRating);
    connect(ui->sliderScore, &QSlider::valueChanged, this, &DialogWindow::calculateRating);
    // Create an integer validator
    QIntValidator* intValidator = new QIntValidator(this);
    intValidator->setBottom(0);
    intValidator->setTop(1000);
    // Set the validators
    ui->leMovieYear->setValidator(intValidator);
    ui->leMovieLength->setValidator(intValidator);
    ui->leMovieImage->setReadOnly(true);
}


DialogWindow::~DialogWindow()
{
    delete ui;
}


void DialogWindow::saveMovie()
{
    // Read user inputs
    QString movieName = ui->leMovieName->text();
    QString movieYearString = ui->leMovieYear->text();
    int movieYear = movieYearString.toInt();
    QString movieLengthString = ui->leMovieLength->text();
    int movieLength = movieLengthString.toInt();
    QString movieGenre = ui->leMovieGenre->text();
    QString movieDirector = ui->leMovieDirector->text();
    QString movieCast = ui->leMovieCast->text();
    QString movieRating = ui->labelMovieRating->text();
    QString movieImage = ui->leMovieImage->text();
    // Check if all fields are filled
    if (movieName.isEmpty() || movieYearString.isEmpty() || movieLengthString.isEmpty() ||
        movieGenre.isEmpty() || movieDirector.isEmpty() || movieCast.isEmpty() || movieRating.isEmpty() ||
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
    // Clear the input fields after adding the movie
    setDefaultValues();
    // Close the dialog and emit the data
    emit dataSaved(movieName, movieYear, movieLength, movieGenre, movieDirector, movieCast, movieRating, destinationPath);
    accept();
}


void DialogWindow::selectImage()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Choose Cover Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!imagePath.isEmpty()) {
        QString appDir = QCoreApplication::applicationDirPath();
        QString destinationFolder = appDir + "/resources/movie_covers/";
        // Check if the destination folder exists, create if not
        QDir destinationDir(destinationFolder);
        if (!destinationDir.exists()) {
            destinationDir.mkpath(destinationFolder);
        }
        // Display the path to the image
        ui->leMovieImage->setText(imagePath);
    }
}


void DialogWindow::setDefaultValues() {
    ui->leMovieName->clear();
    ui->leMovieYear->clear();
    ui->leMovieLength->clear();
    ui->leMovieGenre->clear();
    ui->leMovieDirector->clear();
    ui->leMovieCast->clear();
    ui->leMovieImage->clear();
    ui->sliderEnjoyment->setValue(5);
    ui->sliderWriting->setValue(5);
    ui->sliderActing->setValue(5);
    ui->sliderVisuals->setValue(5);
    ui->sliderScore->setValue(5);
    ui->labelMovieRating->setText("5,0");
}


void DialogWindow::calculateRating()
{
    int enjoymentVal = ui->sliderEnjoyment->value();
    double enjoymentWeight = 1;
    int writingVal = ui->sliderWriting->value();
    double writingWeight = 0.5;
    int actingVal = ui->sliderActing->value();
    double actingWeight = 0.3;
    int visualsVal = ui->sliderVisuals->value();
    double visualsWeight = 0.2;
    int scoreVal = ui->sliderScore->value();
    double scoreWeight = 0.1;
    double sumOfValues = (enjoymentVal * enjoymentWeight) + (writingVal * writingWeight) +
                      (actingVal * actingWeight) + (visualsVal * visualsWeight) + (scoreVal * scoreWeight);

    double finalScore = (sumOfValues / 2.1);
    // Round the double to one decimal place
    double finalScoreRounded = std::round(finalScore * 10.0) / 10.0;
    std::cout << std::fixed << std::setprecision(1);
    QString formattedFinalScore = QString::number(finalScoreRounded, 'f', 1);
    formattedFinalScore.replace(".", ",");
    ui->labelMovieRating->setText(formattedFinalScore);
}


void DialogWindow::onSliderValueChanged(QSlider* slider, QLineEdit* lineEdit)
{
    int value = slider->value();
    lineEdit->setText(QString::number(value));
}
