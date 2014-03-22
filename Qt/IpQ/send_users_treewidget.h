#ifndef FILE_USER_TREEWIDGET_H
#define FILE_USER_TREEWIDGET_H

#include <QTreeWidget>
#include <QItemDelegate>
#include <QApplication>

class SendUsersTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SendUsersTreeWidget(QWidget *parent = 0);
    void retranslateUi();
    int userCount() const;
    int completedOfRow();

signals:
    
public slots:

private:
    void initialize();
    
    int completedPercent;
};

class SendUsersItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline SendUsersItemDelegate(QWidget *parent) : QItemDelegate(parent) {}

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
    {
        if (index.column() != 1)
        {
            QItemDelegate::paint(painter, option, index);
            return;
        }

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
        int progress = 0;// qobject_cast<SendUsersTreeWidget *>(parent())->completedPercent;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};

#endif // FILE_USER_TREEWIDGET_H
