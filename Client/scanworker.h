#pragma once

#include <QtCore>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <zxing/qrcode/QRCodeReader.h>

typedef CvCapture *TDevice;

class ScanWorker: public QObject
{
    Q_OBJECT

    TDevice device;
    IplImage *frame, *gray;
    int width, height;

    IplImage *CaptureFrame();

    zxing::qrcode::QRCodeReader reader;
    void decodeImage();
    void UpdateFrame();

public:
    explicit ScanWorker(QObject *parent = nullptr);
    ~ScanWorker();

    bool SetupCam();
    void StopCam();

public slots:
    void work();

signals:
    void finished();
    void codeScanned(QString code);
};
