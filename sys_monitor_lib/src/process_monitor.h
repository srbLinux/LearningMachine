#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <QtCore>

namespace LM::SysMonitor {

struct ProcessInfo
{
    size_t pid;
    QString name;
    time_t runTime;
    time_t startTime;
};

class ProcessMonitor
{
public:
    explicit ProcessMonitor();

    QMap<QString, time_t> getAllProcessesTime();
private:
    static QMap<QString, ProcessInfo> processInfo;
};

}

#endif // PROCESS_MONITOR_H
