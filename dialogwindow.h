#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QDialog>
#include <QPair>
#include <QSlider>
#include <QLineEdit>


namespace Ui {
class DialogWindow;
}

class DialogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWindow(QWidget *parent = nullptr);
    ~DialogWindow();

private slots:
    void saveMovie();
    void selectImage();
    void onSliderValueChanged(QSlider* slider, QLineEdit* lineEdit);
    void calculateRating();
    void setDefaultValues();

signals:
    void dataSaved(QString movieName, int movieYear, int movieLength, QString movieGenre,
                   QString movieDirector, QString movieCast, QString movieRating, QString movieImage);

private:
    Ui::DialogWindow *ui;
};

#endif // DIALOGWINDOW_H
