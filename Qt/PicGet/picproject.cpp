#include "picproject.h"

PicProject::PicProject()
{
}

void PicProject::setProjectUrl(const QString &url)
{
    this->m_proUrl = url;
}

void PicProject::setProjectPath(const QString &path)
{
    this->m_proPath = path;
}

void PicProject::setProjectTitle(const QString &title)
{
    this->m_proTitle = title;
}

void PicProject::setProjectId(const int &id)
{
    this->m_proId = id;
}
