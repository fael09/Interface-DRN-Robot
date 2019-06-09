#include "widget.h"
#include "ui_widget.h"
#include <QCameraInfo>
#include <QMessageBox>
#include <iostream>
#include <QAbstractButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QCamera>
#include <QFileDialog>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QMediaRecorder>
#include <QMediaPlayer>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"



using namespace std;
using namespace cv;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);



    arduino_port_nome = "";
    arduino_is_available = false;
    arduino = new QSerialPort;


    /*
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier() ){
            qDebug() << "Vendor ID : " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID : " << serialPortInfo.productIdentifier();
        }
    }
        */
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == rduino_uno_product_id){
                   arduino_port_nome =  serialPortInfo.portName();
                   arduino_is_available = true;

                }
            }
        }
    }

    if(arduino_is_available){
       //open and configure the serialport
        arduino->setPortName(arduino_port_nome);
        //arduino->open(QSerialPort::WriteOnly);
        arduino->open(QSerialPort::ReadWrite);// Write onle. I need to change this for write and read.
        arduino->setBaudRate(QSerialPort::Baud115200);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }else{
        //give error message if not available
        QMessageBox::warning(this,"Port error","Couldn't find Arduino!");
    }



    QMessageBox msgApp;

    /* Check if find device cameras here */
    if(checkCameras()){
        devCameras = QCameraInfo::availableCameras();
        foreach (const QCameraInfo &cameraInfo, devCameras) {
            qDebug() << "Device Name  : " << cameraInfo.deviceName() << endl;
            qDebug() << "Description  : " << cameraInfo.description() << endl;
            qDebug() << "Position     : " << cameraInfo.position() << endl;
            qDebug() << "Cam Default  : " << cameraInfo.defaultCamera() << endl;
            qDebug() << "Orientation  : " << cameraInfo.orientation() << endl;
        }
    }

    /* Set in ComboxBox list of device cameras */
    this->SetCbListCam(devCameras);


    /* If find camera OK, enable button Conectar
     * else Message Box "Not Found Device Camera"
     */
    if(ui->cbListCam->count() < 1) {
        msgApp.setWindowTitle("Informação");
        msgApp.setInformativeText("Nenhuma camera foi encontrada...");
        msgApp.setIcon(QMessageBox::Information);
        msgApp.exec();
    }
    else {
        ui->btnOpenCam->setEnabled(true);
    }
}


/**
 * @brief Widget::~Widget
 * Object Destructor Class Widget
 */
Widget::~Widget()
{
    if(arduino->isOpen()){
        arduino->close();
    }
    delete ui;
}



/**
 * @brief Widget::SetCbListCam
 * @param cams
 * Function to load device cameras in ComboBox
 */
void Widget::SetCbListCam(QList<QCameraInfo> &cams)
{
    QStringList AllCams;
    foreach (const QCameraInfo &cam, cams) {
        /* Set Description in ComboBox */
        //AllCams.append(cam.description());

        /* Set DeviceName in ComboBox */
        AllCams.append(cam.deviceName());
    }
    ui->cbListCam->addItems(AllCams);
}


/**
 * @brief Widget::on_btnOpenCam_clicked
 */
void Widget::on_btnOpenCam_clicked()
{
    QByteArray myDeviceCam;

    /* Enable Button "Capturar Image" and "Desconectar" */
    ui->btnCaptureImage->setEnabled(true);
    ui->btnCloseCam->setEnabled(true);
    ui->gravarVideo->setEnabled(true);


    /* Trick: Convert to QString to QByteArray using .toUtf8() */
    myDeviceCam = ui->cbListCam->itemText(ui->cbListCam->currentIndex()).toUtf8();

    /* Set device select in ComboxBox */
    camera = new QCamera(myDeviceCam);
    CameraImageCapture = new QCameraImageCapture(camera,this);
    recorder = new QMediaRecorder(camera,this);

    /*
     * Create ViewFinder "Visualizador" to device camera
     */
    viewFinder = new QCameraViewfinder(ui->camViewfinder);

    /* Call function OpenCam() */
    this->OpenCam();
}


/**
 * @brief Widget::checkCameras
 * @return True or False if find device cameras
 */
bool Widget::checkCameras()
{
    int camCount = QCameraInfo::availableCameras().count();
    if ( camCount > 0) {
        qDebug() << "Cameras encontradas: " << camCount << endl;
        return true;
    }
    else {
        qDebug() << "Nenhuma camera foi detectada!" << endl;
        return false;
    }
}


/**
 * @brief Widget::OpenCam
 */
void Widget::OpenCam(void)
{ 

    /*
     * Set resize screen width x heigth with size my ViewFinder
     */
    viewFinder->setFixedWidth(ui->camViewfinder->width());
    viewFinder->setFixedHeight(ui->camViewfinder->height());

    /*
     * Set my ViewFinder Widget in ViewFinder object QCamera
     */
    camera->setViewfinder(viewFinder);

    /*
     * Show ViewFinder
     */
     viewFinder->show();

    /*
     * Start capture video
     */
     camera->start();
}


/**
 * @brief Widget::on_btnCloseCam_clicked
 */
void Widget::on_btnCloseCam_clicked()
{
    /*
     * Stop capture video
     */
    camera->stop();

    /* Destruct objects created in OpenCam */
    delete camera;
    delete viewFinder;

    /*
     * Enable button "Conectar"
     * Disable buttons "Descontar" and "Captura Image"
     */
    ui->btnOpenCam->setEnabled(true);
    ui->btnCloseCam->setEnabled(false);
    ui->btnCaptureImage->setEnabled(false);
    ui->gravarVideo->setEnabled(false);


}


/**
 * @brief Widget::on_btnCaptureImage_clicked
 */
void Widget::on_btnCaptureImage_clicked()
{
    auto filename_imagem = QFileDialog::getSaveFileName(this, "Capturar", "/",
                                "Imagen (*.jpg;*.jpeg)");

    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->searchAndLock();
    //on shutter button pressed
    CameraImageCapture->capture(filename_imagem);
    //on shutter button released
    camera->unlock();
}

void Widget::on_btnClose_clicked()
{
    ui->btnCloseCam->click();
    close();
}


//-----------------------------------------------------------------------///
//cotrole do carrinho pelas setas
void Widget::on_up_carro_pressed()
{
     cout << "Carro anda para  frente." << endl;
     Widget::upWrite("1");
     Widget::upRead();
}

void Widget::on_right_carro_pressed()
{
    cout << "Carro anda para  direita." << endl;
    Widget::upWrite("2");
    Widget::upRead();

}

void Widget::on_down_carro_pressed()
{
    cout << "Carro anda para  trás." << endl;
    Widget::upWrite("5");
    Widget::upRead();
}


void Widget::on_left_carro_pressed()
{
    cout << "Carro anda para  esquerda." << endl;
    Widget::upWrite("4");
    Widget::upRead();
}
//-----------------------------------------------------//
// --------------------------------------------//
// controle da camera do carrinho pelos botões de seta
void Widget::on_up_webcam_pressed()
{
    cout << "Camera gira para cima." << endl;
}

void Widget::on_right_webcam_pressed()
{
    cout << "Camera gira para  direita." << endl;
}

void Widget::on_down_webcam_pressed()
{
    cout << "Camera gira para  baixo." << endl;
}

void Widget::on_left_webcam_pressed()
{
    cout << "Camera gira para  esquerda." << endl;

}

void Widget::on_pushButton_clicked()
{
    cout << "Camera no centro" << endl;

}

//----------------------------------------------------------//
// Escrita na porta seria do arduino
void Widget::upWrite(QString command){
    if(arduino->isWritable()){
        arduino->write(command.toStdString().c_str());
    }else{
        qDebug() << "Couldn't write the Serial!";
    }

}
// Leitura na porta seria do arduino
void Widget::upRead(){
    QString out;
    if(arduino->isReadable()){
       out = arduino->readAll();
      // qDebug() << out;
    }else{
        qDebug() << "Couldn't write the Serial!";
    }

}
//----------------------------------------------------//

//-----------------------------------------------------//
// controle do pwm do carrinho
void Widget::on_pwm_ctrl_valueChanged(int pwm)
{
    qDebug() << pwm;
}
// controle de ms do carrinho
void Widget::on_ms_ctrl_valueChanged(int ms)
{
    qDebug() << ms;
}
// Controle do angulo da base
void Widget::on_servo_base_valueChanged(int angulo_base)
{
    qDebug() << angulo_base;
}
 // Controle do angulo da camera
void Widget::on_servo_camera_valueChanged(int angulo_camera)
{
    qDebug() << angulo_camera;
}
//----------------------------------------------------//







void Widget::on_gravarVideo_clicked()
{
    ui->gravarVideo->setEnabled(false);
    ui->pararVideo->setEnabled(true);
    QString css = "background-color:#f00;color:#000";
     ui->gravarVideo->setStyleSheet(css);

     camera->stop();

     /* Destruct objects created in OpenCam */
    delete camera;
    delete viewFinder;

     VideoCapture vcap(0);
       if(!vcap.isOpened()){
              cout << "Error opening video stream or file" << endl;

       }

    int frame_width=   vcap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height=   vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
    VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height),true);
    //QString x;
    while(1){

        Mat frame;
        vcap >> frame;
        video.write(frame);
        imshow( "Frame", frame );
        // se apertar esc vai parar o loop
        char c = (char)waitKey(60);
        if (c == 27){
          break;
        }
        //x = ui->pararVideo->text();

        if(ui->pararVideo->text() == "parado"){
            break;
        }


     }

    vcap.release();
    destroyAllWindows();
     css = "background-color:#ddd;color:#000";
     ui->gravarVideo->setStyleSheet(css);
     this->on_btnOpenCam_clicked();
     this->OpenCam();
}


void Widget::on_pararVideo_clicked()
{
    ui->pararVideo->setText("parado");
    ui->gravarVideo->setEnabled(true);
    ui->pararVideo->setEnabled(false);
}
