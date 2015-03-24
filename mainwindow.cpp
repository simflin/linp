#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "linp_exception.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    application = new Application;
    on_nextQuestionButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete application;
}

void MainWindow::on_nextQuestionButton_clicked()
{
    on_checkAnswerButton_clicked();
    try {
        ui->questionTextEdit->setText(application->getNextQuestion());
    } catch (ApplicationException &e) {
        QMessageBox::StandardButton res =
                QMessageBox::question(this, "Questions is over",
                                      "Current set of questions is over. Start a new game?",
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (res == QMessageBox::Yes) {
            application->createNewGame();
            ui->questionTextEdit->setText(application->getNextQuestion());
        } else {
            return;
        }
    }
    ui->answerTextEdit->clear();
    ui->answerStatusLabel->clear();
    ui->rightAnswerTextEdit->clear();
    ui->questionLabel->setText(QString("Question ") +
                               QString::number(application->getCurrentQuestionIndex()) +
                               "/" +
                               QString::number(application->getGameSize()) +
                               " (#" +
                               QString::number(application->getCurrentQuestionNumber()) +
                               "):");
    this->statusBar()->showMessage(QString("Correct answers: ") +
                                   QString::number(application->getCorrectAnswersNumber()) + "/" +
                                   QString::number(application->getGameSize()));
}

void MainWindow::on_checkAnswerButton_clicked()
{
    ui->rightAnswerTextEdit->setText(application->getCurrentAnswer());
    if (application->isAnswerCorrect(ui->answerTextEdit->toPlainText())) {
        ui->answerStatusLabel->setText("Your answer is correct");
    } else {
        ui->answerStatusLabel->setText("Your answer is wrong");
    }
    this->statusBar()->showMessage(QString("Correct answers: ") +
                                   QString::number(application->getCorrectAnswersNumber()) + "/" +
                                   QString::number(application->getGameSize()));
}
