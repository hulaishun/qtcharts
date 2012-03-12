#include "charttheme_p.h"
#include "qchart.h"
#include "qchartaxis.h"
#include <QTime>

//series
#include "qbarset.h"
#include "qbarseries.h"
#include "qstackedbarseries.h"
#include "qpercentbarseries.h"
#include "qlineseries.h"
#include "qareaseries.h"
#include "qscatterseries.h"
#include "qpieseries.h"
#include "qpieslice.h"
#include "qsplineseries.h"

//items
#include "axisitem_p.h"
#include "barpresenter_p.h"
#include "stackedbarpresenter_p.h"
#include "percentbarpresenter_p.h"
#include "linechartitem_p.h"
#include "areachartitem_p.h"
#include "scatterchartitem_p.h"
#include "piepresenter_p.h"
#include "splinechartitem_p.h"

//themes
#include "chartthemedefault_p.h"
#include "chartthemevanilla_p.h"
#include "chartthemeicy_p.h"
#include "chartthemegrayscale_p.h"
#include "chartthemescientific_p.h"


QTCOMMERCIALCHART_BEGIN_NAMESPACE

ChartTheme::ChartTheme(QChart::ChartTheme id)
{
    m_id = id;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}


ChartTheme* ChartTheme::createTheme(QChart::ChartTheme theme)
{
    switch(theme) {
        case QChart::ChartThemeVanilla:
            return new ChartThemeVanilla();
        case QChart::ChartThemeIcy:
            return new ChartThemeIcy();
        case QChart::ChartThemeGrayscale:
            return new ChartThemeGrayscale();
        case QChart::ChartThemeScientific:
            return new ChartThemeScientific();
        default:
            return new ChartThemeDefault();
    }
}

void ChartTheme::decorate(QChart* chart)
{
    chart->setChartBackgroundBrush(m_backgroundGradient);
}
/*
//TODO helper to by removed later
void ChartTheme::decorate(ChartItem* item, QSeries* series,int count)
{
    switch(series->type())
    {
        case QSeries::SeriesTypeLine: {
            QLineSeries* s = static_cast<QLineSeries*>(series);
            LineChartItem* i = static_cast<LineChartItem*>(item);
            decorate(i,s,count);
            break;
        }
        case QSeries::SeriesTypeArea: {
                   QAreaSeries* s = static_cast<QAreaSeries*>(series);
                   AreaChartItem* i = static_cast<AreaChartItem*>(item);
                   decorate(i,s,count);
                   break;
        }
        case QSeries::SeriesTypeBar: {
            QBarSeries* b = static_cast<QBarSeries*>(series);
            BarPresenter* i = static_cast<BarPresenter*>(item);
            decorate(i,b,count);
            break;
        }
        case QSeries::SeriesTypeStackedBar: {
            QStackedBarSeries* s = static_cast<QStackedBarSeries*>(series);
            StackedBarPresenter* i = static_cast<StackedBarPresenter*>(item);
            decorate(i,s,count);
            break;
        }
        case QSeries::SeriesTypePercentBar: {
            QPercentBarSeries* s = static_cast<QPercentBarSeries*>(series);
            PercentBarPresenter* i = static_cast<PercentBarPresenter*>(item);
            decorate(i,s,count);
            break;
        }
        case QSeries::SeriesTypeScatter: {
            QScatterSeries* s = qobject_cast<QScatterSeries*>(series);
            Q_ASSERT(s);
            ScatterChartItem* i = static_cast<ScatterChartItem*>(item);
            Q_ASSERT(i);
            decorate(i, s, count);
            break;
        }
        case QSeries::SeriesTypePie: {
            QPieSeries* s = static_cast<QPieSeries*>(series);
            PiePresenter* i = static_cast<PiePresenter*>(item);
            decorate(i,s,count);
            break;
        }
        default:
        qDebug()<<"Wrong item to be decorated by theme";
        break;
    }

}
*/
void ChartTheme::decorate(AreaChartItem* item, QAreaSeries* series,int count)
{
    QPen pen;
    QBrush brush;

    if (pen != series->pen()){
       item->setPen(series->pen());
    } else {
        pen.setColor(colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 1.0));
        pen.setWidthF(2);
        item->setPen(pen);
    }

    if (brush != series->brush()) {
       item->setBrush(series->brush());
    } else {
       QBrush brush(m_seriesColors.at(count % m_seriesColors.size()));
       item->setBrush(brush);
    }
}


void ChartTheme::decorate(LineChartItem* item, QLineSeries* series,int count)
{
    QPen pen;
    if(pen != series->pen()){
        item->setLinePen(series->pen());
        return;
    }
    pen.setColor(m_seriesColors.at(count%m_seriesColors.size()));
    pen.setWidthF(2);
    item->setLinePen(pen);
}

void ChartTheme::decorate(BarPresenter* item, QBarSeries* series,int count)
{
    QList<QBarSet*> sets = series->barSets();
    for (int i=0; i<sets.count(); i++) {
        qreal pos = 0.5;
        if (sets.count() > 1)
            pos = (qreal) i / (qreal) (sets.count() - 1);
        QColor c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), pos);
        sets.at(i)->setBrush(QBrush(c));

        // Pick label color as far as possible from bar color (within gradient).
        // 0.3 is magic number that was picked as value that gave enough contrast with icy theme gradient :)
        // TODO: better picking of label color?
        if (pos < 0.3) {
            c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 1);
        } else {
            c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 0);
        }
        sets.at(i)->setFloatingValuePen(QPen(c));
    }
}

void ChartTheme::decorate(StackedBarPresenter* item, QStackedBarSeries* series,int count)
{
    QList<QBarSet*> sets = series->barSets();
    for (int i=0; i<sets.count(); i++) {
        qreal pos = 0.5;
        if (sets.count() > 1)
            pos = (qreal) i / (qreal) (sets.count() - 1);
        QColor c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), pos);
        sets.at(i)->setBrush(QBrush(c));

        if (pos < 0.3) {
            c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 1);
        } else {
            c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 0);
        }
        sets.at(i)->setFloatingValuePen(QPen(c));
    }
}

void ChartTheme::decorate(PercentBarPresenter* item, QPercentBarSeries* series,int count)
{
    QList<QBarSet*> sets = series->barSets();
    for (int i=0; i<sets.count(); i++) {
        qreal pos = 0.5;
        if (sets.count() > 1)
            pos = (qreal) i / (qreal) (sets.count() - 1);
        QColor c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), pos);
        sets.at(i)->setBrush(QBrush(c));

        if (pos < 0.3) {
            c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 1);
        } else {
            c = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 0);
        }
        sets.at(i)->setFloatingValuePen(QPen(c));
    }
}

void ChartTheme::decorate(ScatterChartItem* item, QScatterSeries* series, int count)
{
    Q_ASSERT(item);
    Q_ASSERT(series);

    // Use a base color for brush
    item->setBrush(m_seriesColors.at(count % m_seriesColors.size()));

    // Take pen near from gradient start, effectively using a lighter color for outline
    QPen pen(QBrush(Qt::SolidPattern), 3);
    pen.setColor(colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 1.0));
    item->setPen(pen);
}

void ChartTheme::decorate(PiePresenter* item, QPieSeries* series, int count)
{
    // Get color for a slice from a gradient linearly, beginning from the start of the gradient
    for (int i(0); i < series->slices().count(); i++) {
        qreal pos = (qreal) i / (qreal) series->count();
        QColor penColor = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), 0.1);
        series->slices().at(i)->setSlicePen(penColor);
        QColor brushColor = colorAt(m_seriesGradients.at(count % m_seriesGradients.size()), pos);
        series->slices().at(i)->setSliceBrush(brushColor);
    }
}


void ChartTheme::decorate(QChartAxis* axis, AxisItem* item)
{
    //TODO: dummy defults for now
    axis->setLabelsBrush(Qt::black);
    axis->setLabelsPen(Qt::NoPen);
    axis->setShadesPen(Qt::NoPen);
    axis->setShadesOpacity(0.5);
}

void ChartTheme::decorate(SplineChartItem* item, QSplineSeries* series, int count)
{
    Q_ASSERT(item);
    Q_ASSERT(series);

    QPen pen;

    if(pen != series->pen()){
        item->setLinePen(series->pen());
    }else{
        pen.setColor(m_seriesColors.at(count%m_seriesColors.size()));
        pen.setWidthF(series->pen().widthF());
        item->setLinePen(series->pen());
    }

//    QColor color = m_seriesColors.at(count % m_seriesColors.size());
    // TODO: define alpha in the theme? or in the series?
    //color.setAlpha(120);

//    QBrush brush(color, Qt::SolidPattern);
//    presenter->m_markerBrush = brush;

//    QPen pen(brush, 3);
//    pen.setColor(color);
//    presenter->m_markerPen = pen;
}

void ChartTheme::generateSeriesGradients()
{
    // Generate gradients in HSV color space
    foreach (QColor color, m_seriesColors) {
        QLinearGradient g;
        qreal h = color.hsvHueF();
        qreal s = color.hsvSaturationF();

        // TODO: tune the algorithm to give nice results with most base colors defined in
        // most themes. The rest of the gradients we can define manually in theme specific
        // implementation.
        QColor start = color;
        start.setHsvF(h, 0.05, 0.95);
        g.setColorAt(0.0, start);

        g.setColorAt(0.5, color);

        QColor end = color;
        end.setHsvF(h, s, 0.25);
        g.setColorAt(1.0, end);

        m_seriesGradients << g;
    }
}


QColor ChartTheme::colorAt(const QColor &start, const QColor &end, qreal pos)
{
    Q_ASSERT(pos >=0.0 && pos <= 1.0);
    qreal r = start.redF() + ((end.redF() - start.redF()) * pos);
    qreal g = start.greenF() + ((end.greenF() - start.greenF()) * pos);
    qreal b = start.blueF() + ((end.blueF() - start.blueF()) * pos);
    QColor c;
    c.setRgbF(r, g, b);
    return c;
}

QColor ChartTheme::colorAt(const QGradient &gradient, qreal pos)
{
    Q_ASSERT(pos >=0 && pos <= 1.0);

    // another possibility:
    // http://stackoverflow.com/questions/3306786/get-intermediate-color-from-a-gradient

    QGradientStops stops = gradient.stops();
    int count = stops.count();

    // find previous stop relative to position
    QGradientStop prev = stops.first();
    for (int i=0; i<count; i++) {
        QGradientStop stop = stops.at(i);
        if (pos > stop.first)
            prev = stop;

        // given position is actually a stop position?
        if (pos == stop.first) {
            //qDebug() << "stop color" << pos;
            return stop.second;
        }
    }

    // find next stop relative to position
    QGradientStop next = stops.last();
    for (int i=count-1; i>=0; i--) {
        QGradientStop stop = stops.at(i);
        if (pos < stop.first)
            next = stop;
    }

    //qDebug() << "prev" << prev.first << "pos" << pos << "next" << next.first;

    qreal range = next.first - prev.first;
    qreal posDelta = pos - prev.first;
    qreal relativePos = posDelta / range;

    //qDebug() << "range" << range << "posDelta" << posDelta << "relativePos" << relativePos;

    return colorAt(prev.second, next.second, relativePos);
}

QTCOMMERCIALCHART_END_NAMESPACE
