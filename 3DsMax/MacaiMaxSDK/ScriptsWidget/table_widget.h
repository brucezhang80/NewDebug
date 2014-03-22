#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QTableWidget>
#include <QToolButton>

class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableWidget(QWidget *parent = 0);
    
    QList<QToolButton*> addButton(const int &count, const QStringList &titles, const QList<QIcon> &icons);
    QList<QToolButton*> addButton(const int &count, const QStringList &titles);
    QToolButton* addButton(const QString &title, const QString &icon, const QString &archive);
    QToolButton* addButton(const QString &title);

    QList<QToolButton*> itemButtons();
    QToolButton* itemButton( int row );

signals:
//    void cellClicked(int row, int column);

public slots:
    void buttonClicked();

private:
    void initialize();
    void initStyleSheet();
};

#endif // TABLE_WIDGET_H
