//#include "newserieswizard.h"
//#include "ui_newserieswizard.h"

//#include "controller/controller.h"
//#include "controller/plugincontroller.h"
//#include "plugins/downloadProviders/downloadprovider.h"

//#include "model/series.h"
//#include "preferences.h"

//#include <QCompleter>
//#include <QPushButton>
//#include <QDir>

//NewSeriesWizard::NewSeriesWizard(QWidget *parent) :
//    QWizard(parent),
//    ui(new Ui::NewSeriesWizard),
//    m_currentDownloadProvider(nullptr),
//    m_completer(nullptr),
//    m_currentSearchId(0)
//{
//    ui->setupUi(this);

//    foreach(DownloadProvider *provider, Controller::plugins()->downloadProviderPlugins()) {
//        ui->comboBoxProvider->addItem(provider->name());
//    }

//    foreach(QString location, Preferences::seriesLocations()) {
//        ui->comboBoxLocation->addItem(location);
//    }

//    ui->comboBoxProvider->setCurrentIndex(0);
//    m_currentDownloadProvider = Controller::plugins()->downloadProviderPlugins().first();
//    setWindowModality(Qt::WindowModal);
//    button(QWizard::NextButton)->setEnabled(false);

//    connect(m_currentDownloadProvider, &DownloadProvider::foundSeries,
//            this, &NewSeriesWizard::complete);

//    connect(m_currentDownloadProvider, &DownloadProvider::error, [=]() {
//        ui->labelError->setText(m_currentDownloadProvider->errorString());
//    });

//    m_timer.setSingleShot(true);
//    connect(&m_timer, &QTimer::timeout,
//            this, &NewSeriesWizard::timeout);

//    connect(this, &QDialog::accepted,
//            this, &NewSeriesWizard::finish);
//}

//NewSeriesWizard::~NewSeriesWizard()
//{
//    delete ui;
//}

//QString NewSeriesWizard::seriesTitle()
//{
//    return m_currentSeries.title;
//}

//void NewSeriesWizard::on_comboBoxProvider_currentIndexChanged(int index)
//{
//    button(QWizard::NextButton)->setEnabled(false);

//    if(m_currentDownloadProvider)
//        disconnect(m_currentDownloadProvider, 0, this, 0);

//    m_currentDownloadProvider = Controller::plugins()->downloadProviderPlugins().at(index);
//    connect(m_currentDownloadProvider, &DownloadProvider::foundSeries,
//            this, &NewSeriesWizard::complete);

//    connect(m_currentDownloadProvider, &DownloadProvider::error, [=]() {
//        ui->labelError->setText(m_currentDownloadProvider->errorString());
//    });

//    QString text = ui->lineEditName->text();
//    if(!m_completer
//            || (m_completer->completionCount() == 0
//                && text.size() > 2)) {
//        m_currentDownloadProvider->searchSeries(text);
//    }
//}

//void NewSeriesWizard::on_lineEditName_textEdited(const QString &text)
//{
//    if(m_timer.isActive())
//        return;

//    button(QWizard::NextButton)->setEnabled(false);
//    m_timer.start(1000);

//    foreach(DownloadProvider::SeriesData s, m_mostRecentResults) {
//        if(QString::compare(s.title, ui->lineEditName->text(), Qt::CaseInsensitive) == 0) {
//            setSeries(s);
//            button(QWizard::NextButton)->setEnabled(true);
//        }
//    }

//    if(!m_completer
//            || text.size() > 2) {
//        m_currentDownloadProvider->searchSeries(text);
//    }
//}


//void NewSeriesWizard::complete(QList<DownloadProvider::SeriesData> series)
//{
//    if(series.isEmpty()) {
//        ui->labelError->setText(tr("No such show"));
//        button(QWizard::NextButton)->setEnabled(false);
//    }
//    else {
//        ui->labelError->setText(QString());
//    }

//    m_mostRecentResults = series;
//    QStringList list;
//    foreach(DownloadProvider::SeriesData s, m_mostRecentResults) {
//        list.append(s.title);

//        if(QString::compare(s.title, ui->lineEditName->text(), Qt::CaseInsensitive) == 0) {
//            setSeries(s);
//            button(QWizard::NextButton)->setEnabled(true);
//        }
//    }

//    if(m_completer) {
//        m_completer->deleteLater();
//    }

//    m_completer = new QCompleter(list, this);
//    ui->lineEditName->setCompleter(m_completer);
//    m_completer->complete();
//}

//void NewSeriesWizard::timeout()
//{
//    complete(m_currentDownloadProvider->series());
//}

//void NewSeriesWizard::finish()
//{
//    QSharedPointer<Series> series = Series::forTitle(m_currentSeries.title);
//    if(!series) {
//        series = Qp::create<Series>();
//        Qp::update(series);
//    }
//    series->setSerienJunkiesUrl(m_currentSeries.url);
//    series->setTitle(m_currentSeries.title);

//    QDir dir(ui->comboBoxLocation->currentText());
//    dir.mkdir(m_currentSeries.title);
//    dir.cd(m_currentSeries.title);

//    series->addFolder(dir.absolutePath());

//    Qp::update(series);
//}

//void NewSeriesWizard::setSeries(DownloadProvider::SeriesData series)
//{
//    m_currentSeries = series;
//    ui->labelTitle->setText(series.title);
//    ui->labelBaseUrl->setText(series.url.toString());
//}

//void NewSeriesWizard::on_comboBoxLocation_currentIndexChanged(const QString &arg1)
//{
//    ui->labelLocation->setText(arg1);
//}
