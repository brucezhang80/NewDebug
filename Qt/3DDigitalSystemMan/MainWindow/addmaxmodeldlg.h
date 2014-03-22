#ifndef ADDMAXMODELDLG_H
#define ADDMAXMODELDLG_H

#include <QDialog>
#include <QMouseEvent>
#include <QLabel>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QStandardItem>
#include <QItemDelegate>
#include <QPainter>
#include <QDebug>

namespace Ui {
class AddMaxModelDlg;
}

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline ComboBoxDelegate(QWidget *parent = 0) : QItemDelegate(parent) {}

protected:
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QString type = index.data(Qt::AccessibleDescriptionRole).toString();
        if ( type == QLatin1String("separator"))
            return QSize(5,5);
        return QItemDelegate::sizeHint(option, index);
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QString type = index.data(Qt::AccessibleDescriptionRole).toString();
        if ( type == QLatin1String("separator"))
        {
            QItemDelegate::paint(painter, option, index);
            int y = (option.rect.top() + option.rect.bottom()) / 2;
            painter->setPen(option.palette.color(QPalette::Active, QPalette::Dark));
            painter->drawLine(option.rect.left(), y, option.rect.right(), y);
        }
        else if( type == QLatin1String("parent") )
        {
            QStyleOptionViewItem parentOption = option;
            parentOption.state |= QStyle::State_Enabled;
            QItemDelegate::paint(painter, parentOption, index);
        }
        else if( type == QLatin1String("child") )
        {
            QStyleOptionViewItem childOption = option;
            int indent = option.fontMetrics.width(QString(4, QChar(' ')));
            childOption.rect.adjust(indent, 0, 0, 0);
            childOption.textElideMode = Qt::ElideNone;

            QItemDelegate::paint(painter, childOption, index);
        }
        else
        {
            QItemDelegate::paint(painter, option, index);
        }
    }
};

class ModelTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ModelTypeComboBox(QWidget *parent = 0): QComboBox(parent)
    {
    }
signals:

public slots:


public:
    void addSeparator()
    {
        insertSeparator( count() );
    }

    void addParentItem(const QString& text)
    {
        QStandardItem *item = new QStandardItem(text);
        item->setFlags(item->flags() &~(Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        item->setData("parent", Qt::AccessibleDescriptionRole);

        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);

        QStandardItemModel* itemModel = (QStandardItemModel*)( this->model() );
        itemModel->appendRow(item);
    }

    void addChildItem(const QString& text)//, const QVariant& data)
    {
        QStandardItem* item = new QStandardItem(text + QString(4, QChar(' ')));
        //item->setData(data, Qt::UserRole);
        item->setData("child", Qt::AccessibleDescriptionRole);

        QStandardItemModel* itemModel = (QStandardItemModel*)(this->model());
        itemModel->appendRow(item);
    }

private:

};

class AddMaxModelDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddMaxModelDlg(QWidget *parent = 0);
    ~AddMaxModelDlg();
    void SetModelThumbnail();

signals:
    void addMaxModelSignal(const QString&, const QString&, const QString&, const QString&);
    void setComboBoxModelTypeDateSignal();

public slots:
    void OnSelMaxModelFileSlot();
    void OnButtonBoxSlot(QAbstractButton *button);
    void OnSetComboBoxModelTypeDataSlot(const QString &text, const bool &isParent);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    Ui::AddMaxModelDlg *ui;

public:
    QString m_maxFilename;
    QString m_thumbFilename;
};

#endif // ADDMAXMODELDLG_H
