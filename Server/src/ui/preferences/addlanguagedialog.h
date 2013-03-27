#ifndef ADDLANGUAGEDIALOG_H
#define ADDLANGUAGEDIALOG_H

#include <QDialog>

#include <QLocale>

namespace Ui {
class AddLanguageDialog;
}

class AddLanguageDialog : public QDialog
{
    Q_OBJECT

    enum DataRoles {
        LanguageDataRole = Qt::UserRole + 1
    };

public:    
    explicit AddLanguageDialog(QWidget *parent = 0);
    ~AddLanguageDialog();
    
    QList<QLocale::Language> checkedLanguages();

private:
    Ui::AddLanguageDialog *ui;
};

#endif // ADDLANGUAGEDIALOG_H
