#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "linp_core.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    LinpCore *application;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_nextQuestionButton_clicked();

    void on_checkAnswerButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
