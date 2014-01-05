#ifndef TOOLS_H
#define TOOLS_H

class QString;
class QWidget;

class Tools
{
public:
    Tools();

    static int levenshteinDistance(const QString &str1, const QString &str2);
    static void showInGraphicalShell(QWidget *parent, const QString &pathIn);
};

#endif // TOOLS_H
