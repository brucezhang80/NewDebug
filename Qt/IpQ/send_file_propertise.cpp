#include "send_file_propertise.h"
#include "ui_sendfilepropertise.h"
#include "constants.h"
#include "system.h"

#include <QFile>
#include <QStringList>
#include <QCompleter>
#include <QTextCodec>

#include <QDebug>

SendFilePropertise::SendFilePropertise(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendFilePropertise)
{
    ui->setupUi(this);
    ui->projectCbox->installEventFilter(this);
    ui->projectZoneCbox->installEventFilter(this);
    ui->projectIdCbox->installEventFilter(this);
    loadSendProp();

    formateCombox();
    updataCombobox();
//    QString iniLabel = ".." + ui->projectCbox->currentText() + "/" + ui->projectZoneCbox->currentText() + "/" + \
//                        ui->projectIdCbox->currentText();
//    ui->projectPathLb->setText(iniLabel);

    connect( ui->projectCbox, SIGNAL(currentIndexChanged (QString)), this, SLOT(currentProjectNameChanged(QString)) );
    connect( ui->projectZoneCbox, SIGNAL(currentIndexChanged (QString)), this, SLOT(currentProjectZoneChanged(QString)) );
    connect( ui->projectIdCbox, SIGNAL(currentIndexChanged (QString)), this, SLOT(currentProjectIdChanged(QString)) );
    connect( this, SIGNAL(updateProjectLabel(QString)), ui->projectPathLb, SLOT(setText(QString)) );
}

SendFilePropertise::~SendFilePropertise()
{
    saveSendProp();
    delete ui;
}

void SendFilePropertise::retranslateUi()
{
    ui->retranslateUi(this);
}

void SendFilePropertise::loadSendProp()
{
    QString filename = System::localIPQDocument() + IPQSendPropName;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QStringList lines;
    QTextStream in(&file);
    while ( !in.atEnd() )
    {
        QString line = in.readLine();
        lines.append(line);
    }

    for(int i = 0; i < lines.count(); ++i)
    {
        QStringList labelList = lines.at(i).split("|");
        if( i == 0 )
        {
            QCompleter *completer = new QCompleter(labelList, ui->projectCbox);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            ui->projectCbox->setCompleter(completer);
            ui->projectCbox->addItems(labelList);
        }
        else if( i == 1)
        {
            QCompleter *completer = new QCompleter(labelList, ui->projectZoneCbox);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            ui->projectZoneCbox->setCompleter(completer);
            ui->projectZoneCbox->addItems(labelList);
        }
        else if( i == 2)
        {
            QCompleter *completer = new QCompleter(labelList, ui->projectIdCbox);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            ui->projectIdCbox->setCompleter(completer);
            ui->projectIdCbox->addItems(labelList);
        }
    }
}

void SendFilePropertise::saveSendProp()
{
    qDebug() << "saveSendProp";
    QString filename = System::localIPQDocument() + IPQSendPropName;
    QFile file(filename);
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for(int i = 1; i < ui->projectCbox->count(); ++i)
    {
        QString label = ui->projectCbox->itemText(i)+"|";
        out<<label;
    }
    out<<"\n";
    for(int i = 1; i < ui->projectZoneCbox->count(); ++i)
    {
        QString label = ui->projectZoneCbox->itemText(i)+"|";
        out<<label;
    }
    out<<"\n";
    for(int i = 1; i < ui->projectIdCbox->count(); ++i)
    {
        QString label = ui->projectIdCbox->itemText(i)+"|";
        out<<label;
    }
}

bool SendFilePropertise::eventFilter(QObject *obj, QEvent *event)
{
    QComboBox *cobox = qobject_cast<QComboBox*>(obj);
    if( cobox == ui->projectCbox)
    {
        if( event->type() == QEvent::FocusOut )
        {
            QString curText = ui->projectCbox->currentText();
            if( !curText.isEmpty() && !comboboxLabels(ui->projectCbox).contains(curText))
            {
                ui->projectCbox->insertItem(1, curText);
                ui->projectCbox->setCurrentIndex(1);
                return true;
            }
        }
    }
    if( cobox == ui->projectZoneCbox)
    {
        if( event->type() == QEvent::FocusOut )
        {
            QString curText = ui->projectZoneCbox->currentText();
            if( !curText.isEmpty() && !comboboxLabels(ui->projectZoneCbox).contains(curText))
            {
                ui->projectZoneCbox->insertItem(1, curText);
                ui->projectZoneCbox->setCurrentIndex(1);
                return true;
            }
        }
    }
    if( cobox == ui->projectIdCbox)
    {
        if( event->type() == QEvent::FocusOut )
        {
            QString curText = ui->projectIdCbox->currentText();
            if( !curText.isEmpty() && !comboboxLabels(ui->projectIdCbox).contains(curText))
            {
                ui->projectIdCbox->insertItem(1, curText);
                ui->projectIdCbox->setCurrentIndex(1);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void SendFilePropertise::updataCombobox()
{
    if( ui->projectCbox->count() == 0 || ui->projectCbox->currentText() == tr("none"))
    {
        ui->projectZoneCbox->setCurrentIndex(0);
        ui->projectIdCbox->setCurrentIndex(0);
        ui->projectZoneCbox->setEnabled(false);
        ui->projectIdCbox->setEnabled(false);
        return;
    }
    else
        ui->projectZoneCbox->setEnabled(true);

    if( ui->projectZoneCbox->count() == 0 || ui->projectZoneCbox->currentText() == tr("none"))
    {
        ui->projectIdCbox->setCurrentIndex(0);
        ui->projectIdCbox->setEnabled(false);
        return;
    }
    else
        ui->projectIdCbox->setEnabled(true);
}

void SendFilePropertise::formateCombox()
{
    for(int i = 0; i < ui->projectCbox->count(); ++i)
    {
        QString label = ui->projectCbox->itemText(i);
        if( label.isEmpty() )
            ui->projectCbox->removeItem(i);
    }
    for(int i = 0; i < ui->projectZoneCbox->count(); ++i)
    {
        QString label = ui->projectZoneCbox->itemText(i);
        if( label.isEmpty() )
            ui->projectZoneCbox->removeItem(i);
    }
    for(int i = 0; i < ui->projectIdCbox->count(); ++i)
    {
        QString label = ui->projectIdCbox->itemText(i);
        if( label.isEmpty() )
            ui->projectIdCbox->removeItem(i);
    }
}

QStringList SendFilePropertise::comboboxLabels(QComboBox *cbox)
{
    QStringList list;
    if( !cbox ) return list;

    int size = cbox->count();
    for(int i = 0; i < size; ++i)
    {
        QString label = cbox->itemText(i);
        list.append(label);
    }

    return list;
}


QString SendFilePropertise::currentProjectName()
{
    QString curText = ui->projectCbox->currentText();
    if(curText.isEmpty())
    {
        ui->projectCbox->setCurrentIndex(0);
        curText = ui->projectCbox->currentText();
    }

    return curText;
}

void SendFilePropertise::currentProjectNameChanged(const QString &name)
{
    m_curProjectName = name;

    QString str, sep = "/";
    if(m_curProjectName == tr("none"))
    {
        str = "../";
    }
    else if(m_curProjectZone == tr("none") || m_curProjectZone.isEmpty())
    {
        str = "../" + m_curProjectName;
    }
    else if(m_curProjectId == tr("none") || m_curProjectId.isEmpty())
    {
        str = "../" + m_curProjectName + sep + m_curProjectZone;
    }
    else
        str = "../" + m_curProjectName + sep + m_curProjectZone + sep + m_curProjectId;


    updataCombobox();
    emit updateProjectLabel(str);
}

void SendFilePropertise::currentProjectZoneChanged(const QString &name)
{
    m_curProjectZone = name;
    m_curProjectName = ui->projectCbox->currentText();
    m_curProjectId = ui->projectIdCbox->currentText();

    QString str, sep = "/";
    if( m_curProjectZone == tr("none"))
    {
        str = "../" + m_curProjectName;
    }
    else if( m_curProjectId == tr("none"))
    {
        str = "../" + m_curProjectName + sep + m_curProjectZone;
    }
    else
        str = "../" + m_curProjectName + sep + m_curProjectZone + sep + m_curProjectId;

    updataCombobox();
    emit updateProjectLabel(str);
}

void SendFilePropertise::currentProjectIdChanged(const QString &name)
{
    m_curProjectId = name;
    m_curProjectName = ui->projectCbox->currentText();
    m_curProjectZone = ui->projectZoneCbox->currentText();

    QString str, sep = "/";
    if( m_curProjectId == tr("none"))
    {
        str = "../" + m_curProjectName + sep + m_curProjectZone;
    }
    else
        str = "../" + m_curProjectName + sep + m_curProjectZone + sep + m_curProjectId;
    emit updateProjectLabel(str);
    //formateCombox();
}
