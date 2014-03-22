#ifndef DATATRANSFEOPTIONS_H
#define DATATRANSFEOPTIONS_H

#include <QtCore\qglobal.h>
#if defined(DATATRANSFE_LIBRARY)
#  define DATATRANSFEOPTIONSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATATRANSFEOPTIONSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>

namespace Ui {
class DataTransfeOptions;
}

class DATATRANSFEOPTIONSSHARED_EXPORT DataTransfeOptions : public QWidget
{
    Q_OBJECT
    
public:
    explicit DataTransfeOptions(QWidget *parent = 0);
    ~DataTransfeOptions();

signals:
    void startCommitSignal();

public slots:
    void startCommitSlot();
    void updateControlSlot(bool);

private:
    Ui::DataTransfeOptions *ui;
};

#endif // DATATRANSFEOPTIONS_H
