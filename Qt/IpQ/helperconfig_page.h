#ifndef HELPERCONFIG_PAGE_H
#define HELPERCONFIG_PAGE_H

#include <QWidget>

namespace Ui {
class HelperConfigPage;
}

class HelperConfigPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit HelperConfigPage(QWidget *parent = 0);
    ~HelperConfigPage();
    void retranslateUi();
    
private:
    Ui::HelperConfigPage *ui;
};

#endif // HELPERCONFIG_PAGE_H
