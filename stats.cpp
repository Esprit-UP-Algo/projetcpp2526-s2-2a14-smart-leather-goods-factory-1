#include "stats.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>

Stats::Stats(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Statistiques des commandes");
    resize(980, 720);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Controls row
    QHBoxLayout *controls = new QHBoxLayout();
    QLabel *lbl = new QLabel("Mois:");
    monthEdit = new QDateEdit(QDate::currentDate());
    monthEdit->setDisplayFormat("yyyy-MM");
    monthEdit->setCalendarPopup(true);
    monthEdit->setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));
    controls->addWidget(lbl);
    controls->addWidget(monthEdit);
    controls->addStretch();
    layout->addLayout(controls);

    QTabWidget *tabs = new QTabWidget(this);
    layout->addWidget(tabs);

    statusChartView = new QChartView();
    monthlyCountChartView = new QChartView();
    revenueChartView = new QChartView();
    paymentChartView = new QChartView();

    tabs->addTab(statusChartView, "États");
    tabs->addTab(monthlyCountChartView, "Cmd / Jour");
    tabs->addTab(revenueChartView, "Revenu / Mois");
    tabs->addTab(paymentChartView, "Paiement");

    // Initial charts
    refreshCharts();

    // React to month changes
    connect(monthEdit, &QDateEdit::dateChanged, this, [this](const QDate &) {
        refreshCharts();
    });
}

QChart *Stats::buildStatusDistributionChart() {
    auto counts = fetchStatusCounts(currentYearMonth());
    QPieSeries *series = new QPieSeries();
    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        if (it.value() > 0) {
            series->append(it.key(), it.value());
        }
    }
    for (auto s : series->slices()) {
        s->setLabel(QString("%1 (%2)").arg(s->label()).arg((int)s->value()));
        s->setLabelVisible(true);
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par état de commande");
    chart->legend()->setAlignment(Qt::AlignRight);
    return chart;
}

QChart *Stats::buildDailyCountChart() {
    auto dayToCount = fetchDailyCounts(currentYearMonth());

    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Commandes");
    QStringList categories;

    // Sort by day number (DD)
    QStringList keys = dayToCount.keys();
    std::sort(keys.begin(), keys.end());

    for (const QString &k : keys) {
        categories << k;
        *set << dayToCount.value(k, 0);
    }
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Nombre de commandes par jour");

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Commandes");
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
    return chart;
}

QChart *Stats::buildMonthlyRevenueChart() {
    auto dayToRevenue = fetchMonthlyRevenueShipped(currentYearMonth());

    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Revenu (€)");
    set->setColor(QColor("#6f8f3d")); // Leather theme green

    QStringList categories;
    
    // Sort keys (days 01, 02, etc.)
    QStringList keys = dayToRevenue.keys();
    std::sort(keys.begin(), keys.end());

    if (keys.isEmpty()) {
        // Handle empty state so chart doesn't crash/look broken
        categories << "Pas de données";
        *set << 0;
    } else {
        for (const QString &day : keys) {
            categories << day;
            *set << dayToRevenue.value(day);
        }
    }

    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Revenu quotidien pour : " + currentYearMonth() + " (Commandes Livrées)");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // X Axis (Days)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Jour du mois");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y Axis (Money)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Montant Total");
    axisY->setLabelFormat("%.2f");
    // Ensure axis starts at 0
    axisY->setMin(0); 
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}

QChart *Stats::buildPaymentModeChart() {
    auto counts = fetchPaymentModeCounts(currentYearMonth());
    QPieSeries *series = new QPieSeries();
    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        if (it.value() > 0) {
            series->append(it.key(), it.value());
        }
    }
    for (auto s : series->slices()) {
        s->setLabel(QString("%1 (%2)").arg(s->label()).arg((int)s->value()));
        s->setLabelVisible(true);
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par mode de paiement");
    chart->legend()->setAlignment(Qt::AlignRight);
    return chart;
}

QString Stats::currentYearMonth() const {
    QDate d = monthEdit->date();
    return d.toString("yyyy-MM");
}

void Stats::refreshCharts() {
    statusChartView->setChart(buildStatusDistributionChart());
    monthlyCountChartView->setChart(buildDailyCountChart());
    revenueChartView->setChart(buildMonthlyRevenueChart());
    paymentChartView->setChart(buildPaymentModeChart());
}

QMap<QString, int> Stats::fetchStatusCounts(const QString &ym) const {
    QMap<QString, int> map;
    QSqlQuery q;
    q.prepare("SELECT ETAT_COMMANDE, COUNT(*) FROM COMMANDE "
              "WHERE TO_CHAR(DATE_COMMANDE, 'YYYY-MM') = :ym "
              "GROUP BY ETAT_COMMANDE");
    q.bindValue(":ym", ym);
    q.exec();
    while (q.next()) {
        map[q.value(0).toString()] = q.value(1).toInt();
    }
    return map;
}

QMap<QString, int> Stats::fetchDailyCounts(const QString &ym) const {
    QMap<QString, int> map;
    QSqlQuery q;
    q.prepare("SELECT TO_CHAR(DATE_COMMANDE, 'DD') AS dd, COUNT(*) "
              "FROM COMMANDE "
              "WHERE TO_CHAR(DATE_COMMANDE, 'YYYY-MM') = :ym "
              "GROUP BY TO_CHAR(DATE_COMMANDE, 'DD')");
    q.bindValue(":ym", ym);
    q.exec();
    while (q.next()) {
        map[q.value(0).toString()] = q.value(1).toInt();
    }
    return map;
}

QMap<QString, double> Stats::fetchMonthlyRevenueShipped(const QString &ym) const {
    QMap<QString, double> map;
    QSqlQuery q;
    // We group by Day (DD) to show revenue distribution over the month
    q.prepare("SELECT TO_CHAR(DATE_COMMANDE, 'DD') AS dd, SUM(MONTANT_TOTAL) "
              "FROM COMMANDE "
              "WHERE TO_CHAR(DATE_COMMANDE, 'YYYY-MM') = :ym "
              "AND (UPPER(ETAT_COMMANDE) LIKE 'LIVR%' OR UPPER(ETAT_COMMANDE) = 'LIVRÉE') "
              "GROUP BY TO_CHAR(DATE_COMMANDE, 'DD') "
              "ORDER BY dd ASC");
    q.bindValue(":ym", ym);
    
    if (!q.exec()) {
        qDebug() << "SQL Error (Revenue):" << q.lastError().text();
    }

    while (q.next()) {
        map[q.value(0).toString()] = q.value(1).toDouble();
    }
    return map;
}

QMap<QString, int> Stats::fetchPaymentModeCounts(const QString &ym) const {
    QMap<QString, int> map;
    QSqlQuery q;
    q.prepare("SELECT MODE_PAIEMENT, COUNT(*) FROM COMMANDE "
              "WHERE TO_CHAR(DATE_COMMANDE, 'YYYY-MM') = :ym "
              "GROUP BY MODE_PAIEMENT");
    q.bindValue(":ym", ym);
    q.exec();
    while (q.next()) {
        map[q.value(0).toString()] = q.value(1).toInt();
    }
    return map;
}

