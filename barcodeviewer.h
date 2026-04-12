#ifndef BARCODEVIEWER_H
#define BARCODEVIEWER_H

#include <QDialog>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class BarcodeViewer : public QDialog {
    Q_OBJECT
public:
    explicit BarcodeViewer(QString code, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_code;
    void setupStyle();
};

#endif
