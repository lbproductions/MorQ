#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>

class InformationProvider;

class Plugin : public QObject
{
    Q_OBJECT
public:
    explicit Plugin(const QString &name, QObject *parent = 0);
    ~Plugin();

    QString name() const;

private:
    QString m_name;
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
    static void registerPlugin(Plugin *plugin);
    static void init();

private:
    static QList<Plugin *> s_plugins;
    static QHash<QString, InformationProvider *> s_informationProviders;
};

#endif // PLUGIN_H
