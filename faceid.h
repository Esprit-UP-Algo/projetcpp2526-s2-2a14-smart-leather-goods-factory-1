#ifndef FACEID_H
#define FACEID_H

#include <QObject>
#include <QProcess>

class FaceID : public QObject
{
    Q_OBJECT

public:
    explicit FaceID(QObject *parent = nullptr);
    void startFaceRecognition();

signals:
    void faceRecognized(const QString &cin);
    void faceRejected();

private slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFaceRecognitionFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *process;
};

#endif // FACEID_H
