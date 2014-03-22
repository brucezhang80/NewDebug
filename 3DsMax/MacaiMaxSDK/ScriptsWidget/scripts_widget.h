#ifndef SCRIPTS_WIDGET_H
#define SCRIPTS_WIDGET_H

#include <QWidget>
#include "xmlreader.h"
#include "xmlwriter.h"

namespace Ui {
class ScriptsWidget;
}

class ScriptsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScriptsWidget(QWidget *parent = 0);
    ~ScriptsWidget();

    bool loadFile(const QString &xmlfile);
    bool saveFile(const QString &xmlFile);

private:
    void resetDefault();
    void reloadData();

    void listArchiveFiles();
    void listArchiveFiles(const QString &dir , int index);

    Ui::ScriptsWidget *ui;
    XmlReader       *m_reader;
    XmlWriter       *m_writer;
    QString          m_xmlFile;
};

#endif // SCRIPTS_WIDGET_H
