#ifndef RENAMERPLUGIN_H
#define RENAMERPLUGIN_H

#include <QObject>

class Episode;

class RenamerPlugin : public QObject
{
    Q_OBJECT
public:
    explicit RenamerPlugin(QObject *parent = 0);

    virtual void renameEpisodes(QList<Episode*> episodes) = 0;
    
signals:
    void finishedRenaming(Episode *episode);
    
public slots:
    
};

#endif // RENAMERPLUGIN_H
