TEMPLATE = app

QT += location

SOURCES += main.cpp \
    imagemodel.cpp \
    metadataparser.cpp

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/location/minimal_map
INSTALLS += target

LIBS += -L$$PWD/exiv2/lib/ -lexiv2
INCLUDEPATH += $$PWD//exiv2/include/

HEADERS += metadataparser.h \
    imagemodel.h
