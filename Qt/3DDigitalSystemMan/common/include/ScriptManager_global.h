#ifndef SCRIPTMANAGER_GLOBAL_H
#define SCRIPTMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SCRIPTMANAGER_LIBRARY)
#  define SCRIPTMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SCRIPTMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SCRIPTMANAGER_GLOBAL_H
