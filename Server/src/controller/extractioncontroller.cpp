#include "extractioncontroller.h"

#include "controller.h"
#include "preferences.h"

#include "model/download.h"
#include "model/downloadpackage.h"

#include <QuunRar.h>
#include <QuunRarJob.h>

#include <QDir>
#include <QElapsedTimer>

ExtractionController::ExtractionController(QObject *parent) :
    QObject(parent)
{
    foreach (QSharedPointer<DownloadPackage> package, Qp::readAll<DownloadPackage>()) {
        connect(package.data(), &DownloadPackage::downloadFinished,
                this, &ExtractionController::extractFinishedPackage);
    }

    connect(Qp::dataAccessObject<DownloadPackage>(), &QpDaoBase::objectCreated,
            [=](QSharedPointer<QObject> obj) {
        QSharedPointer<DownloadPackage> package = qSharedPointerCast<DownloadPackage>(obj);
        if (!package)
            return;

        connect(package.data(), &DownloadPackage::downloadFinished,
                this, &ExtractionController::extractFinishedPackage);
    });
}

ExtractionController::~ExtractionController()
{
    foreach (QuunRarJob *job, m_jobs) {
        job->quit();
    }

    foreach (QElapsedTimer *timer, m_timers.values()) {
        delete timer;
    }
}

void ExtractionController::extractPackage(QSharedPointer<DownloadPackage> package)
{
    if (package->downloads().isEmpty())
        return;

//    if (!package->isDownloadFinished())
//        return;

    QList<QSharedPointer<Download> > dls = package->downloads();
    qSort(dls.begin(), dls.end(), [](QSharedPointer<Download> dl1, QSharedPointer<Download> dl2) {
        return dl1->fileName() < dl2->fileName();
    });

    QSharedPointer<Download> dl = dls.first();

    if (dl->destinationFolder().isEmpty())
        dl->setDestinationFolder(Preferences::downloadFolder());

    QuunRar *rar = new QuunRar(dl->destinationFolder() + QDir::separator() + dl->fileName(), this);
    rar->setDestinationDir(package->extractFolder());
    rar->setPassword("serienjunkies.org");
    if (!rar->open()) {
        package->setMessage(QLatin1String("Extraction failed: ") + rar->errorString());
        Qp::update(package);
        return;
    }

    QuunRarJob *job = rar->extract();
    package->setExtractedFileSize(rar->totalUnpackedSize());

    QElapsedTimer *timer = new QElapsedTimer;
    m_timers.insert(job, timer);
    timer->start();

    m_jobs.append(job);
    m_runningExtractions.insert(job, package);
    m_currentExtractingDownloads.insert(package, dl);
    Qp::update(package);

    // Must not use lambdas, because the signals come from a different thread!
    connect(job, &QuunRarJob::dataProcessed,
            this, &ExtractionController::bytesProcessed);

    connect(job, &QuunRarJob::volumeChanged,
            this, &ExtractionController::volumeChanged);

    connect(job, &QuunRarJob::finished,
            this, &ExtractionController::finished);

    connect(job, &QuunRarJob::currentFileChanged,
            this, &ExtractionController::currentFileChanged);

    connect(job, &QuunRarJob::error,
            this, &ExtractionController::error);

    connect(dl.data(), &QObject::destroyed,
            this, &ExtractionController::downloadDestroyed);

    connect(package.data(), &QObject::destroyed, [=]() {
        quitAndRemoveJob(job);
    });
}

void ExtractionController::extractFinishedPackage()
{
    QSharedPointer<DownloadPackage> package = Qp::sharedFrom(qobject_cast<DownloadPackage *>(sender()));
    if (!package)
        return;

    extractPackage(package);
}

void ExtractionController::bytesProcessed()
{
    QuunRarJob *job = static_cast<QuunRarJob *>(sender());
    QElapsedTimer *timer = m_timers.value(job);
    if (!timer)
        return;

    if (!timer->hasExpired(1000))
        return;
    timer->restart();

    QSharedPointer<DownloadPackage> package = m_runningExtractions.value(job);
    if (!package) {
        quitAndRemoveJob(job);
        return;
    }

    package->setBytesExtracted(job->processedData());
    Qp::update(package);

    QSharedPointer<Download> dl = m_currentExtractingDownloads.value(package);
    if (dl) {
        dl->setExtracting(true);
        Qp::update(dl);
    }
}

void ExtractionController::volumeChanged(const QString &volumeName)
{
    QuunRarJob *job = static_cast<QuunRarJob *>(sender());

    QSharedPointer<DownloadPackage> package = m_runningExtractions.value(job);
    if (!package) {
        quitAndRemoveJob(job);
        return;
    }

    QSharedPointer<Download> dl = m_currentExtractingDownloads.value(package);
    if (dl) {
        disconnect(dl.data(), 0, this, 0);
        dl->setExtracting(false);
        Qp::update(dl);
    }

    QFileInfo info(volumeName);
    QString fileName = info.fileName();

    foreach (dl, package->downloads()) {
        if (dl->fileName() == fileName) {
            connect(dl.data(), &QObject::destroyed,
                    this, &ExtractionController::downloadDestroyed);

            dl->setExtracting(true);
            m_currentExtractingDownloads.remove(package);
            m_currentExtractingDownloads.insert(package, dl);
            Qp::update(dl);
            break;
        }
    }
}

void ExtractionController::finished()
{
    QuunRarJob *job = static_cast<QuunRarJob *>(sender());

    QSharedPointer<DownloadPackage> package = m_runningExtractions.value(job);
    if (!package) {
        quitAndRemoveJob(job);
        return;
    }

    QSharedPointer<Download> dl = m_currentExtractingDownloads.value(package);
    if (dl) {
        dl->setExtracting(false);
        m_currentExtractingDownloads.remove(package);
        Qp::update(dl);
    }

    package->setBytesExtracted(job->processedData());
    package->setMessage(tr("Extract OK"));
    Qp::update(package);

    quitAndRemoveJob(job);
}

void ExtractionController::currentFileChanged(const QString &fileName)
{
    QuunRarJob *job = static_cast<QuunRarJob *>(sender());

    QSharedPointer<DownloadPackage> package = m_runningExtractions.value(job);
    if (!package) {
        quitAndRemoveJob(job);
        return;
    }

    package->setMessage(tr("Extracting %1").arg(fileName));
    Qp::update(package);
}

void ExtractionController::error()
{
    QuunRarJob *job = static_cast<QuunRarJob *>(sender());

    QSharedPointer<DownloadPackage> package = m_runningExtractions.value(job);
    if (!package) {
        quitAndRemoveJob(job);
        return;
    }

    package->setMessage(QLatin1String("Extraction failed: ") + job->archive()->errorString());
    quitAndRemoveJob(job);
}

void ExtractionController::downloadDestroyed()
{
    Download *dl = static_cast<Download *>(sender());
    m_currentExtractingDownloads.remove(dl->package());
}

void ExtractionController::quitAndRemoveJob(QuunRarJob *job)
{
    QElapsedTimer *timer = m_timers.value(job);
    if (timer) {
        delete timer;
        m_timers.remove(job);
    }

    QSharedPointer<DownloadPackage> package = m_runningExtractions.value(job);
    if (package) {
        if (m_currentExtractingDownloads.contains(package)) {
            m_currentExtractingDownloads.remove(package);
        }
        m_runningExtractions.remove(job);
    }

    if (m_jobs.contains(job)) {
        if (job->isRunning())
            job->quit();

//        job->archive()->deleteLater();
        m_jobs.removeAll(job);
    }

    emit extractionFinished(package);
}
