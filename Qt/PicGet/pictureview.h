#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QtGui>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QCloseEvent;
class QItemDelegate;
QT_END_NAMESPACE


// TorrentViewDelegate is used to draw the progress bars.
class DataViewDelegate : public QItemDelegate
{
    Q_OBJECT
    QScopedPointer<QPushButton> button;
public:
    inline DataViewDelegate(QWidget *parent = 0) : QItemDelegate(parent),button(new QPushButton)
    {
        button->setStyleSheet("background-color: red");
        QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect(button.data());
        effect->setColor(Qt::red);
        button->setGraphicsEffect(effect);
    }

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
    {

        //网格
//        QPen pen;
//        pen.setColor(QColor(Qt::lightGray));
//        pen.setWidth(1);
//        painter->setPen(pen);
//        painter->drawRect(option.rect);

        if (index.column() != 2 && index.column() != 5)
        {
            QItemDelegate::paint(painter, option, index);
            return;
        }

        if( index.column() == 5)
        {
//            button->setFixedSize(100, 50);
//            button->render(painter, QPoint(50, 25));

            QStyleOptionButton* pbtnRemovedPic = new QStyleOptionButton;
            pbtnRemovedPic->text = QApplication::tr("-");
            pbtnRemovedPic->state = QStyle::State_Enabled | QStyle::State_Active;
            pbtnRemovedPic->direction = QApplication::layoutDirection();
            pbtnRemovedPic->rect = QRect(option.rect.x(), option.rect.y(), 20, option.rect.height());
            /*button->*/QApplication::style()->drawControl(QStyle::CE_PushButton, pbtnRemovedPic, painter/*, button.data()*/);
        }
        else if (index.column() == 2)
        {
        // Set up a QStyleOptionProgressBar to precisely mimic the
        // environment of a progress bar.
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        // Set the progress and text values of the style option.
        int progress = 40;//qobject_cast<PictureView *>(parent())->clientForRow(index.row())->progress();
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
        }
    }
};

class PictureView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PictureView(QWidget *parent = 0);
    
    void addPicture();
    void delPicture();

signals:

public slots:

protected:

};

#endif // PICTUREVIEW_H
