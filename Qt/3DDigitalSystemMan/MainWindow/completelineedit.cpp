#include "completelineedit.h"

CompleteLineEdit::CompleteLineEdit(QStringList words, QWidget *parent) :
    QLineEdit(parent),words(words)
{
    listView = new QListView(this);
    model = new QStringListModel(this);
    listView->setWindowFlags(Qt::ToolTip);
    this->setText( tr("�����ؼ���") );

    connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(setCompleterSlot(const QString &)));
    connect(listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(completeTextSlot(const QModelIndex &)));
}

void CompleteLineEdit::setCompleterSlot(const QString &text)
{
    if(text ==  tr("�����ؼ���") ) return;
    if (text.isEmpty())
    {
        listView->hide();
        return;
    }

    if ((text.length() > 1) && (!listView->isHidden()))
    {
        return;
    }

    // �������������б��е�ĳ�����ʰ���������ı��������Ҫ��ʾ������б���
    QStringList sl;
    foreach(QString word, words)
    {
        if (word.contains(text))
        {
            sl << word;
        }
    }

    model->setStringList(sl);
    listView->setModel(model);

    if (model->rowCount() == 0)
    {
        return;
    }

    // Position the text edit
    listView->setMinimumWidth(width());
    listView->setMaximumWidth(width());

    QPoint p(0, height());
    int x = mapToGlobal(p).x();
    int y = mapToGlobal(p).y() + 1;

    listView->move(x, y);
    listView->show();

}

void CompleteLineEdit::completeTextSlot(const QModelIndex &index)
{
    QString text = index.data().toString();
    setText(text);
    listView->hide();
}

void CompleteLineEdit::keyPressEvent(QKeyEvent *e)
{
    if (!listView->isHidden())
    {
        int key = e->key();
        int count = listView->model()->rowCount();
        QModelIndex currentIndex = listView->currentIndex();

        if (Qt::Key_Down == key)
        {
            // �����·����ʱ���ƶ����ѡ����һ������б��е���
            int row = currentIndex.row() + 1;
            if (row >= count)
            {
                row = 0;
            }

            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        }
        else if (Qt::Key_Up == key)
        {
            // �����·����ʱ���ƶ����ѡ����һ������б��е���
            int row = currentIndex.row() - 1;
            if (row < 0)
            {
                row = count - 1;
            }

            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        }
        else if (Qt::Key_Escape == key)
        {
            // ����Esc��ʱ����������б�
            listView->hide();
        }
        else if (Qt::Key_Enter == key || Qt::Key_Return == key)
        {
            // ���»س���ʱ��ʹ������б���ѡ�е������������б�
            if (currentIndex.isValid())
            {
                QString text = listView->currentIndex().data().toString();
                setText(text);
            }
            listView->hide();
        }
        else
        {
            // �����������������б���ʹ��QLineEdit�ļ��̰����¼�
            listView->hide();
            QLineEdit::keyPressEvent(e);
        }
    }
    else
    {
        QLineEdit::keyPressEvent(e);
    }
}


void CompleteLineEdit::focusOutEvent(QFocusEvent *e)
{
    // ������λ�ò���listView�����ڣ�����listView
    QRect listviewRect = QRect(listView->pos(), listView->size() );
    if( !listviewRect.contains(QCursor::pos()) )
        listView->hide();

    //ʧȥ�����������ʾ�������ؼ��֡�
    if(this->text().isEmpty())
        this->setText(QString(tr("�����ؼ���")));

    QLineEdit::focusOutEvent(e);
}

//��갴�£�ȥ���������ؼ��֡�
void CompleteLineEdit::mousePressEvent(QMouseEvent *me)
{
    if(this->text() == tr("�����ؼ���"))
        this->setText(QString(tr("")));
    else
        this->selectAll();

    QLineEdit::mousePressEvent(me);
}

