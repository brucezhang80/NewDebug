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
    void setCompleterSlot(const QString &text); // 动态的显示完成列表
    void completeTextSlot(const QModelIndex &index); // 点击完成列表中的项，使用此项自动完成输入的单词

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    void mousePressEvent(QMouseEvent *me);

private:
    QStringList words; // 整个完成列表的单词
    QListView *listView; // 完成列表
    QStringListModel *model; // 完成列表的model
};

#endif // COMPLETELINEEDIT_H
