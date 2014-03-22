#ifndef TOOLBARTABWIDGET_H
#define TOOLBARTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class ScriptTreeWidget;
class ModelTreeWidget;

class ToolbarTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ToolbarTabWidget(QWidget *parent = 0);

    //Overridden method from QTabWidget
    QTabBar* tabBar()
    {
      return QTabWidget::tabBar();
    }

protected:
    void initCurrentPage();

signals:
    
public slots:
    void itemChangeSlot(int index);

private:
};

#endif // TOOLBARTABWIDGET_H
