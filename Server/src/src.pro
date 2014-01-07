LIBPATH = ../../lib/

QPERSISTENCE_PATH = $$LIBPATH/QPersistence
include($$QPERSISTENCE_PATH/QPersistence.pri)

QSERIENJUNKIES_PATH = $$LIBPATH/QSerienJunkies
include($$QSERIENJUNKIES_PATH/QSerienJunkies.pri)

QUUNRAR_PATH = $$LIBPATH/QuunRar
include($$QUUNRAR_PATH/QuunRar.pri)

### General config ###

TARGET          = MorQServer
VERSION         = 0.0.0
TEMPLATE        = app
QT              += sql widgets network xml concurrent
CONFIG          += c++11
QMAKE_CXXFLAGS  += $$QPERSISTENCE_COMMON_QMAKE_CXXFLAGS
DEFINES         += "VERSION=\"$$VERSION\""
LIBS            += -framework AppKit

### Qp ###

INCLUDEPATH     += $$QPERSISTENCE_INCLUDEPATH
LIBS            += $$QPERSISTENCE_LIBS


### QSerienJunkies ###

LIBS            += $$QSERIENJUNKIES_LIBS
INCLUDEPATH     += $$QSERIENJUNKIES_INCLUDEPATH


#### QuunRar ###

LIBS            += $$QUUNRAR_LIBS
INCLUDEPATH     += $$QUUNRAR_INCLUDEPATH


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
    ui/mainwindow/model/serieslistmodel.h \
    ui/mainwindow/serieslistitemdelegate.h \
    ui/mainwindow/model/seasonslistmodel.h \
    model/download.h \
    model/downloadpackage.h \
    model/episode.h \
    model/season.h \
    model/series.h \
    ui/mainwindow/model/episodeslistmodel.h \
    ui/dialogs/choosedownloadlinksdialog.h \
    ui/mainwindow/noisygradientitemdelegate.h \
    ui/mainwindow/seasonslistitemdelegate.h \
    ui/mainwindow/episodeslistitemdelegate.h \
    ui/dialogs/rescancollectiondialog.h \
    plugins/scraper/filescraper.h \
    plugins/informationProviders/thetvdbinformationprovider.h \
    ui/widgets/serieswidget.h \
    ui/dialogs/seriessearchresultitemdelegate.h \
    ui/preferences/addlanguagedialog.h \
    plugins/renamer/renamerplugin.h \
    plugins/renamer/classicrenamerandmoverplugin.h \
    plugins/scraper/scraper.h \
    plugins/scraper/newseriesscraper.h \
    controller/episodedownloadcontroller.h \
    controller/scrapercontroller.h \
    misc/tools.h \
    plugins/plugins.h \
    plugins/informationProviders/informationprovider.h \
    plugins/downloadProviders/downloadprovider.h \
    plugins/downloadProviders/serienjunkiesprovider.h \
    model/onlineresource.h \
    ui/mainwindow/statusbar.h \
    ui/widgets/label.h \
    ui/mainwindow/headerview.h

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
    ui/mainwindow/model/serieslistmodel.cpp \
    ui/mainwindow/serieslistitemdelegate.cpp \
    ui/mainwindow/model/seasonslistmodel.cpp \
    model/download.cpp \
    model/downloadpackage.cpp \
    model/series.cpp \
    model/episode.cpp \
    model/season.cpp \
    ui/mainwindow/model/episodeslistmodel.cpp \
    ui/dialogs/choosedownloadlinksdialog.cpp \
    ui/mainwindow/noisygradientitemdelegate.cpp \
    ui/mainwindow/seasonslistitemdelegate.cpp \
    ui/mainwindow/episodeslistitemdelegate.cpp \
    ui/dialogs/rescancollectiondialog.cpp \
    plugins/scraper/filescraper.cpp \
    plugins/informationProviders/thetvdbinformationprovider.cpp \
    ui/widgets/serieswidget.cpp \
    ui/dialogs/seriessearchresultitemdelegate.cpp \
    ui/preferences/addlanguagedialog.cpp \
    plugins/renamer/renamerplugin.cpp \
    plugins/renamer/classicrenamerandmoverplugin.cpp \
    plugins/scraper/scraper.cpp \
    plugins/scraper/newseriesscraper.cpp \
    controller/episodedownloadcontroller.cpp \
    controller/scrapercontroller.cpp \
    misc/tools.cpp \
    plugins/plugins.cpp \
    plugins/informationProviders/informationprovider.cpp \
    plugins/downloadProviders/downloadprovider.cpp \
    plugins/downloadProviders/serienjunkiesprovider.cpp \
    model/onlineresource.cpp \
    ui/mainwindow/statusbar.cpp \
    ui/widgets/label.cpp \
    ui/mainwindow/headerview.cpp

FORMS += \
    ui/mainwindow/mainwindow.ui \
    ui/preferences/preferenceswindow.ui \
    ui/dialogs/newserieswizard.ui \
    ui/dialogs/choosedownloadlinksdialog.ui \
    ui/dialogs/rescancollectiondialog.ui \
    ui/widgets/serieswidget.ui \
    ui/preferences/addlanguagedialog.ui

RESOURCES += \
    ui/resources/uiresources.qrc \
    ui/resources/flags.qrc

OBJECTIVE_SOURCES +=

