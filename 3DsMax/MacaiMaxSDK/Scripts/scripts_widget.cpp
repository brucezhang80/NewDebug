#include "scripts_widget.h"
#include "ui_scriptswidget.h"
#include "constants.h"
#include "plugin.h"

#include <QDir>

ScriptsWidget::ScriptsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptsWidget)
{
    ui->setupUi(this);

    QString data_path = Plugin::dataPath();
    m_xmlfile = data_path + QDir::separator() + XML_FILENAME;
    bool ok = ui->scriptsTreeWidget->loadFile(m_xmlfile);
    if( !ok )
    {
        ui->scriptsTreeWidget->listArchiveFiles();
//        ui->scriptsTreeWidget->saveFile( m_xmlfile );
    }
}

ScriptsWidget::~ScriptsWidget()
{
    ui->scriptsTreeWidget->saveFile( m_xmlfile );
    delete ui;
}

void ScriptsWidget::saveXmlFile()
{
    ui->scriptsTreeWidget->saveFile( m_xmlfile );
}
