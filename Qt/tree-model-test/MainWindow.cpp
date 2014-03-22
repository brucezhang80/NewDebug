#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "dndmodel.h"
#include "constance.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // open database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydatabase1.db");
    if(!db.open())
    {
        qDebug("database error");
        return;
    }

    QStringList tableList = db.tables();
    QSqlQuery *query = new QSqlQuery(db);
    if(!tableList.contains( SQL_TABEL_TREETAG ))
    {
        QString createTable = QString("CREATE TABLE %1"
            "(ID INTEGER PRIMARY KEY AUTOINCREMENT, sID INTEGER UNIQUE, pID INTEGER, name TEXT)").arg(SQL_TABEL_TREETAG);
        if ( !query->exec(createTable) )
        {
            QMessageBox::warning(this, tr("Create table error"), tr("Error: ")+query->lastError().text(), QMessageBox::Ok);
            return;
        }
        query->exec("INSERT INTO tags(sID, pID, name) VALUES (1, 0, 'item 1')");
        query->exec("INSERT INTO tags(sID, pID, name) VALUES (2, 1, 'sub 1')");
        query->exec("INSERT INTO tags(sID, pID, name) VALUES (3, 2, 'sub sub 1')");
        query->exec("INSERT INTO tags(sID, pID, name) VALUES (4, 2, 'sub sub 2')");
        query->exec("INSERT INTO tags(sID, pID, name) VALUES (5, 0, 'item 2')");
        query->exec("INSERT INTO tags(sID, pID, name) VALUES (6, 5, 'sub 1')");
    }


    // setup model
//    model = new TreeModel(query);
    DragDropModel *dndModel = new DragDropModel(query);
    ui->treeView->setModel(dndModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
