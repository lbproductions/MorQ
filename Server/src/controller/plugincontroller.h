#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <QObject>

class HosterPlugin;
class DecrypterPlugin;
class DownloadProvider;
class RenamerPlugin;

class PluginController : public QObject
{
    Q_OBJECT
public:
    explicit PluginController(QObject *parent = 0);
    
    QList<HosterPlugin *> hosterPlugins() const;
    QList<DecrypterPlugin *> decrypterPlugins() const;
    QList<RenamerPlugin *> renamerPlugins() const;

    void registerHoster(HosterPlugin *hoster);
    void registerDecrypter(DecrypterPlugin *decrypter);
    void registerRenamerPlugin(RenamerPlugin *renamer);

private:
    QList<HosterPlugin *> m_hosterPlugins;
    QList<DecrypterPlugin *> m_decrypterPlugins;
    QList<RenamerPlugin *> m_renamerPlugins;

    void createHosterPlugins();
    void createDecrypterPlugins();
    void createRenamerPlugins();
};

#endif // PLUGINCONTROLLER_H
