#ifndef MESSAGEBOX_PAGE_H
#define MESSAGEBOX_PAGE_H

#include <QWidget>

namespace Ui {
class MessageConfigPage;
}

class MessageConfigPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit MessageConfigPage(QWidget *parent = 0);
    ~MessageConfigPage();
    void retranslateUi();
    
private:
    Ui::MessageConfigPage *ui;
};

#endif // MESSAGEBOX_PAGE_H
