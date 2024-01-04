#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QDialog>
#include <QPair>

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
    void on_btnSave_clicked();
    void on_btnSelectImage_clicked();

signals:
    void dataSaved(QString movieName, int movieYear, int movieLength, QString movieGenre, QString movieDirector, QString movieCast, QString movieRating, QString movieImage);

private:
    Ui::DialogWindow *ui;
};

#endif // DIALOGWINDOW_H
