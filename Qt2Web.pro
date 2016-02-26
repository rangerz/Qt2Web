# Using Qt5
QT += core gui widgets websockets

TARGET = Qt2Web
TEMPLATE = app

SOURCES += \
    main.cpp \
    WidgetStreamer.cpp \
    RenderThread.cpp \
    RenderServer.cpp

HEADERS += \
    WidgetStreamer.h \
    RenderThread.h \
    RenderServer.h

OTHER_FILES = test.html
