 #ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QList>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QAudioEncoderSettings>
#include <QVideoEncoderSettings>


namespace Ui {
class Widget;
}
//-------------------------
//classes de capitura de imagens

class QCameraImageCapture;
class QMediaRecorder;


//-------------------------

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
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

    void on_pushButton_clicked();

    void upWrite(QString );
    void upRead();

    void on_pwm_ctrl_valueChanged(int arg1);

    void on_ms_ctrl_valueChanged(int arg1);

    void on_servo_base_valueChanged(int arg1);

    void on_servo_camera_valueChanged(int arg1);


    void on_gravarVideo_clicked();



    void on_pararVideo_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 6790;
    static const quint16 rduino_uno_product_id = 29987;
    QString arduino_port_nome;
    bool arduino_is_available;

    QCameraImageCapture *CameraImageCapture;

    QCameraViewfinder *viewFinder;

    QCamera *camera;
    QMediaRecorder *recorder;


    bool checkCameras(void);
    void OpenCam(void);

protected:
    QList<QCameraInfo> devCameras;

};

#endif // WIDGET_H
