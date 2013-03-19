LIBPATH = ../../lib/

QPERSISTENCE_PATH = $$LIBPATH/QPersistence
include($$QPERSISTENCE_PATH/QPersistence.pri)

#QRESTSERVER_PATH = $$QPERSISTENCE_BASEPATH/QRestServer
#include($$QRESTSERVER_PATH/QRestServer.pri)

#QHAL_PATH = $$QRESTSERVER_PATH/lib/QHal
#include($$QHAL_PATH/QHal.pri)

QSERIENJUNKIES_PATH = $$LIBPATH/QSerienJunkies
include($$QSERIENJUNKIES_PATH/QSerienJunkies.pri)

QUUNRAR_PATH = $$LIBPATH/QuunRar
include($$QUUNRAR_PATH/QuunRar.pri)

### General config ###

TARGET          = MorQServer
VERSION         = 0.0.0
TEMPLATE        = app
QT              += sql widgets network xml
CONFIG          += c++11
QMAKE_CXXFLAGS  += $$QPERSISTENCE_COMMON_QMAKE_CXXFLAGS
DEFINES         += "VERSION=\"$$VERSION\""
LIBS            += -framework AppKit

### QPersistence ###

INCLUDEPATH     += $$QPERSISTENCE_INCLUDEPATH
LIBS            += $$QPERSISTENCE_LIBS


### QSerienJunkies ###

LIBS            += $$QSERIENJUNKIES_LIBS
INCLUDEPATH     += $$QSERIENJUNKIES_INCLUDEPATH


#### QuunRar ###

LIBS            += $$QUUNRAR_LIBS
INCLUDEPATH     += $$QUUNRAR_INCLUDEPATH


#### QHttpServer ###

#INCLUDEPATH     += $$QHTTPSERVER_INCLUDEPATH
#LIBS            += $$QHTTPSERVER_LIBS


#### QHal ###

#LIBS            += $$QHAL_LIBS
#INCLUDEPATH     += $$QHAL_INCLUDEPATH


### Files ###

HEADERS += \
    ui/mainwindow/mainwindow.h \
    controller/downloadcontroller.h \
    ui/mainwindow/model/downloadsitemmodel.h \
    controller/controller.h \
    ui/preferences/preferenceswindow.h \
    preferences.h \
    controller/plugincontroller.h \
    plugins/hoster/hosterplugin.h \
    plugins/hoster/premiumizemeplugin.h \
    controller/downloader.h \
    controller/linkscontroller.h \
    plugins/decrypter/decrypterplugin.h \
    plugins/decrypter/serienjunkiesdecrypterplugin.h \
    ui/mainwindow/downloadsitemdelegate.h \
    controller/extractioncontroller.h \
    ui/dialogs/newserieswizard.h \
    plugins/downloadProviders/downloadproviderplugin.h \
    plugins/downloadProviders/serienjunkiesproviderplugin.h \
    ui/mainwindow/model/serieslistmodel.h \
    ui/mainwindow/serieslistitemdelegate.h \
    ui/mainwindow/model/seasonslistmodel.h \
    model/download.h \
    model/downloadpackage.h \
    model/videodownloadlink.h \
    model/episode.h \
    model/season.h \
    model/series.h \
    ui/mainwindow/model/episodeslistmodel.h \
    ui/dialogs/choosedownloadlinksdialog.h \
    ui/mainwindow/model/objectlistmodel.h \
    plugins/informationProviders/informationproviderplugin.h \
    ui/mainwindow/noisygradientitemdelegate.h \
    ui/mainwindow/seasonslistitemdelegate.h \
    ui/mainwindow/episodeslistitemdelegate.h \
    model/seriesdataaccessobject.h \
    ui/dialogs/rescancollectiondialog.h \
    controller/filescraper.h

SOURCES += main.cpp \
    ui/mainwindow/mainwindow.cpp \
    controller/downloadcontroller.cpp \
    ui/mainwindow/model/downloadsitemmodel.cpp \
    controller/controller.cpp \
    ui/preferences/preferenceswindow.cpp \
    preferences.cpp \
    controller/plugincontroller.cpp \
    plugins/hoster/hosterplugin.cpp \
    plugins/hoster/premiumizemeplugin.cpp \
    controller/downloader.cpp \
    controller/linkscontroller.cpp \
    plugins/decrypter/decrypterplugin.cpp \
    plugins/decrypter/serienjunkiesdecrypterplugin.cpp \
    ui/mainwindow/downloadsitemdelegate.cpp \
    controller/extractioncontroller.cpp \
    ui/dialogs/newserieswizard.cpp \
    plugins/downloadProviders/downloadproviderplugin.cpp \
    plugins/downloadProviders/serienjunkiesproviderplugin.cpp \
    ui/mainwindow/model/serieslistmodel.cpp \
    ui/mainwindow/serieslistitemdelegate.cpp \
    ui/mainwindow/model/seasonslistmodel.cpp \
    model/download.cpp \
    model/downloadpackage.cpp \
    model/videodownloadlink.cpp \
    model/series.cpp \
    model/episode.cpp \
    model/season.cpp \
    ui/mainwindow/model/episodeslistmodel.cpp \
    ui/dialogs/choosedownloadlinksdialog.cpp \
    ui/mainwindow/model/objectlistmodel.cpp \
    plugins/informationProviders/informationproviderplugin.cpp \
    ui/mainwindow/noisygradientitemdelegate.cpp \
    ui/mainwindow/seasonslistitemdelegate.cpp \
    ui/mainwindow/episodeslistitemdelegate.cpp \
    model/seriesdataaccessobject.cpp \
    ui/dialogs/rescancollectiondialog.cpp \
    controller/filescraper.cpp

FORMS += \
    ui/mainwindow/mainwindow.ui \
    ui/preferences/preferenceswindow.ui \
    ui/dialogs/newserieswizard.ui \
    ui/dialogs/choosedownloadlinksdialog.ui \
    ui/dialogs/rescancollectiondialog.ui

RESOURCES += \
    ui/resources/uiresources.qrc

OBJECTIVE_SOURCES +=

