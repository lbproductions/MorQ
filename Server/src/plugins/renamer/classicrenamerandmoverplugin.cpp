#include "classicrenamerandmoverplugin.h"

#include <QFile>
#include <QDir>

#include <controller/controller.h>
#include <controller/extractioncontroller.h>
#include <plugins/scraper/filescraper.h>

#include <model/episode.h>
#include <model/season.h>
#include <model/downloadpackage.h>
#include <model/videodownloadlink.h>
#include <model/download.h>

#include "preferences.h"

ClassicRenamerAndMoverPlugin::ClassicRenamerAndMoverPlugin(QObject *parent):
    RenamerPlugin(parent)
{
    connect(Controller::extractor(), &ExtractionController::extractionFinished, this, &ClassicRenamerAndMoverPlugin::renameAndMoveEpisodeFromDownload);
}

void ClassicRenamerAndMoverPlugin::renameEpisodes(QList<Episode *> episodes)
{
    foreach(Episode *episode, episodes) {
        if(episode->videoFile() == "") {
            continue;
        }

        int seasonNumber = episode->seasonNumber();
        int episodeNumber = episode->number();
        QString title = episode->title();

        QFile file(episode->videoFile());
        QFileInfo info(file);

        QString folder;

        QStringList seasonPath = episode->season()->folders();
        qDebug() << "SeasonPath: " << seasonPath;
        QString episodeDir = info.dir().absolutePath();
        qDebug() << "EpsiodeDir: " << episodeDir;

        if(seasonPath.contains(episodeDir)) {
            folder = episodeDir;
        }
        else{
            folder = seasonPath.first();
        }

        QString newName = folder + "/S" + numberToString(seasonNumber) + "E" + numberToString(episodeNumber)
                + " - " + title + "." + info.suffix();

        qDebug() << "FileName: " << file.fileName();
        qDebug() << "Folder: " << folder;
        qDebug() << "Renaming " << file.fileName() << newName;
        file.rename(newName);

        episode->setVideoFile(newName);

        Qp::update(episode);
    }
}

void ClassicRenamerAndMoverPlugin::renameAndMoveEpisodeFromDownload(DownloadPackage *package)
{
    if(Preferences::extractMode() != "SERIES") {
        return;
    }

    qDebug() << "ExtractFolder: " << package->extractFolder();
    qDebug() << "PackageName: " << package->name();
    QFileInfo packageNameInfo(package->name());
    QString extractSubFolder = package->downloads().first()->fileName();
    extractSubFolder = extractSubFolder.remove(packageNameInfo.suffix());
    extractSubFolder = extractSubFolder.remove(extractSubFolder.size()-1, extractSubFolder.size());
    qDebug() << "Dir: " << package->extractFolder() + "/" + extractSubFolder;
    QDir dir(package->extractFolder() + "/" + extractSubFolder);
    QStringList videoFiles;
    foreach(QFileInfo info, dir.entryInfoList()) {
        if(FileScraper::VIDEOEXTENSIONS().contains(info.suffix())) {
            videoFiles.append(info.absoluteFilePath());
        }
    }
    if(videoFiles.size() != 1) {
        return;
        // TODO: Choose correct file if there are more than one video-file e.g. sampler
    }
    qDebug() << videoFiles;
    Episode* episode = package->videoDownloadLinks().first()->episode();

    episode->setVideoFile(videoFiles.first());

    renameEpisodes(QList<Episode*>() << episode);

    removeDir(package->extractFolder() + "/" + extractSubFolder);
}

QString ClassicRenamerAndMoverPlugin::numberToString(int number)
{
    if(number < 10) {
        return "0" + QString::number(number);
    }
    else{
        return QString::number(number);
    }
}

void ClassicRenamerAndMoverPlugin::removeDir(QString dirName)
{
    QDir dir(dirName);
    if (dir.exists(dirName)) {
            Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
                if (info.isDir()) {
                    removeDir(info.absoluteFilePath());
                }
                else {
                    QFile::remove(info.absoluteFilePath());
                }
            }
            dir.rmdir(dirName);
        }
}
