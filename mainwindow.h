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
    void addMovie(QString movieName, int movieYear, int movieLength, QString movieDirector, QString movieCast, QString movieRating, QString imagePath);
    void displayMovieCover(int row, QString imageName, QString imagePath);
    void onClearSearchClicked();
    void updateTableWithSearch();
    void displayEntireList();

private slots:
    void on_btnAddMovie_clicked();

public slots:
    void on_data_saved(QString movieName, int movieYear, int movieLength, QString movieDirector, QString movieCast, QString movieRating, QString movieImage);
    void on_btnDeleteMovie_clicked();

private:
    Ui::MainWindow *ui;
    QDialog *dialogWindow;
};
#endif // MAINWINDOW_H
