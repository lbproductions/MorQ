#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QStringList>

class Preferences
{
public:
    static QString premiumizeMeUserName();
    static void setPremiumizeMeUserName(const QString &name);
    static QString premiumizeMeUserPassword();
    static void setPremiumizeMeUserPassword(const QString &password);

    static QString downloadFolder();
    static void setDownloadFolder(const QString &downloadFolder);

    static int maxDownloads();
    static void setMaxDownloads(int maxDownloads);

    static bool extractDownloads();
    static void setExtractDownloads(bool extract);
    static QString extractFolder();
    static void setExtractFolder(const QString &folder);
    static QString extractMode();
    static void setExtractMode(const QString &mode);

    static QStringList seriesLocations();
    static void setSeriesLocations(const QStringList &locations);
};

#endif // PREFERENCES_H
