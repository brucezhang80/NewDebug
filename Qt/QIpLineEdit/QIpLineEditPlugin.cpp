/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/3 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "QIpLineEdit.h"
#include "QIpLineEditPlugin.h"

#include <QtPlugin>

QIpLineEditPlugin::QIpLineEditPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QIpLineEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QIpLineEditPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QIpLineEditPlugin::createWidget(QWidget *parent)
{
    return new QIpLineEdit(parent);
}

QString QIpLineEditPlugin::name() const
{
    return QLatin1String("QIpLineEdit");
}

QString QIpLineEditPlugin::group() const
{
    return QLatin1String("");
}

QIcon QIpLineEditPlugin::icon() const
{
    return QIcon(QLatin1String(":/network_ip.png"));
}

QString QIpLineEditPlugin::toolTip() const
{
    return QLatin1String("IP Address Edit");
}

QString QIpLineEditPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QIpLineEditPlugin::isContainer() const
{
    return false;
}

QString QIpLineEditPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QIpLineEdit\" name=\"qIpLineEdit\">\n</widget>\n");
}

QString QIpLineEditPlugin::includeFile() const
{
    return QLatin1String("QIpLineEdit.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(QIpLineEdit, QIpLineEditPlugin)
#endif // QT_VERSION < 0x050000
