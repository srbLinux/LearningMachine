TEMPLATE = lib  
CONFIG += shared c++17  
QT += core
DEFINES += BUILD_SYS_MONITOR_DLL
HEADERS += \
    include/ai_learning_lib.h
SOURCES += \
    src/base_ai.cpp
DESTDIR = $$PWD/../bin   # 输出到统一bin目录[6,7](@ref)
