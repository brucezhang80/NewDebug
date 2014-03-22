#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "docmodel.h"
#include <QMenu>
#include <QDomDocument>
#include <QFileDialog>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()),
                        QKeySequence::Open);
    m_fileMenu->addAction(tr("E&xit"), this, SLOT(close()),
                        QKeySequence::Quit);

    QDomDocument doc;
    m_model = new DocModel(doc, this);
    m_treeview = new TreeViewWidget(this);
    m_treeview->setModel(m_model);

    this->setCentralWidget(m_treeview);
    this->setWindowTitle(tr("Simple DOM XML Model"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    m_xmlPath, tr("XML files (*.xml);;HTML files (*.html);;"
                                                                "SVG files (*.svg);;User Interface files (*.ui)"));

    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QDomDocument document;
            if (document.setContent(&file))
            {
                DocModel *newModel = new DocModel(document, this);
                m_treeview->setModel(newModel);
                delete m_model;
                m_model = newModel;
                m_xmlPath = filePath;
                return true;
            }
            file.close();
        }
    }

    return false;
}
