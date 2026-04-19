#include "stats.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>
#include <QBrush>
#include <QFont>
#include <QColor>

static void applyThemeToChart(QChart *chart) {
    chart->setBackgroundBrush(QBrush(QColor("#fffaf5")));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor("#fffaf5")));
    chart->setPlotAreaBackgroundVisible(true);

    QFont titleFont("Segoe UI", 14, QFont::Bold);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(QColor("#5b2f1d")));

    QFont legendFont("Segoe UI", 10);
    chart->legend()->setFont(legendFont);
    chart->legend()->setLabelColor(QColor("#3a2a20"));

    for (QAbstractAxis *axis : chart->axes()) {
        axis->setLabelsColor(QColor("#3a2a20"));
        axis->setLabelsFont(QFont("Segoe UI", 10));
        if (QValueAxis *valAxis = qobject_cast<QValueAxis*>(axis)) {
            valAxis->setGridLineColor(QColor("#e0d2c5"));
            valAxis->setLinePenColor(QColor("#b08a6b"));
        } else if (QBarCategoryAxis *catAxis = qobject_cast<QBarCategoryAxis*>(axis)) {
            catAxis->setGridLineColor(QColor("#e0d2c5"));
            catAxis->setLinePenColor(QColor("#b08a6b"));
        }
    }
}

Stats::Stats(QWidget *parent) : QDialog(parent) {
    setStyleSheet(
        "QDialog { background-color: #f1e7dc; color: #3a2a20; font-family: 'Segoe UI'; }"
        "QLabel { color: #3a2a20; font-weight: bold; font-size: 14px; }"
        "QDateEdit { background-color: rgba(255, 250, 245, 0.6); border: 1px solid #c6a88d; border-radius: 6px; padding: 6px; color: #3a2a20; }"
        "QDateEdit:focus { border: 2px solid #6b3e26; background-color: #fffaf5; }"
        "QTabWidget::pane { border: 2px solid #b08a6b; border-radius: 8px; background-color: #fffaf5; }"
        "QTabBar::tab { background-color: #e9dccf; color: #3a2a20; padding: 8px 16px; border-top-left-radius: 8px; border-top-right-radius: 8px; border: 1px solid #c6a88d; border-bottom: none; margin-right: 2px; }"
        "QTabBar::tab:selected { background-color: #fffaf5; color: #5b2f1d; font-weight: bold; border-bottom: 2px solid #fffaf5; }"
    );
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
    const QList<QColor> colors = {QColor("#b08a6b"), QColor("#6f8f3d"), QColor("#a23b2a"), QColor("#7a4a2e"), QColor("#a47148")};
    int colorIdx = 0;
    for (auto s : series->slices()) {
        s->setLabel(QString("%1 (%2)").arg(s->label()).arg((int)s->value()));
        s->setLabelVisible(true);
        s->setBrush(colors.at(colorIdx % colors.size()));
        s->setLabelColor(QColor("#3a2a20"));
        s->setLabelFont(QFont("Segoe UI", 10, QFont::Bold));
        colorIdx++;
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par état de commande");
    chart->legend()->setAlignment(Qt::AlignRight);
    applyThemeToChart(chart);
    return chart;
}

QChart *Stats::buildDailyCountChart() {
    auto dayToCount = fetchDailyCounts(currentYearMonth());

    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Commandes");
    set->setColor(QColor("#7a4a2e")); // Dark brown
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
    applyThemeToChart(chart);
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

    applyThemeToChart(chart);
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
    const QList<QColor> colors = {QColor("#a47148"), QColor("#d8b59c"), QColor("#4a2717"), QColor("#b08a6b"), QColor("#a23b2a")};
    int colorIdx = 0;
    for (auto s : series->slices()) {
        s->setLabel(QString("%1 (%2)").arg(s->label()).arg((int)s->value()));
        s->setLabelVisible(true);
        s->setBrush(colors.at(colorIdx % colors.size()));
        s->setLabelColor(QColor("#3a2a20"));
        s->setLabelFont(QFont("Segoe UI", 10, QFont::Bold));
        colorIdx++;
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par mode de paiement");
    chart->legend()->setAlignment(Qt::AlignRight);
    applyThemeToChart(chart);
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

