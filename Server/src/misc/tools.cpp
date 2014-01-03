#include "tools.h"

#include <QString>
#include <QVector>

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
