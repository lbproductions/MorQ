#include <QApplication>

#include "controller/controller.h"
#include "ui/mainwindow/mainwindow.h"

#include <QMessageBox>
#include <QSqlError>
#include <QFile>
#include <QDir>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("MorQServer");
    QApplication::setOrganizationName("LBProductions");
    QApplication::setApplicationVersion("VERSION");

    if(a.arguments().size() < 2) {
        qDebug("Pass the database as first argument.");
        return 0;
    }

    QString dbFileName = a.arguments().at(1);

    {
        QFile dbFile(dbFileName);
        if(!dbFile.exists()) {
            QDir dir;
            dir.mkpath(dbFileName);
            dbFile.open(QFile::ReadOnly);
            dbFile.close();
        }
    }

    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbFileName);
        if (!db.open()) {
            qCritical() << db.lastError();
            return 0;
        }
        Qp::setDatabase(db);
    }

    bool cleanDatabase = a.arguments().size() > 2 && a.arguments().at(2) == QLatin1String("cleanDatabase");
    Controller::initialize(cleanDatabase);

    MainWindow::instance()->show();
    int ret = a.exec();
    delete MainWindow::instance();
    return ret;
}
