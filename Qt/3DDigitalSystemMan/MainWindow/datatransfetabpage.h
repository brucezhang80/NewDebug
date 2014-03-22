#ifndef DATATRANSFETABPAGE_H
#define DATATRANSFETABPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QSplitter>

class MainWindow;
class DataTransfe;
class DataTransfePlan;
class DataTransfeHistory;
class DataTransfeOptions;

class DataTransfeTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit DataTransfeTabPage(QWidget *parent = 0);
    
signals:
    
public slots:
    void OnDataCommitBtnSlot();
    void OnDataHistoryBtnSlot();
    void OnDataPlansBtnSlot();

protected:
    void setSlotSignalConnect();

public:
    QToolButton         *m_dataCommitBtn;
    QToolButton         *m_dataHistoryBtn;
    QToolButton         *m_dataPlansBtn;

//    QSplitter           *m_vSplitter;
//    QSplitter           *m_leftSplitter;
//    QSplitter           *m_rightSplitter;

//    DataTransfe         *m_dataTransfe;
    DataTransfePlan     *m_dataTransfePlan;
    DataTransfeHistory  *m_dataTransfeHistory;
    DataTransfeOptions  *m_dataTransfeOptions;
};

#endif // DATATRANSFETABPAGE_H
