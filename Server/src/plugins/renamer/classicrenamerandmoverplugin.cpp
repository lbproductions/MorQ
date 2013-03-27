#include "classicrenamerandmoverplugin.h"

#include <QFile>
#include <QDir>

#include <model/episode.h>
#include <model/season.h>

ClassicRenamerAndMoverPlugin::ClassicRenamerAndMoverPlugin(QObject *parent)
{
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


        qDebug() << "FileName: " << file.fileName();
        qDebug() << "Folder: " << folder;
        qDebug() << "Renaming " << file.fileName() << folder + "/S" + numberToString(seasonNumber) + "E" + numberToString(episodeNumber)
                    + " - " + title + "." + info.suffix();
        file.rename(folder + "/S" + numberToString(seasonNumber) + "E" + numberToString(episodeNumber)
                    + " - " + title + "." + info.suffix());
    }
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
