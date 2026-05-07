#include "stats.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTabWidget>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>
#include <QFileDialog>
#include <QBrush>
#include <QFont>
#include <QColor>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>

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

// ── Style d'une carte KPI ─────────────────────────────────────────────────────
static QFrame *makeKpiCard(QLabel *&valLabel, const QString &caption, QWidget *parent) {
    QFrame *card = new QFrame(parent);
    card->setStyleSheet(
        "QFrame { background: qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "stop:0 #6b3e26,stop:1 #4a2717); border-radius:14px; }");

    QVBoxLayout *lay = new QVBoxLayout(card);
    lay->setContentsMargins(16, 14, 16, 12);
    lay->setSpacing(4);

    valLabel = new QLabel("—");
    valLabel->setAlignment(Qt::AlignCenter);
    valLabel->setStyleSheet("color:#f0c060; font-size:26px; font-weight:900; background:transparent;");

    QLabel *capLabel = new QLabel(caption);
    capLabel->setAlignment(Qt::AlignCenter);
    capLabel->setStyleSheet("color:#fffaf5; font-size:11px; font-weight:700; "
                            "letter-spacing:1.5px; background:transparent;");

    lay->addWidget(valLabel);
    lay->addWidget(capLabel);
    return card;
}

Stats::Stats(QWidget *parent) : QDialog(parent) {
    setStyleSheet(
        "QDialog { background-color: #f1e7dc; color: #3a2a20; font-family: 'Segoe UI'; }"
        "QLabel { color: #3a2a20; font-weight: bold; font-size: 13px; }"
        "QDateEdit { background-color: rgba(255,250,245,0.6); border:1px solid #c6a88d;"
        "  border-radius:6px; padding:6px; color:#3a2a20; }"
        "QDateEdit:focus { border:2px solid #6b3e26; background-color:#fffaf5; }"
        "QTabWidget::pane { border:2px solid #b08a6b; border-radius:8px; background-color:#fffaf5; }"
        "QTabBar::tab { background-color:#e9dccf; color:#3a2a20; padding:8px 16px;"
        "  border-top-left-radius:8px; border-top-right-radius:8px;"
        "  border:1px solid #c6a88d; border-bottom:none; margin-right:2px; }"
        "QTabBar::tab:selected { background-color:#fffaf5; color:#5b2f1d;"
        "  font-weight:bold; border-bottom:2px solid #fffaf5; }"
        "QPushButton { background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "  stop:0 #c4890a,stop:1 #8b4513); color:white; border-radius:8px;"
        "  padding:8px 18px; font-weight:800; border-bottom:3px solid #7a4800; }"
        "QPushButton:hover { background:#d4980c; }"
    );
    setWindowTitle("Statistiques des commandes");
    resize(1020, 780);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(16, 16, 16, 16);

    // ── Ligne de contrôles (mois + bouton export) ─────────────────────────────
    QHBoxLayout *controls = new QHBoxLayout();
    QLabel *lbl = new QLabel("Mois :");
    monthEdit = new QDateEdit(QDate::currentDate());
    monthEdit->setDisplayFormat("yyyy-MM");
    monthEdit->setCalendarPopup(true);
    monthEdit->setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));

    m_btnExport = new QPushButton("Exporter rapport PDF");
    connect(m_btnExport, &QPushButton::clicked, this, &Stats::exportStatsPdf);

    controls->addWidget(lbl);
    controls->addWidget(monthEdit);
    controls->addStretch();
    controls->addWidget(m_btnExport);
    layout->addLayout(controls);

    // ── Panel KPI — 4 cartes de résumé ───────────────────────────────────────
    QHBoxLayout *kpiRow = new QHBoxLayout();
    kpiRow->setSpacing(12);
    kpiRow->addWidget(makeKpiCard(m_kpiTotal,     "COMMANDES",   this));
    kpiRow->addWidget(makeKpiCard(m_kpiRevenue,   "REVENU (TND)", this));
    kpiRow->addWidget(makeKpiCard(m_kpiAvg,       "MOY / CMD",   this));
    kpiRow->addWidget(makeKpiCard(m_kpiDelivered, "LIVRÉES",     this));
    layout->addLayout(kpiRow);

    // ── Onglets des graphiques ────────────────────────────────────────────────
    QTabWidget *tabs = new QTabWidget(this);
    layout->addWidget(tabs);

    statusChartView       = new QChartView();
    monthlyCountChartView = new QChartView();
    revenueChartView      = new QChartView();
    paymentChartView      = new QChartView();

    statusChartView->setRenderHint(QPainter::Antialiasing);
    monthlyCountChartView->setRenderHint(QPainter::Antialiasing);
    revenueChartView->setRenderHint(QPainter::Antialiasing);
    paymentChartView->setRenderHint(QPainter::Antialiasing);

    tabs->addTab(statusChartView,       "États");
    tabs->addTab(monthlyCountChartView, "Cmd / Jour");
    tabs->addTab(revenueChartView,      "Revenu / Mois");
    tabs->addTab(paymentChartView,      "Paiement");

    refreshKpis();
    refreshCharts();

    connect(monthEdit, &QDateEdit::dateChanged, this, [this](const QDate &) {
        refreshKpis();
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
    chart->setAnimationOptions(QChart::SeriesAnimations);
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
    chart->setAnimationOptions(QChart::SeriesAnimations);

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
    QBarSet *set = new QBarSet("Revenu (TND)");
    set->setColor(QColor("#6f8f3d")); // vert thème cuir

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
    axisY->setTitleText("Montant Total (TND)");
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
        if (it.value() <= 0) continue;
        // Remplace les labels vides ou NULL par "Non défini"
        QString label = it.key().trimmed().isEmpty() ? "Non défini" : it.key();
        series->append(label, it.value());
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
    q.prepare("SELECT ETAT_COMMANDE, COUNT(*) FROM SMARTLEATHER.COMMANDE "
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
              "FROM SMARTLEATHER.COMMANDE "
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
              "FROM SMARTLEATHER.COMMANDE "
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
    q.prepare("SELECT MODE_PAIEMENT, COUNT(*) FROM SMARTLEATHER.COMMANDE "
              "WHERE TO_CHAR(DATE_COMMANDE, 'YYYY-MM') = :ym "
              "GROUP BY MODE_PAIEMENT");
    q.bindValue(":ym", ym);
    q.exec();
    while (q.next()) {
        map[q.value(0).toString()] = q.value(1).toInt();
    }
    return map;
}

// ── Mise à jour des 4 KPI cards ──────────────────────────────────────────────
void Stats::refreshKpis() {
    const QString ym = currentYearMonth();
    QSqlQuery q;
    q.prepare(
        "SELECT COUNT(*), "
        "       NVL(SUM(MONTANT_TOTAL), 0), "
        "       NVL(AVG(MONTANT_TOTAL), 0), "
        "       SUM(CASE WHEN UPPER(ETAT_COMMANDE) LIKE 'LIVR%' THEN 1 ELSE 0 END) "
        "FROM SMARTLEATHER.COMMANDE "
        "WHERE TO_CHAR(DATE_COMMANDE, 'YYYY-MM') = :ym");
    q.bindValue(":ym", ym);

    if (!q.exec() || !q.next()) {
        m_kpiTotal->setText("—");
        m_kpiRevenue->setText("—");
        m_kpiAvg->setText("—");
        m_kpiDelivered->setText("—");
        return;
    }

    int    total     = q.value(0).toInt();
    double revenue   = q.value(1).toDouble();
    double avg       = q.value(2).toDouble();
    int    delivered = q.value(3).toInt();

    m_kpiTotal->setText(QString::number(total));
    m_kpiRevenue->setText(QString("%L1").arg(revenue, 0, 'f', 0));
    m_kpiAvg->setText(QString("%L1").arg(avg, 0, 'f', 0));
    m_kpiDelivered->setText(QString::number(delivered));
}

// ── Export rapport PDF des statistiques du mois ───────────────────────────────
void Stats::exportStatsPdf() {
    const QString ym = currentYearMonth();

    QString path = QFileDialog::getSaveFileName(
        this, "Exporter rapport statistiques",
        "rapport_stats_" + ym + ".pdf",
        "PDF (*.pdf)");
    if (path.isEmpty()) return;

    // ── Collecte des données ──────────────────────────────────────────────────
    auto statusCounts  = fetchStatusCounts(ym);
    auto dailyCounts   = fetchDailyCounts(ym);
    auto revenueData   = fetchMonthlyRevenueShipped(ym);
    auto paymentCounts = fetchPaymentModeCounts(ym);

    int    totalCmd = 0;
    double totalRev = 0.0;
    for (auto v : statusCounts)  totalCmd += v;
    for (auto v : revenueData)   totalRev += v;

    // ── Construction HTML ─────────────────────────────────────────────────────
    QString css =
        "body { font-family:'Segoe UI',Arial,sans-serif; color:#3a2a20; margin:30px; }"
        "h1 { color:#5b2f1d; font-size:22pt; margin-bottom:4px; }"
        "h2 { color:#6b3e26; font-size:14pt; margin:20px 0 6px; border-bottom:2px solid #b08a6b; }"
        "table { border-collapse:collapse; width:100%; margin-bottom:12px; }"
        "th { background:#6b3e26; color:#fffaf5; padding:8px 12px; text-align:left; }"
        "td { padding:7px 12px; border:1px solid #c9b2a2; }"
        "tr:nth-child(even) td { background:#f7ede2; }"
        ".kpi-grid { display:grid; grid-template-columns:repeat(4,1fr); gap:10px; margin-bottom:20px; }"
        ".kpi { background:linear-gradient(135deg,#6b3e26,#4a2717); color:#fffaf5;"
        "  border-radius:10px; padding:14px; text-align:center; }"
        ".kpi-val { font-size:22pt; font-weight:900; color:#f0c060; }"
        ".kpi-lbl { font-size:9pt; letter-spacing:1px; margin-top:4px; }";

    QString html;
    html += "<!DOCTYPE html><html><head><meta charset='utf-8'/><style>" + css + "</style></head><body>";
    html += "<h1>SmartLeather — Rapport Statistiques</h1>";
    html += "<p style='color:#8b4513;font-size:12pt;'>Période : <b>" + ym + "</b>"
            " &nbsp;|&nbsp; Généré le : <b>"
            + QDate::currentDate().toString("dd/MM/yyyy") + "</b></p><hr/>";

    // KPI grid
    html += "<div class='kpi-grid'>";
    html += "<div class='kpi'><div class='kpi-val'>" + QString::number(totalCmd) + "</div><div class='kpi-lbl'>COMMANDES</div></div>";
    html += "<div class='kpi'><div class='kpi-val'>" + QString::number((int)totalRev) + "</div><div class='kpi-lbl'>REVENU TND</div></div>";
    html += "<div class='kpi'><div class='kpi-val'>"
            + (totalCmd > 0 ? QString::number((int)(totalRev/totalCmd)) : "0")
            + "</div><div class='kpi-lbl'>MOY / CMD</div></div>";
    int livrees = statusCounts.value("Livrée", 0) + statusCounts.value("Livrée", 0);
    // cherche toutes les clés contenant "livr" (insensible casse)
    int nbLivrees = 0;
    for (auto it = statusCounts.cbegin(); it != statusCounts.cend(); ++it)
        if (it.key().contains("livr", Qt::CaseInsensitive)) nbLivrees += it.value();
    html += "<div class='kpi'><div class='kpi-val'>" + QString::number(nbLivrees) + "</div><div class='kpi-lbl'>LIVRÉES</div></div>";
    html += "</div>";

    // Tableau état
    html += "<h2>Répartition par état</h2><table><tr><th>État</th><th>Nb commandes</th></tr>";
    for (auto it = statusCounts.cbegin(); it != statusCounts.cend(); ++it)
        html += "<tr><td>" + it.key() + "</td><td>" + QString::number(it.value()) + "</td></tr>";
    html += "</table>";

    // Tableau revenu
    if (!revenueData.isEmpty()) {
        html += "<h2>Revenu journalier — commandes livrées</h2>"
                "<table><tr><th>Jour</th><th>Montant (TND)</th></tr>";
        QStringList days = revenueData.keys();
        std::sort(days.begin(), days.end());
        for (const QString &d : days)
            html += "<tr><td>" + d + "</td><td>"
                    + QString::number(revenueData.value(d), 'f', 2) + "</td></tr>";
        html += "</table>";
    }

    // Tableau mode de paiement
    if (!paymentCounts.isEmpty()) {
        html += "<h2>Mode de paiement</h2><table><tr><th>Mode</th><th>Nb</th></tr>";
        for (auto it = paymentCounts.cbegin(); it != paymentCounts.cend(); ++it) {
            QString label = it.key().trimmed().isEmpty() ? "Non défini" : it.key();
            html += "<tr><td>" + label + "</td><td>" + QString::number(it.value()) + "</td></tr>";
        }
        html += "</table>";
    }

    html += "<br/><p style='text-align:center;color:#a47148;font-style:italic;'>"
            "SmartLeather ERP — Rapport généré automatiquement</p></body></html>";

    // ── Impression PDF ────────────────────────────────────────────────────────
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Export réussi",
        "Rapport exporté vers :\n" + path);
}

