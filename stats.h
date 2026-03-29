#ifndef STATS_H
#define STATS_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QDateEdit>

class Stats : public QDialog
{
    Q_OBJECT
public:
    explicit Stats(QWidget *parent = nullptr);

private:
    // Controls
    QDateEdit *monthEdit;

    // Views
    QChartView *statusChartView;
    QChartView *monthlyCountChartView;
    QChartView *revenueChartView;
    QChartView *paymentChartView;

    // Builders
    QChart *buildStatusDistributionChart();
    QChart *buildDailyCountChart();
    QChart *buildMonthlyRevenueChart(); // filtered to shipped only
    QChart *buildPaymentModeChart();

    // Helpers
    QString currentYearMonth() const;
    void refreshCharts();

    QMap<QString, int> fetchStatusCounts(const QString &ym) const;
    QMap<QString, int> fetchDailyCounts(const QString &ym) const;
    QMap<QString, double> fetchMonthlyRevenueShipped(const QString &ym) const;
    QMap<QString, int> fetchPaymentModeCounts(const QString &ym) const;
};

#endif // STATS_H
