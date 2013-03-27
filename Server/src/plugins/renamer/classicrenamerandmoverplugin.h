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
public:
    ClassicRenamerAndMoverPlugin(QObject *parent = 0);

public slots:
    virtual void renameEpisodes(QList<Episode*> episodes);

    void renameAndMoveEpisodeFromDownload(DownloadPackage *package);

private:
    QString numberToString(int number);

    void removeDir(QString dirName);
};

#endif // CLASSICRENAMERANDMOVERPLUGIN_H
