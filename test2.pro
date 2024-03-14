QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    hikcamera.cpp

HEADERS += \
        mainwindow.h \
    hikcamera.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS += "I:/My_lib/opencv/build/x64/vc15/lib/opencv_world3414d.lib"
INCLUDEPATH += I:\My_lib\opencv\build\include\
                I:\My_lib\opencv\build\include\opencv\
                I:\My_lib\opencv\build\include\opencv2\

LIBS += $$PWD/lib/HCNetSDK.lib
LIBS += $$PWD/lib/PlayCtrl.lib
LIBS += $$PWD/lib/HCCore.lib
LIBS += $$PWD/lib/GdiPlus.lib
LIBS += $$PWD/lib/HCNetSDKCom/HCAlarm.lib
LIBS += $$PWD/lib/HCNetSDKCom/HCGeneralCfgMgr.lib
LIBS += $$PWD/lib/HCNetSDKCom/HCPreview.lib

INCLUDEPATH += $$PWD\include
DEPENDPATH += $$PWD\include

win32: LIBS += -L$$PWD/lib/ -lHCCore
win32: LIBS += -L$$PWD/lib/ -lHCNetSDK
win32: LIBS += -L$$PWD/lib/ -lPlayCtrl
win32: LIBS += -L$$PWD/lib/ -lGdiPlus
win32: LIBS += -L$$PWD/lib/HCNetSDKCom/ -lHCAlarm
win32: LIBS += -L$$PWD/lib/HCNetSDKCom/ -lHCGeneralCfgMgr
win32: LIBS += -L$$PWD/lib/HCNetSDKCom/ -lHCPreview


INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include



win32: LIBS += -LG:/ONNXruntime/onnxruntime-win-x64-gpu-1.14.1/lib/ -lonnxruntime

INCLUDEPATH += G:/ONNXruntime/onnxruntime-win-x64-gpu-1.14.1/include
DEPENDPATH += G:/ONNXruntime/onnxruntime-win-x64-gpu-1.14.1/include
