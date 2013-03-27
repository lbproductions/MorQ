#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <QObject>

class HosterPlugin;
class DecrypterPlugin;
class DownloadProviderPlugin;
class RenamerPlugin;

class PluginController : public QObject
{
    Q_OBJECT
public:
    explicit PluginController(QObject *parent = 0);
    
    QList<HosterPlugin *> hosterPlugins() const;
    QList<DecrypterPlugin *> decrypterPlugins() const;
    QList<DownloadProviderPlugin *> downloadProviderPlugins() const;
    QList<RenamerPlugin *> renamerPlugins() const;

    DownloadProviderPlugin * downloadProviderPluginByName(QString name) const;

    void registerHoster(HosterPlugin *hoster);
    void registerDecrypter(DecrypterPlugin *decrypter);
    void registerDownloadProvider(DownloadProviderPlugin *provider);
    void registerRenamerPlugin(RenamerPlugin *renamer);

private:
    QList<HosterPlugin *> m_hosterPlugins;
    QList<DecrypterPlugin *> m_decrypterPlugins;
    QList<DownloadProviderPlugin *> m_downloadProviderPlugins;
    QList<RenamerPlugin *> m_renamerPlugins;

    void createHosterPlugins();
    void createDecrypterPlugins();
    void createDownloadProviderPlugins();
    void createRenamerPlugins();
};

#endif // PLUGINCONTROLLER_H
