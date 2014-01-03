#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>

class InformationProvider;
class DownloadProvider;

class Plugin : public QObject
{
    Q_OBJECT
public:
    explicit Plugin(const QString &name, QObject *parent = 0);
    ~Plugin();

    QString name() const;

signals:
    void finishedAllTasks();

private slots:
    friend class PluginTask;
    void increaseTaskCount();
    void decreaseTaskCount();

private:
    QString m_name;
    int m_taskCount;
};

class PluginTask : public QObject
{
    Q_OBJECT
public:
    QString statusMessage() const;
    QString errorMessage() const;

signals:
    void finished();
    void statusMessageChanged(QString);
    void error(QString);

protected:
    explicit PluginTask(Plugin *parent = 0);
    ~PluginTask();

protected slots:
    void setErrorMessage(const QString &errorMessage);
    void setStatusMessage(const QString &statusMessage);

private:
    QString m_statusMessage;
    QString m_errorMessage;
};

class Plugins {
public:
    static InformationProvider *informationProvider(const QString &name);
    static QList<InformationProvider *> informationProviders();
    static DownloadProvider *downloadProvider(const QString &name);
    static QList<DownloadProvider *> downloadProviders();
    static void registerPlugin(Plugin *plugin);
    static void init();

private:
    static QList<Plugin *> s_plugins;
    static QHash<QString, InformationProvider *> s_informationProviders;
    static QHash<QString, DownloadProvider *> s_downloadProviders;
};

#endif // PLUGIN_H
