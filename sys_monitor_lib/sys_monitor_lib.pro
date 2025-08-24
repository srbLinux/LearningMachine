TEMPLATE = lib  
CONFIG += shared c++17  
QT += core
DEFINES += BUILD_SYS_MONITOR_DLL
HEADERS += include/sys_monitor.h  
SOURCES += src/cpu_monitor.cpp src/memory_monitor.cpp  
DESTDIR = $$PWD/../bin   # 输出到统一bin目录[6,7](@ref)
