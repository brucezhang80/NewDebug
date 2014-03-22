#include "messagechat_page.h"
#include "ui_messagechatpage.h"

MessageChatPage::MessageChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageChatPage)
{
    ui->setupUi(this);
}

MessageChatPage::~MessageChatPage()
{
    delete ui;
}
