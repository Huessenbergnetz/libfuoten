#ifndef FUOTEN_GLOBAL_H
#define FUOTEN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FUOTEN_LIBRARY)
#  define GELTANSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GELTANSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FUOTEN_GLOBAL_H
