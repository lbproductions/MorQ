#include "rescancollectiondialog.h"
#include "ui_rescancollectiondialog.h"

#include "controller/filescraper.h"

#include <QPushButton>

RescanCollectionDialog::RescanCollectionDialog(QWidget *parent) :
    QDialog(parent),
    m_scraper(new FileScraper(this)),
    ui(new Ui::RescanCollectionDialog)
{
    ui->setupUi(this);

    connect(m_scraper, &FileScraper::enteredFolder,
            this, &RescanCollectionDialog::setCurrentFolder);

    connect(m_scraper, &FileScraper::finished, [=]() {
        ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(true);
    });

    ui->buttonBox->button(QDialogButtonBox::Abort)->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(false);

    setWindowModality(Qt::ApplicationModal);
}

RescanCollectionDialog::~RescanCollectionDialog()
{
    delete ui;
}

void RescanCollectionDialog::scan()
{
    m_scraper->scanAllLocationsForShowsSeasonsAndEpisodesAsync();
}

void RescanCollectionDialog::setCurrentFolder(const QString &folder)
{
    ui->textEdit->append(folder);
    ui->labelStatus->setText(tr("Scanning folder %1 of %2 in location %3 of %4...")
                             .arg(m_scraper->currentFolder())
                             .arg(m_scraper->folderCount())
                             .arg(m_scraper->currentLocation())
                             .arg(m_scraper->locationCount()));

    ui->progressBar->setRange(0, m_scraper->locationCount() + m_scraper->folderCount());
    ui->progressBar->setValue(m_scraper->currentLocation() + m_scraper->currentFolder());
}
