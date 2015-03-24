#include <QtGui/QApplication>
#include "mainwindow.h"
#include "linp_exception.h"
#include <QtGlobal>
#include <QMessageBox>
#include <QString>

const unsigned SEED = 1;

int main(int argc, char *argv[])
{
    qsrand(SEED);
    QApplication a(argc, argv);
    try {
        MainWindow w;
        w.show();
        return a.exec();
    } catch(LinpException &e) {
        QMessageBox::critical(NULL, "Error!", QString(e.what()) + " Program will be terminated.");
        return 0;
    }
    return -1;
}
