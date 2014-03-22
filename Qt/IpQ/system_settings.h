#ifndef SYSTEM_SETTINGS_H
#define SYSTEM_SETTINGS_H

#include <QDialog>
#include <QWidget>
#include <QListWidget>

namespace Ui {
class SystemSettings;
}

class SystemSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit SystemSettings(QWidget *parent = 0);
    ~SystemSettings();
    void retranslateUi();

private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void createIcons();
    Ui::SystemSettings *ui;
};

#endif // SYSTEM_SETTINGS_H
