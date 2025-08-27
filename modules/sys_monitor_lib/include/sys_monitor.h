#ifndef SYS_MONITOR_H
#define SYS_MONITOR_H

#include <QtCore>

#ifdef BUILD_SYS_MONITOR_DLL
 #define SYS_MONITOR_API Q_DECL_EXPORT
#else
 #define SYS_MONITOR_API Q_DECL_IMPORT
#endif

namespace LM
{

class SYS_MONITOR_API SystemMonitor : public QObject
{
public:
    explicit SystemMonitor(QObject *parent=NULL);
    time_t getSystemOpenTime();
    QMap<QString, time_t> getAllProcessTime();
};

}

#endif
