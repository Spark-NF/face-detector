#-------------------------------------------------
#
# Project created by QtCreator 2014-10-19T16:43:04
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = FaceDetector
TEMPLATE = app

# OpenCV
win32 {
    INCLUDEPATH += "C:/Users/Nicolas/Downloads/opencv/build/include"
    LIBS += C:/Users/Nicolas/Downloads/opencv/build/x86/vc12/lib/opencv_contrib249d.lib \
            C:/Users/Nicolas/Downloads/opencv/build/x86/vc12/lib/opencv_core249d.lib \
            C:/Users/Nicolas/Downloads/opencv/build/x86/vc12/lib/opencv_features2d249d.lib \
            C:/Users/Nicolas/Downloads/opencv/build/x86/vc12/lib/opencv_highgui249d.lib \
            C:/Users/Nicolas/Downloads/opencv/build/x86/vc12/lib/opencv_imgproc249d.lib \
            C:/Users/Nicolas/Downloads/opencv/build/x86/vc12/lib/opencv_objdetect249d.lib \
            C:/Users/Nicolas/Downloads/opencv/build/x86/vc12/lib/opencv_video249d.lib
}
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

SOURCES += main.cpp \
        mainwindow.cpp \
    webcamdialog.cpp \
    cvqt.cpp \
    addpersondialog.cpp \
    addpicturedialog.cpp \
    person.cpp \
    aspectratiopixmaplabel.cpp

HEADERS  += mainwindow.h \
    webcamdialog.h \
    cvqt.h \
    addpersondialog.h \
    addpicturedialog.h \
    person.h \
    aspectratiopixmaplabel.h

FORMS    += mainwindow.ui \
    webcamdialog.ui \
    addpersondialog.ui \
    addpicturedialog.ui
