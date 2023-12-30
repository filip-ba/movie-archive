#include "dialogwindow.h"
#include "ui_dialogwindow.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QIntValidator>

DialogWindow::DialogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWindow)
{
    ui->setupUi(this);
    connect(ui->btnCancel, &QPushButton::clicked, this, &DialogWindow::close);
    connect(ui->btnSave, &QPushButton::clicked, this, &DialogWindow::on_btnSave_clicked);
    // Create an integer validator
    QIntValidator* intValidator = new QIntValidator(this);
    intValidator->setBottom(0);  // Set the minimum allowed value
    intValidator->setTop(1000);   // Set the maximum allowed value
    // Set the validator
    ui->leMovieYear->setValidator(intValidator);
    ui->leMovieLength->setValidator(intValidator);
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
    // Disconnect the signal-slot connection
    disconnect(ui->btnSave, &QPushButton::clicked, this, &DialogWindow::on_btnSave_clicked);
    accept();
    emit dataSaved(movieName, movieYear, movieLength, movieDirector, movieCast, movieRating);
}
