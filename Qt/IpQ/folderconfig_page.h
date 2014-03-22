#ifndef FOLDERCONFIG_PAGE_H
#define FOLDERCONFIG_PAGE_H

#include <QWidget>

namespace Ui {
class FolderConfigPage;
}

class FolderConfigPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit FolderConfigPage(QWidget *parent = 0);
    ~FolderConfigPage();
    
    void retranslateUi();

private:
    void initialize();

    Ui::FolderConfigPage *ui;
};

#endif // FOLDERCONFIG_PAGE_H
