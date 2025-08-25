#include "process_monitor.h"

#if __linux__
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#endif

#include <QtCore>

using LM::SysMonitor::ProcessInfo;
using LM::SysMonitor::ProcessMonitor;

QMap<QString, ProcessInfo> ProcessMonitor::processInfo = {};

#if __linux__

// 获取系统启动时间 (btime)
static long getBootTime()
{
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return -1;
    char buf[256];
    long btime = -1;
    while (fgets(buf, sizeof(buf), fp)) {
        if (sscanf(buf, "btime %ld", &btime) == 1) {
            break;
        }
    }
    fclose(fp);
    return btime;
}

// 获取进程启动时间（Unix时间戳）
static long getProcessStartTime(pid_t pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *fp = fopen(path, "r");
    if (!fp) return -1;

    int dummyPid;
    char comm[256];
    char state;
    unsigned long long starttime = 0;
    // 读取前3个字段：pid (comm) state
    if (fscanf(fp, "%d (%[^)]) %c", &dummyPid, comm, &state) != 3) {
        fclose(fp);
        return -1;
    }
    // 跳过到第22字段
    char buf[4096];
    for (int i = 0; i < 19; i++) fscanf(fp, "%s", buf);
    fscanf(fp, "%llu", &starttime);
    fclose(fp);

    long clk_tck = sysconf(_SC_CLK_TCK);
    long btime = getBootTime();
    if (btime < 0) return -1;

    return btime + starttime / clk_tck;
}

// 获取进程名
static QString getProcessName(pid_t pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/comm", pid);

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    QByteArray name = file.readLine().trimmed();
    return QString::fromUtf8(name);
}

QMap<QString, time_t> ProcessMonitor::getAllProcessesTime()
{
    QMap<QString, time_t> processTime;

    QDir procDir("/proc");
    QStringList entries = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    time_t now = time(NULL);

    for (const QString &entry : entries) {
        bool ok = false;
        pid_t pid = entry.toInt(&ok);
        if (!ok) continue;  // 非数字目录跳过

        QString name = getProcessName(pid);
        if (name.isEmpty()) continue;

        long start = getProcessStartTime(pid);
        if (start < 0) continue;

        ProcessInfo info;
        info.pid = pid;
        info.name = name;
        info.startTime = start;
        info.runTime = now - start;

        processInfo[entry] = info;
        processTime[name] = info.runTime;
    }

    return processTime;
}
#elif _WIN32
QMap<QString, time_t> ProcessMonitor::getAllProcessesTime()
{
    // Windows 实现可用 ToolHelp32Snapshot / QueryFullProcessImageName
    return {};
}
#endif

