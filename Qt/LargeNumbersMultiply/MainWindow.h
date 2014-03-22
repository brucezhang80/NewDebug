#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void multiply(const QVector<int>& multiplier,
                  const QVector<int>& multiplicand,
                  QVector<int> &result);
public slots:
    void doCompute();
    void doClean();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
