#ifndef SEND_FILE_PROPERTISE_H
#define SEND_FILE_PROPERTISE_H

#include <QWidget>
#include <QFocusEvent>
#include <QComboBox>

namespace Ui {
class SendFilePropertise;
}

class SendFilePropertise : public QWidget
{
    Q_OBJECT
    
public:
    explicit SendFilePropertise(QWidget *parent = 0);
    ~SendFilePropertise();
    void retranslateUi();

    void addProjectPro();
    void delProjectPro();
    QString currentProjectName();
signals:
    void updateProjectLabel(const QString &label);

private slots:
    void currentProjectNameChanged(const QString &name);
    void currentProjectZoneChanged(const QString &name);
    void currentProjectIdChanged(const QString &name);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void updataCombobox();
    void formateCombox();
    QStringList comboboxLabels(QComboBox *cbox);
    void loadSendProp();
    void saveSendProp();

    Ui::SendFilePropertise *ui;
    QString     m_curProjectName;
    QString     m_curProjectZone;
    QString     m_curProjectId;

};

#endif // SEND_FILE_PROPERTISE_H
