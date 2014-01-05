#include "plugins.h"

#include "informationProviders/informationprovider.h"
#include "informationProviders/thetvdbinformationprovider.h"
#include "downloadProviders/downloadprovider.h"
#include "downloadProviders/serienjunkiesprovider.h"

Plugin::Plugin(const QString &name, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_taskCount(0)
{
}

Plugin::~Plugin()
{
}

QString Plugin::name() const
{
    return m_name;
}

void Plugin::increaseTaskCount()
{
    ++m_taskCount;
}

void Plugin::decreaseTaskCount()
{
    --m_taskCount;

    if(m_taskCount == 0) {
        emit finishedAllTasks();
    }
}

PluginTask::PluginTask(Plugin *parent) :
    QObject(parent)
{
    parent->increaseTaskCount();

    connect(this, &PluginTask::finished,
            parent, &Plugin::decreaseTaskCount);
    connect(this, &PluginTask::error,
            parent, &Plugin::decreaseTaskCount);
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
QHash<QString, DownloadProvider *> Plugins::s_downloadProviders;

InformationProvider *Plugins::informationProvider(const QString &name)
{
    return s_informationProviders.value(name);
}

QList<InformationProvider *> Plugins::informationProviders()
{
    return s_informationProviders.values();
}

DownloadProvider *Plugins::downloadProvider(const QString &name)
{
    return s_downloadProviders.value(name);
}

QList<DownloadProvider *> Plugins::downloadProviders()
{
    return s_downloadProviders.values();
}

void Plugins::registerPlugin(Plugin *plugin)
{
    s_plugins.append(plugin);
    if(plugin->metaObject()->superClass()->className() == InformationProvider::staticMetaObject.className()) {
        s_informationProviders.insert(plugin->name(), static_cast<InformationProvider *>(plugin));
    }
    else if(plugin->metaObject()->superClass()->className() == DownloadProvider::staticMetaObject.className()) {
        s_downloadProviders.insert(plugin->name(), static_cast<DownloadProvider *>(plugin));
    }
}

void Plugins::init()
{
    static QObject guard;
    registerPlugin(new TheTvdbInformationProvider(&guard));
    registerPlugin(new SerienjunkiesProvider(&guard));
}
