#include "configuration_page.h"
#include "ui_configurationpage.h"
#include "macai.h"
#include "window_manager.h"
#include "preferences.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QPalette>
#include <QTranslator>

#include <QDebug>

static const QString ThemePath = ":/themes";
static const QString LangPath = ":/lang";

ConfigurationPage::ConfigurationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigurationPage)
{
    ui->setupUi(this);
    loadThemeFiles();
    loadLangFiles();

    m_fsWatcher.addPath(ThemePath);
    connect(&m_fsWatcher, SIGNAL(directoryChanged(QString)),
            this, SLOT(directoryChanged(QString)) );
    connect(&m_fsWatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(fileChanged(QString)) );
    connect(ui->themeCbox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentThemeChanged(int)) );
    connect(ui->languageCbox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentLangChanged(int)) );
    connect(ui->themeclrSlider, SIGNAL(valueChanged(int)),
            this, SLOT(colorSliderValueChanged(int)));
}

ConfigurationPage::~ConfigurationPage()
{
    delete ui;
}

void ConfigurationPage::retranslateUi()
{
    ui->retranslateUi(this);
}

QString ConfigurationPage::currentTheme() const
{
    return Macai::preferences->themeFile;
}

QString ConfigurationPage::currentLang() const
{
    return Macai::preferences->langFile;
}

void ConfigurationPage::directoryChanged(const QString &/*path*/)
{
    loadThemeFiles();
}

void ConfigurationPage::fileChanged(const QString &/*path*/)
{
    loadThemeFiles();
}

void ConfigurationPage::currentThemeChanged(int index)
{
    QString theme_file = m_themeFiles.at(index);
    Macai::setThemeAll(theme_file);
    Macai::windowManager->setMessageWindowTheme(theme_file);
    Macai::preferences->themeFile = theme_file;
}

void ConfigurationPage::currentLangChanged(int index)
{
    QString lang_file = m_langFiles.at(index);
    Macai::preferences->langFile = lang_file;
    if(!QFile::exists(lang_file))
            qWarning("failed-no file");
    Macai::retranslateAllUi(lang_file);
}

void ConfigurationPage::colorSliderValueChanged(int value)
{
    QColor color;
    // 分6个色块
    // 色块0 (255, 0, value) value:4-255
    // 色块1 (value, 0, 255) value:255-0
    // 色块2 (0, value, 255) value:0-255
    // 色块3 (0, 255, value) value:255-0
    // 色块4 (value, 255, 0) value:0-255
    // 色块5 (255, value, 0) value:255-0

    // hue: 360 - 0
    int hue = 360;

    int nclr;
    if( value <= 255)//0
    {
        color.setRgb(255, 0, value, 255);
    }
    else if( value <= 510)//1
    {
        nclr = 510-value;
        color.setRgb(nclr, 0, 255, 255);
    }
    else if( value <= 765)//2
    {
        nclr = value-510;
        color.setRgb(0, nclr, 255, 255);
    }
    else if( value <= 1020)//3
    {
        nclr = 1020-value;
        color.setRgb(0, 255, nclr, 255);
    }
    else if( value <= 1275)//4
    {
        nclr = value-1020;
        color.setRgb(nclr, 255, 0, 255);
    }
    else    //5
    {
        nclr = 1530 - value;
        color.setRgb(255, nclr, 0, 255);
    }
//    hue = hue - (int)((double)value/4.25);
//    color.setHsl(hue, 255, 255);

    Macai::setThemeBkColor(color);
}

void ConfigurationPage::currentCheckboxChanged(int index)
{

}

void ConfigurationPage::loadThemeFiles()
{
    QStringList lables;
    QDir dir(ThemePath);
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        lables << fileInfo.fileName();// include .xxx but not path
        m_themeFiles.append(fileInfo.filePath());
    }
    ui->themeCbox->addItems(lables);
}

void ConfigurationPage::loadLangFiles()
{
    QStringList lables;
    QDir dir(LangPath);
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        lables << fileInfo.baseName();
        m_langFiles.append(LangPath+"/"+fileInfo.fileName());
    }
    ui->languageCbox->addItems(lables);
}


