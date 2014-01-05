#include "tools.h"

#include <QString>
#include <QVector>
#include <QStringList>
#include <QProcess>

Tools::Tools()
{
}

int Tools::levenshteinDistance(const QString &str1, const QString &str2)
{
    // This has been copied from https://gitorious.org/levenshtein
    // License is LGPL2! Let's keep that in mind, for when we ever want to change our license

    int len1, len2;

    QString str1u = str1.toUpper();
    QString str2u = str2.toUpper();

    len1 = str1u.size();
    len2 = str2u.size();

    QVector<QVector<int> > d(len1+1, QVector<int>(len2+1));

    d[0][0] = 0;

    for (int i=1; i <= len1; ++i) {
        d[i][0] = i;
    }
    for (int j=1; j <= len2; ++j) {
        d[0][j] = j;
    }

    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            d[i][j] = qMin(qMin(d[i-1][j] + 1, d[i][j-1] + 1), d[i-1][j-1] + (str1u.data()[i-1] == str2u.data()[j-1] ? 0 : 1));
        }
    }

    return d[len1][len2];
}

// This is from http://stackoverflow.com/questions/3490336/how-to-reveal-in-finder-or-show-in-explorer-with-qt
// The code is from Qt Creator!
void Tools::showInGraphicalShell(QWidget *parent, const QString &pathIn)
{
    // Mac, Windows support folder or file.
#if defined(Q_OS_WIN)
    const QString explorer = Environment::systemEnvironment().searchInPath(QLatin1String("explorer.exe"));
    if (explorer.isEmpty()) {
        QMessageBox::warning(parent,
                             tr("Launching Windows Explorer failed"),
                             tr("Could not find explorer.exe in path to launch Windows Explorer."));
        return;
    }
    QString param;
    if (!QFileInfo(pathIn).isDir())
        param = QLatin1String("/select,");
    param += QDir::toNativeSeparators(pathIn);
    QProcess::startDetached(explorer, QStringList(param));
#elif defined(Q_OS_MAC)
    Q_UNUSED(parent)
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                                     .arg(pathIn);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute("/usr/bin/osascript", scriptArgs);
#else
    // we cannot select a file here, because no file browser really supports it...
    const QFileInfo fileInfo(pathIn);
    const QString folder = fileInfo.absoluteFilePath();
    const QString app = Utils::UnixUtils::fileBrowser(Core::ICore::instance()->settings());
    QProcess browserProc;
    const QString browserArgs = Utils::UnixUtils::substituteFileBrowserParameters(app, folder);
    if (debug)
        qDebug() <<  browserArgs;
    bool success = browserProc.startDetached(browserArgs);
    const QString error = QString::fromLocal8Bit(browserProc.readAllStandardError());
    success = success && error.isEmpty();
    if (!success)
        showGraphicalShellError(parent, app, error);
#endif
}
