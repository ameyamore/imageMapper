TEMPLATE = app

QT += location

SOURCES += main.cpp
SOURCES += controllers/metadataparser.cpp
SOURCES += models/imagemodel.cpp

RESOURCES += qml.qrc


HEADERS += controllers/metadataparser.h \
    commonIncludes.h
HEADERS += models/imagemodel.h


LIBS += -L$$PWD/exiv2/lib/ -lexiv2
INCLUDEPATH += $$PWD//exiv2/include/
