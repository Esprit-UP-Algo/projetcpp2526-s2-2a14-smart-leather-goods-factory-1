#ifndef STATS_H
#define STATS_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

class Stats : public QDialog
{
    Q_OBJECT
public:
    explicit Stats(QWidget *parent = nullptr);

private:
    // Contrôle de sélection du mois
    QDateEdit *monthEdit;

    // Vues des 4 graphiques
    QChartView *statusChartView;
    QChartView *monthlyCountChartView;
    QChartView *revenueChartView;
    QChartView *paymentChartView;

    // Labels du panel KPI (résumé chiffré en haut de la fenêtre)
    QLabel *m_kpiTotal;
    QLabel *m_kpiRevenue;
    QLabel *m_kpiAvg;
    QLabel *m_kpiDelivered;

    // Bouton export rapport PDF
    QPushButton *m_btnExport;

    // Constructeurs des graphiques
    QChart *buildStatusDistributionChart();
    QChart *buildDailyCountChart();
    QChart *buildMonthlyRevenueChart();
    QChart *buildPaymentModeChart();

    // Rafraîchissement
    QString currentYearMonth() const;
    void refreshCharts();
    void refreshKpis();       // Met à jour les 4 KPI cards
    void exportStatsPdf();    // Génère un rapport PDF des statistiques

    // Requêtes SQL
    QMap<QString, int>    fetchStatusCounts(const QString &ym) const;
    QMap<QString, int>    fetchDailyCounts(const QString &ym) const;
    QMap<QString, double> fetchMonthlyRevenueShipped(const QString &ym) const;
    QMap<QString, int>    fetchPaymentModeCounts(const QString &ym) const;
};

#endif // STATS_H
