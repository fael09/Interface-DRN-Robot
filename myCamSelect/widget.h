#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QList>
#include <QCameraInfo>
#include <QCameraViewfinder>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void SetCbListCam(QList<QCameraInfo> &cams);

private slots:
    void on_btnOpenCam_clicked();

    void on_btnCloseCam_clicked();

    void on_btnCaptureImage_clicked();

    void on_btnClose_clicked();

    void on_up_carro_pressed();

    void on_right_carro_pressed();

    void on_down_carro_pressed();


    void on_left_carro_pressed();

    void on_up_webcam_pressed();

    void on_right_webcam_pressed();

    void on_down_webcam_pressed();

    void on_left_webcam_pressed();

private:
    Ui::Widget *ui;
    QCameraViewfinder *viewFinder;

    QCamera *camera;

    bool checkCameras(void);
    void OpenCam(void);

protected:
    QList<QCameraInfo> devCameras;

};

#endif // WIDGET_H
