#include "plugins.h"

#include "informationProviders/informationprovider.h"
#include "informationProviders/thetvdbinformationprovider.h"

Plugin::Plugin(const QString &name, QObject *parent) :
    QObject(parent),
    m_name(name)
{
}

Plugin::~Plugin()
{
}

QString Plugin::name() const
{
    return m_name;
}

PluginTask::PluginTask(Plugin *parent) :
    QObject(parent)
{
}

PluginTask::~PluginTask()
{
}

QString PluginTask::statusMessage() const
{
    return m_statusMessage;
}

void PluginTask::setStatusMessage(const QString &statusMessage)
{
    m_statusMessage = statusMessage;
    emit statusMessageChanged(statusMessage);
}

QString PluginTask::errorMessage() const
{
    return m_errorMessage;
}

void PluginTask::setErrorMessage(const QString &errorMessage)
{
    m_errorMessage = errorMessage;
    emit error(errorMessage);
}


QList<Plugin *>  Plugins::s_plugins;
QHash<QString, InformationProvider *> Plugins::s_informationProviders;

InformationProvider *Plugins::informationProvider(const QString &name)
{
    return s_informationProviders.value(name);
}

QList<InformationProvider *> Plugins::informationProviders()
{
    return s_informationProviders.values();
}

void Plugins::registerPlugin(Plugin *plugin)
{
    s_plugins.append(plugin);
    if(plugin->metaObject()->superClass()->className() == InformationProvider::staticMetaObject.className()) {
        s_informationProviders.insert(plugin->name(), static_cast<InformationProvider *>(plugin));
    }
}

void Plugins::init()
{
    static QObject guard;
    registerPlugin(new TheTvdbInformationProvider(&guard));
}
