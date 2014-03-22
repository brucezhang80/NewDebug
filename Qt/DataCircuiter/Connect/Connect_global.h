#ifndef CONNECT_GLOBAL_H
#define CONNECT_GLOBAL_H

#include "Connect_global.h"
#include <QObject>

#if defined(CONNECT_LIBRARY)
#  define CONNECTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONNECTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CONNECT_GLOBAL_H
