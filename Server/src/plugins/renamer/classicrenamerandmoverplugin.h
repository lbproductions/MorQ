#ifndef CLASSICRENAMERANDMOVERPLUGIN_H
#define CLASSICRENAMERANDMOVERPLUGIN_H

#include "renamerplugin.h"

class DownloadPackage;

class QDir;

/*
 *  Renames episodes to classic structure S01E01 - Episodename and moves it to
 *  corresponding season- or series-folder
 */
class ClassicRenamerAndMoverPlugin : public RenamerPlugin
{
    Q_OBJECT
public:
    ClassicRenamerAndMoverPlugin(QObject *parent = 0);

public slots:
    virtual void renameEpisodes(QList<QSharedPointer<Episode> > episodes);

    void renameAndMoveEpisodeFromDownload(QSharedPointer<DownloadPackage> package);

private:
    QString numberToString(int number);

    void removeDir(QString dirName);

    QList<QDir> subDirectories(QDir dir);
};

#endif // CLASSICRENAMERANDMOVERPLUGIN_H
