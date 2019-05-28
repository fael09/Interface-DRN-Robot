#include "widget.h"
#include "ui_widget.h"
#include <QCameraInfo>
#include <QDebug>
#include <QMessageBox>
#include <iostream>
#include <QAbstractButton>
using namespace std;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

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

    /* Trick: Convert to QString to QByteArray using .toUtf8() */
    myDeviceCam = ui->cbListCam->itemText(ui->cbListCam->currentIndex()).toUtf8();

    /* Set device select in ComboxBox */
    camera = new QCamera(myDeviceCam);

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

}


/**
 * @brief Widget::on_btnCaptureImage_clicked
 */
void Widget::on_btnCaptureImage_clicked()
{
    /* Implement Code Here! */
    /* Future Implementation */
}

void Widget::on_btnClose_clicked()
{
    ui->btnCloseCam->click();
    close();
}






void Widget::on_up_carro_pressed()
{
     cout << "Carro anda para  frente." << endl;
}

void Widget::on_right_carro_pressed()
{
    cout << "Carro anda para  direita." << endl;
}

void Widget::on_down_carro_pressed()
{
    cout << "Carro anda para  trás." << endl;
}


void Widget::on_left_carro_pressed()
{
    cout << "Carro anda para  esquerda." << endl;
}

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
