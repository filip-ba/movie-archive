#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTableWidgetItem>


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
    void addMovie();
    void deleteMovie();
    void editMovie(QTableWidgetItem *item);
    void clearSearch();
    void backupDataFile();
    void saveMovie(QString movieName, int movieYear, int movieLength, QString movieGenre,
                   QString movieDirector, QString movieCast, QString movieRating, QString movieImage);

private:
    Ui::MainWindow *ui;
    QDialog *dialogWindow;
    void loadDataFromFile();
    void displayMovieCover(int row, QString imageName, QString imagePath);
    void displayEntireList();
    void updateTableWithSearch();
    void updateDataInFile(int row, int col, const QString &updatedText);
    void saveDataToFile();
    void createEmptyDataFile(const QString& filePath);
};
#endif // MAINWINDOW_H
