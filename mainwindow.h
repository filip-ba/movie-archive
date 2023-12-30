#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddMovie_clicked(); //Disablnul sem diagnostic nevim jeslti jenom tu nebo celkove

public slots:
    void on_data_saved(QString movieName, int movieYear, int movieLength, QString movieDirector, QString movieCast, int movieRating);

private:
    Ui::MainWindow *ui;
    QDialog *dialogWindow;
};
#endif // MAINWINDOW_H
