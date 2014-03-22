#ifndef PICPROJECT_H
#define PICPROJECT_H

#include <QString>
#include <QStringList>

class PicProject
{
public:
    PicProject();
public:
    void setProjectUrl(const QString&);
    void setProjectPath(const QString&);
    void setProjectTitle(const QString&);
    void setProjectId(const int&);

    QString getProjectPath() const {return m_proPath;}
    QString getProjectTitle() const {return m_proTitle;}
    QString getProjectId() const {return m_proId;}
    QString getProjectUrl() const {return m_proUrl;}

    //int addImage(TheImage *);
    PicProject& operator =(PicProject *pp)
    {
        this->m_proId = pp->m_proId;
        this->m_proPath = pp->m_proPath;
        this->m_proTitle = pp->m_proTitle;
        this->m_proUrl = pp->m_proUrl;

        return *this;
    }

public:
    QString     m_proUrl;
    QString     m_proPath;
    QString     m_proTitle;
    int         m_proId;

    struct TheImage
    {
        QString     m_url;
        QString     m_title;
        QString     m_type;
        qint32      m_size;
        qint32      m_width;
        qint32      m_height;
    };

};

#endif // PICPROJECT_H
