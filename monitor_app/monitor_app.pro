TEMPLATE = app  
QT += core widgets  
INCLUDEPATH += $$PWD/../sys_monitor_lib/include  # 包含库头文件  
LIBS += -L$$PWD/../bin -lsys_monitor_lib          # 链接动态库[1,6](@ref)
