TEMPLATE = lib
CONFIG += staticlib c++17   # 静态库
QT += core
DEFINES += BUILD_AI_LEARNING_DLL

HEADERS += include/ai_learning_lib.h
SOURCES += src/base_ai.cpp   # 确保路径正确

DESTDIR = $$PWD/../../bin       # 输出到 bin 目录
