#ifndef RENAMERPLUGIN_H
#define RENAMERPLUGIN_H

#include <QObject>

class Episode;

class RenamerPlugin : public QObject
{
    Q_OBJECT
public:
    explicit RenamerPlugin(QObject *parent = 0);

public slots:
    virtual void renameEpisodes(QList<QSharedPointer<Episode> > episodes) = 0;
    
signals:
    void finishedRenaming(QSharedPointer<Episode> episode);
    
public slots:
    
};

#endif // RENAMERPLUGIN_H
