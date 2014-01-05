#ifndef SERIESWIDGET_H
#define SERIESWIDGET_H

#include <QWidget>

namespace Ui {
class SeriesWidget;
}

class Series;

class SeriesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SeriesWidget(QWidget *parent = 0);
    ~SeriesWidget();
    
    void setSeries(QSharedPointer<Series> series);

private:
    Ui::SeriesWidget *ui;
};

#endif // SERIESWIDGET_H
