#ifndef INFORMATIONPROVIDERPLUGIN_H
#define INFORMATIONPROVIDERPLUGIN_H

#include <QObject>

class InformationProviderPlugin : public QObject
{
    Q_OBJECT
public:
    explicit InformationProviderPlugin(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // INFORMATIONPROVIDERPLUGIN_H
