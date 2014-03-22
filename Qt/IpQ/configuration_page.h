#ifndef CONFIGURATION_PAGE_H
#define CONFIGURATION_PAGE_H

#include <QWidget>
#include <QFileSystemWatcher>

namespace Ui {
class ConfigurationPage;
}

class ConfigurationPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConfigurationPage(QWidget *parent = 0);
    ~ConfigurationPage();
    void retranslateUi();

    QString currentTheme() const;
    QString currentLang() const;

private slots:
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);
    void currentThemeChanged(int index);
    void currentLangChanged(int index);
    void colorSliderValueChanged(int value);
    void currentCheckboxChanged(int index);

private:
    void loadThemeFiles();
    void loadLangFiles();

    QFileSystemWatcher  m_fsWatcher;
    QList<QString>      m_themeFiles;
    QList<QString>      m_langFiles;


    Ui::ConfigurationPage *ui;
};

#endif // CONFIGURATION_PAGE_H
