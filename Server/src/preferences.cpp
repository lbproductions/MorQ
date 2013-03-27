#include "preferences.h"

#include <QSettings>

static const QString PREMIUMIZEME_USERNAME("hosters/premiumizeme/username");
static const QString PREMIUMIZEME_PASSWORD("hosters/premiumizeme/password");

static const QString EXTRACTDOWNLOADS("downloads/extract");
static const QString EXTRACTMODE("FOLDER");
static const QString EXTRACTFOLDER("downloads/extractFolder");
static const QString DOWNLOADFOLDER("downloads/folder");
static const QString DOWNLOADSMAX("downloads/max");

static const QString SERIESLOCATIONS("series/locations");
static const QString TVDB_LANGUAGES("tvdb/languages");

QString Preferences::premiumizeMeUserName()
{
    QSettings settings;
    return settings.value(PREMIUMIZEME_USERNAME).toString();
}

void Preferences::setPremiumizeMeUserName(const QString &name)
{
    QSettings settings;
    settings.setValue(PREMIUMIZEME_USERNAME, name);
}

QString Preferences::premiumizeMeUserPassword()
{
    QSettings settings;
    return settings.value(PREMIUMIZEME_PASSWORD).toString();
}

void Preferences::setPremiumizeMeUserPassword(const QString &password)
{
    QSettings settings;
    settings.setValue(PREMIUMIZEME_PASSWORD, password);
}

bool Preferences::extractDownloads()
{
    QSettings settings;
    return settings.value(EXTRACTDOWNLOADS).toBool();
}

void Preferences::setExtractDownloads(bool extract)
{
    QSettings settings;
    settings.setValue(EXTRACTDOWNLOADS, extract);
}

QString Preferences::downloadFolder()
{
    QSettings settings;
    return settings.value(DOWNLOADFOLDER).toString();
}

void Preferences::setDownloadFolder(const QString &downloadFolder)
{
    QSettings settings;
    settings.setValue(DOWNLOADFOLDER, downloadFolder);
}

int Preferences::maxDownloads()
{
    QSettings settings;
    return settings.value(DOWNLOADSMAX, 2).toInt();
}

void Preferences::setMaxDownloads(int maxDownloads)
{
    QSettings settings;
    settings.setValue(DOWNLOADSMAX, maxDownloads);
}

QString Preferences::extractFolder()
{
    QSettings settings;
    return settings.value(EXTRACTFOLDER).toString();
}

void Preferences::setExtractFolder(const QString &folder)
{
    QSettings settings;
    settings.setValue(EXTRACTFOLDER, folder);
}

QString Preferences::extractMode()
{
    QSettings settings;
    return settings.value(EXTRACTMODE).toString();
}

void Preferences::setExtractMode(const QString &mode)
{
    QSettings settings;
    settings.setValue(EXTRACTMODE, mode);
}

QStringList Preferences::seriesLocations()
{
    QSettings settings;
    return settings.value(SERIESLOCATIONS).toStringList();
}

void Preferences::setSeriesLocations(const QStringList &locations)
{
    QSettings settings;
    settings.setValue(SERIESLOCATIONS, locations);
}

QList<QLocale::Language> Preferences::languages()
{
    QList<QLocale::Language> result;
    QSettings settings;
    QStringList list = settings.value(TVDB_LANGUAGES).toStringList();
    foreach(QString string, list) {
        result.append(static_cast<QLocale::Language>(string.toInt()));
    }
    return result;
}

void Preferences::setLanguages(const QList<QLocale::Language> &languages)
{
    QSettings settings;
    QStringList result;
    foreach(QLocale::Language language, languages) {
        result.append(QString::number(language));
    }
    settings.setValue(TVDB_LANGUAGES, result);
}
