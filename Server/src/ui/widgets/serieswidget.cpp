#include "serieswidget.h"
#include "ui_serieswidget.h"

#include "model/series.h"

SeriesWidget::SeriesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SeriesWidget)
{
    ui->setupUi(this);
}

SeriesWidget::~SeriesWidget()
{
    delete ui;
}

void SeriesWidget::setSeries(QSharedPointer<Series> series)
{
    ui->labelActors->setText(series->actors().join(", "));
    ui->labelAiredFirst->setText(series->firstAired().toString(Qt::ISODate));
    ui->labelImdbId->setText(series->imdbId());
    ui->labelTitle->setText(series->title());
    ui->labelTvdbId->setText(QString::number(series->tvdbId()));
    ui->textEditOverview_2->setText(series->overview());
}
