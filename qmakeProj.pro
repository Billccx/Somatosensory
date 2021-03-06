QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calibrator.cpp \
    capturer.cpp \
    main.cpp \
    mainwindow.cpp \
    posetracker.cpp \
    posetracker2.cpp

HEADERS += \
    calibrator.h \
    capturer.h \
    mainwindow.h \
    posetracker.h \
    posetracker2.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lapriltag

INCLUDEPATH += $$PWD/../../../../../usr/local/include/apriltag
DEPENDPATH += $$PWD/../../../../../usr/local/include/apriltag

INCLUDEPATH+=/usr/include/eigen3

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lceres

INCLUDEPATH += $$PWD/../../../../../usr/local/include/ceres
DEPENDPATH += $$PWD/../../../../../usr/local/include/ceres

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_core

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_calib3d

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_features2d

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_highgui

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lrealsense2

INCLUDEPATH += $$PWD/../../../../../usr/local/include/librealsense2
DEPENDPATH += $$PWD/../../../../../usr/local/include/librealsense2

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lrealsense2-gl

INCLUDEPATH += $$PWD/../../../../../usr/local/include/librealsense2-gl
DEPENDPATH += $$PWD/../../../../../usr/local/include/librealsense2-gl

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../??????/myLib/lib/ -lexample

INCLUDEPATH += $$PWD/../../../??????/myLib/include
DEPENDPATH += $$PWD/../../../??????/myLib/include

unix:!macx: LIBS += -L$$PWD/../../../??????/poselib/lib/ -lpose

INCLUDEPATH += $$PWD/../../../??????/poselib/include
DEPENDPATH += $$PWD/../../../??????/poselib/include


unix:!macx: LIBS += -L$$PWD/../../../??????/poselib/lib/ -labsl

INCLUDEPATH += $$PWD/../../../../../usr/local/include/absl
DEPENDPATH += $$PWD/../../../../../usr/local/include/absl

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../??????/poselib/lib/libabsl.a

INCLUDEPATH += /usr/include/python3.8


