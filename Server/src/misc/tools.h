#ifndef TOOLS_H
#define TOOLS_H

class QString;
class QWidget;
class QPixmap;
class QDate;

class Tools
{
public:
    static QPixmap cachedPixmap(const QString &resource);
    static int levenshteinDistance(const QString &str1, const QString &str2);
    static void showInGraphicalShell(QWidget *parent, const QString &pathIn);
    static QString airsInString(const QDate &date);
};

#endif // TOOLS_H
