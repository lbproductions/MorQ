#ifndef TOOLS_H
#define TOOLS_H

class QString;

class Tools
{
public:
    Tools();

    static int levenshteinDistance(const QString &str1, const QString &str2);
};

#endif // TOOLS_H
