QT += core gui widgets positioning
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Frontend.cpp \
    main.cpp \
    pugixml.cpp

HEADERS += \
    mainwindow(2).h \
    pugiconfig.hpp \
    pugixml.hpp

FORMS += \
    mainwindow(2).ui
    OTHER_FILES += \
        BackEnd.cpp \
        Saigon.osm
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Saigon.osm
