#ifndef PROJECTSVIEW_H
#define PROJECTSVIEW_H
#include "picproject.h"
#include <QtGui>
#include <QTreeWidget>
#include <QAbstractItemModel>


class PicProject;
class PicProjectViewModel;
class ProjectView : public QTreeWidget
{
    Q_OBJECT
public:
    ProjectView(QWidget *parent = 0);
    void addProject(PicProject *);

public:
    PicProject      *m_picProject;
    PicProjectViewModel* ppViewModel;
};

#endif // PROJECTSVIEW_H
