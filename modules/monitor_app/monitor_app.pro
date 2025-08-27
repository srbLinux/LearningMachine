TEMPLATE = app  
QT += core widgets network
# 包含库头文件
INCLUDEPATH += \
    $$PWD/../sys_monitor_lib/include \
    $$PWD/../ai_learning_lib/include

SOURCES += \
    loginwidget.cpp \
    main.cpp \
    userinfo.cpp

DEFINES += \
    LM_DEBUG LM_AUTHOR_SRB

LIBS += -L$$PWD/../../bin -lsys_monitor_lib -lai_learning_lib          # 链接动态库[1,6](@ref)

HEADERS += \
    lmdef.h \
    loginwidget.h \
    ui_tools.h \
    userinfo.h
