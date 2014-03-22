#ifndef SCRIPTS_WIDGET_H
#define SCRIPTS_WIDGET_H

#include <QWidget>

namespace Ui {
class ScriptsWidget;
}

class ScriptsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScriptsWidget(QWidget *parent = 0);
    ~ScriptsWidget();
    
private slots:
    void saveXmlFile();

private:
    Ui::ScriptsWidget *ui;

    QString         m_xmlfile;
};

#endif // SCRIPTS_WIDGET_H
