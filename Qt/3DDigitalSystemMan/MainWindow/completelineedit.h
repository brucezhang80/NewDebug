#ifndef COMPLETELINEEDIT_H
#define COMPLETELINEEDIT_H

#include <QtGui>
#include <QLineEdit>
#include <QModelIndex>
#include <QListView>
#include <QString>
#include <QDebug>

class CompleteLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CompleteLineEdit(QStringList words, QWidget *parent = 0);
    
    void setWords(QStringList wordList){words = wordList;}
    void appendWord(QStringList newWords){words << newWords;}
    QStringList getWords(){return words;}
signals:

public slots:
    void setCompleterSlot(const QString &text); // ��̬����ʾ����б�
    void completeTextSlot(const QModelIndex &index); // �������б��е��ʹ�ô����Զ��������ĵ���

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    void mousePressEvent(QMouseEvent *me);

private:
    QStringList words; // ��������б�ĵ���
    QListView *listView; // ����б�
    QStringListModel *model; // ����б��model
};

#endif // COMPLETELINEEDIT_H
