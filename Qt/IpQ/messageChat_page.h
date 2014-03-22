#ifndef MESSAGEBOX_PAGE_H
#define MESSAGEBOX_PAGE_H

#include <QWidget>

namespace Ui {
class MessageChatPage;
}

class MessageChatPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit MessageChatPage(QWidget *parent = 0);
    ~MessageChatPage();
    
private:
    Ui::MessageChatPage *ui;
};

#endif // MESSAGEBOX_PAGE_H
