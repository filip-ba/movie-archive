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
    void saveDataToFile();
    void loadDataFromFile();
    void displayMovieCover(int row, QString imageName, QString imagePath);
    void updateTableWithSearch();
    void displayEntireList();

private slots:
    void addMovie();
    void deleteMovie();
    void clearSearch();

public slots:
    void saveMovie(QString movieName, int movieYear, int movieLength, QString movieGenre,
                     QString movieDirector, QString movieCast, QString movieRating, QString movieImage);

private:
    Ui::MainWindow *ui;
    QDialog *dialogWindow;
};
#endif // MAINWINDOW_H
