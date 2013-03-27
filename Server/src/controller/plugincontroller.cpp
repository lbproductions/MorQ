#include "plugincontroller.h"

#include "preferences.h"
#include "plugins/hoster/hosterplugin.h"
#include "plugins/hoster/premiumizemeplugin.h"
#include "plugins/decrypter/decrypterplugin.h"
#include "plugins/decrypter/serienjunkiesdecrypterplugin.h"
#include "plugins/downloadProviders/downloadproviderplugin.h"
#include "plugins/downloadProviders/serienjunkiesproviderplugin.h"
#include "plugins/renamer/renamerplugin.h"
#include "plugins/renamer/classicrenamerandmoverplugin.h"


PluginController::PluginController(QObject *parent) :
    QObject(parent)
{
    createHosterPlugins();
    createDecrypterPlugins();
    createDownloadProviderPlugins();
    createRenamerPlugins();
}

QList<HosterPlugin *> PluginController::hosterPlugins() const
{
    return m_hosterPlugins;
}

QList<DecrypterPlugin *> PluginController::decrypterPlugins() const
{
    return m_decrypterPlugins;
}

QList<DownloadProviderPlugin *> PluginController::downloadProviderPlugins() const
{
    return m_downloadProviderPlugins;
}

QList<RenamerPlugin *> PluginController::renamerPlugins() const
{
    return m_renamerPlugins;
}

DownloadProviderPlugin *PluginController::downloadProviderPluginByName(QString name) const
{
    foreach(DownloadProviderPlugin* plugin, m_downloadProviderPlugins) {
        if(plugin->name() == name) {
            return plugin;
        }
    }

    return nullptr;
}

void PluginController::registerHoster(HosterPlugin *hoster)
{
    m_hosterPlugins.append(hoster);
}

void PluginController::registerDecrypter(DecrypterPlugin *decrypter)
{
    m_decrypterPlugins.append(decrypter);
}

void PluginController::registerDownloadProvider(DownloadProviderPlugin *provider)
{
    m_downloadProviderPlugins.append(provider);
}

void PluginController::registerRenamerPlugin(RenamerPlugin *renamer)
{
    m_renamerPlugins.append(renamer);
}

void PluginController::createHosterPlugins()
{
    HosterPlugin *plugin = new PremuimizeMePlugin(this);
    HosterPlugin::Account account;
    account.userName = Preferences::premiumizeMeUserName();
    account.password = Preferences::premiumizeMeUserPassword();
    plugin->setAccount(account);

    registerHoster(plugin);
}

void PluginController::createDecrypterPlugins()
{
    SerienJunkiesDecrypterPlugin *plugin = new SerienJunkiesDecrypterPlugin(this);
    registerDecrypter(plugin);
}

void PluginController::createDownloadProviderPlugins()
{
    DownloadProviderPlugin *provider = new SerienjunkiesProviderPlugin(this);
    registerDownloadProvider(provider);
}

void PluginController::createRenamerPlugins()
{
    ClassicRenamerAndMoverPlugin *renamer = new ClassicRenamerAndMoverPlugin(this);
    registerRenamerPlugin(renamer);
}
