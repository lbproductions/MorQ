#ifndef CLASSICRENAMERANDMOVERPLUGIN_H
#define CLASSICRENAMERANDMOVERPLUGIN_H

#include "renamerplugin.h"

/*
 *  Renames episodes to classic structure S01E01 - Episodename and moves it to
 *  corresponding season- or series-folder
 */
class ClassicRenamerAndMoverPlugin : public RenamerPlugin
{
public:
    ClassicRenamerAndMoverPlugin(QObject *parent = 0);

    virtual void renameEpisodes(QList<Episode*> episodes);

    QString numberToString(int number);
};

#endif // CLASSICRENAMERANDMOVERPLUGIN_H
