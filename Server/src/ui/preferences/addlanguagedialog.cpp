#include "addlanguagedialog.h"
#include "ui_addlanguagedialog.h"

#include "preferences.h"
#include "model/series.h"

#include <QDebug>

AddLanguageDialog::AddLanguageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLanguageDialog)
{
    ui->setupUi(this);

    QList<QString> alreadyAddedLanguages;

    foreach (QLocale::Language language, Preferences::languages()) {
        QString name = QLocale::languageToString(language);
        alreadyAddedLanguages.append(name);

        QListWidgetItem *item = new QListWidgetItem(QIcon(Series::languageFlag(language)),
                                                    name,
                                                    ui->listWidgetLanguages);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        item->setData(LanguageDataRole, static_cast<int>(language));
        ui->listWidgetLanguages->addItem(item);
    }

    QList<QLocale> allLocales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);
    foreach (QLocale locale, allLocales) {
        QString name = QLocale::languageToString(locale.language());

        if (alreadyAddedLanguages.contains(name))
            continue;

        alreadyAddedLanguages.append(name);

        QListWidgetItem *item = new QListWidgetItem(QIcon(Series::languageFlag(locale.language())),
                                                    name,
                                                    ui->listWidgetLanguages);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setData(LanguageDataRole, static_cast<int>(locale.language()));
        ui->listWidgetLanguages->addItem(item);
    }

    setWindowModality(Qt::WindowModal);
}

AddLanguageDialog::~AddLanguageDialog()
{
    delete ui;
}

QList<QLocale::Language> AddLanguageDialog::checkedLanguages()
{
    QList<QLocale::Language> languages;
    int count = ui->listWidgetLanguages->count();
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item = ui->listWidgetLanguages->item(i);
        if (item->checkState() == Qt::Checked) {
            languages.append(static_cast<QLocale::Language>(item->data(LanguageDataRole).toInt()));
        }
    }
    return languages;
}
