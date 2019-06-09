/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QGroupBox *webcam;
    QLineEdit *console;
    QSpinBox *servo_base;
    QFrame *line_4;
    QPushButton *down_webcam;
    QLineEdit *distancia;
    QPushButton *left_webcam;
    QLabel *label_4;
    QFrame *line_3;
    QFrame *line;
    QLabel *label_3;
    QPushButton *up_webcam;
    QSpinBox *servo_camera;
    QLabel *label_5;
    QPushButton *right_webcam;
    QLabel *carro;
    QPushButton *down_carro;
    QPushButton *left_carro;
    QPushButton *up_carro;
    QPushButton *right_carro;
    QLabel *label_2;
    QFrame *line_5;
    QSpinBox *pwm_ctrl;
    QSpinBox *ms_ctrl;
    QFrame *line_2;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_6;
    QWidget *camViewfinder;
    QGroupBox *groupBox_2;
    QPushButton *btnCaptureImage;
    QPushButton *btnClose;
    QPushButton *btnCloseCam;
    QPushButton *btnOpenCam;
    QComboBox *cbListCam;
    QLineEdit *lineEdit;
    QLabel *label_7;
    QPushButton *gravarVideo;
    QPushButton *pararVideo;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->setEnabled(true);
        Widget->resize(1076, 703);
        Widget->setStyleSheet(QString::fromUtf8("background-color: rgb(211, 215, 207);"));
        gridLayout = new QGridLayout(Widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        webcam = new QGroupBox(Widget);
        webcam->setObjectName(QString::fromUtf8("webcam"));
        webcam->setMaximumSize(QSize(320, 16777215));
        console = new QLineEdit(webcam);
        console->setObjectName(QString::fromUtf8("console"));
        console->setGeometry(QRect(100, 650, 211, 31));
        console->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        servo_base = new QSpinBox(webcam);
        servo_base->setObjectName(QString::fromUtf8("servo_base"));
        servo_base->setGeometry(QRect(30, 380, 91, 30));
        servo_base->setLayoutDirection(Qt::LeftToRight);
        servo_base->setStyleSheet(QString::fromUtf8("background-color:#fff"));
        servo_base->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        servo_base->setMinimum(0);
        servo_base->setMaximum(180);
        servo_base->setValue(90);
        line_4 = new QFrame(webcam);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(20, 310, 281, 20));
        line_4->setLineWidth(2);
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        down_webcam = new QPushButton(webcam);
        down_webcam->setObjectName(QString::fromUtf8("down_webcam"));
        down_webcam->setGeometry(QRect(140, 540, 41, 41));
        down_webcam->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/down.png"), QSize(), QIcon::Normal, QIcon::Off);
        down_webcam->setIcon(icon);
        down_webcam->setIconSize(QSize(25, 25));
        down_webcam->setAutoRepeat(true);
        distancia = new QLineEdit(webcam);
        distancia->setObjectName(QString::fromUtf8("distancia"));
        distancia->setGeometry(QRect(100, 610, 211, 31));
        distancia->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        left_webcam = new QPushButton(webcam);
        left_webcam->setObjectName(QString::fromUtf8("left_webcam"));
        left_webcam->setGeometry(QRect(70, 480, 41, 41));
        left_webcam->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/left.png"), QSize(), QIcon::Normal, QIcon::Off);
        left_webcam->setIcon(icon1);
        left_webcam->setIconSize(QSize(25, 25));
        left_webcam->setAutoRepeat(true);
        label_4 = new QLabel(webcam);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(210, 360, 91, 20));
        label_4->setStyleSheet(QString::fromUtf8("border-color: rgb(0, 0, 0);"));
        line_3 = new QFrame(webcam);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(20, 590, 291, 20));
        line_3->setLineWidth(2);
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line = new QFrame(webcam);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 50, 20, 541));
        line->setLineWidth(2);
        line->setMidLineWidth(1);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(webcam);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 360, 111, 20));
        label_3->setStyleSheet(QString::fromUtf8("border-color: rgb(0, 0, 0);"));
        up_webcam = new QPushButton(webcam);
        up_webcam->setObjectName(QString::fromUtf8("up_webcam"));
        up_webcam->setGeometry(QRect(140, 410, 41, 41));
        up_webcam->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        up_webcam->setIcon(icon2);
        up_webcam->setIconSize(QSize(25, 25));
        up_webcam->setAutoRepeat(true);
        servo_camera = new QSpinBox(webcam);
        servo_camera->setObjectName(QString::fromUtf8("servo_camera"));
        servo_camera->setGeometry(QRect(210, 380, 91, 30));
        servo_camera->setLayoutDirection(Qt::LeftToRight);
        servo_camera->setStyleSheet(QString::fromUtf8("background-color:#fff"));
        servo_camera->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        servo_camera->setMinimum(0);
        servo_camera->setMaximum(180);
        servo_camera->setValue(84);
        label_5 = new QLabel(webcam);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(90, 330, 141, 20));
        right_webcam = new QPushButton(webcam);
        right_webcam->setObjectName(QString::fromUtf8("right_webcam"));
        right_webcam->setGeometry(QRect(210, 480, 41, 41));
        right_webcam->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/right.png"), QSize(), QIcon::Normal, QIcon::Off);
        right_webcam->setIcon(icon3);
        right_webcam->setIconSize(QSize(25, 25));
        right_webcam->setAutoRepeat(true);
        carro = new QLabel(webcam);
        carro->setObjectName(QString::fromUtf8("carro"));
        carro->setGeometry(QRect(120, 170, 81, 71));
        carro->setStyleSheet(QString::fromUtf8("border-image:url(:carro.jpg);"));
        down_carro = new QPushButton(webcam);
        down_carro->setObjectName(QString::fromUtf8("down_carro"));
        down_carro->setGeometry(QRect(140, 250, 41, 41));
        down_carro->setStyleSheet(QString::fromUtf8(""));
        down_carro->setIcon(icon);
        down_carro->setIconSize(QSize(25, 25));
        down_carro->setAutoRepeat(true);
        left_carro = new QPushButton(webcam);
        left_carro->setObjectName(QString::fromUtf8("left_carro"));
        left_carro->setGeometry(QRect(70, 180, 41, 41));
        left_carro->setStyleSheet(QString::fromUtf8(""));
        left_carro->setIcon(icon1);
        left_carro->setIconSize(QSize(25, 25));
        left_carro->setAutoRepeat(true);
        up_carro = new QPushButton(webcam);
        up_carro->setObjectName(QString::fromUtf8("up_carro"));
        up_carro->setGeometry(QRect(140, 120, 41, 41));
        up_carro->setStyleSheet(QString::fromUtf8(""));
        up_carro->setIcon(icon2);
        up_carro->setIconSize(QSize(25, 25));
        up_carro->setAutoRepeat(true);
        right_carro = new QPushButton(webcam);
        right_carro->setObjectName(QString::fromUtf8("right_carro"));
        right_carro->setGeometry(QRect(210, 180, 41, 41));
        right_carro->setMinimumSize(QSize(41, 0));
        right_carro->setStyleSheet(QString::fromUtf8(""));
        right_carro->setIcon(icon3);
        right_carro->setIconSize(QSize(25, 25));
        right_carro->setAutoRepeat(true);
        label_2 = new QLabel(webcam);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(90, 60, 131, 20));
        line_5 = new QFrame(webcam);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(20, 40, 291, 20));
        line_5->setLineWidth(2);
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        pwm_ctrl = new QSpinBox(webcam);
        pwm_ctrl->setObjectName(QString::fromUtf8("pwm_ctrl"));
        pwm_ctrl->setGeometry(QRect(30, 90, 91, 30));
        pwm_ctrl->setStyleSheet(QString::fromUtf8("background-color:#fff"));
        pwm_ctrl->setMinimum(250);
        pwm_ctrl->setMaximum(2500);
        ms_ctrl = new QSpinBox(webcam);
        ms_ctrl->setObjectName(QString::fromUtf8("ms_ctrl"));
        ms_ctrl->setGeometry(QRect(200, 90, 83, 30));
        ms_ctrl->setStyleSheet(QString::fromUtf8("background-color:#fff"));
        ms_ctrl->setMinimum(2000);
        ms_ctrl->setMaximum(2500);
        line_2 = new QFrame(webcam);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(300, 50, 20, 541));
        line_2->setLineWidth(2);
        line_2->setMidLineWidth(1);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        pushButton = new QPushButton(webcam);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(120, 460, 81, 71));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/webcam.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon4);
        pushButton->setIconSize(QSize(90, 80));
        label = new QLabel(webcam);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 616, 67, 21));
        label_6 = new QLabel(webcam);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 650, 67, 31));

        gridLayout->addWidget(webcam, 0, 1, 3, 1);

        camViewfinder = new QWidget(Widget);
        camViewfinder->setObjectName(QString::fromUtf8("camViewfinder"));
        camViewfinder->setEnabled(true);
        camViewfinder->setStyleSheet(QString::fromUtf8("background-color:#555;"));

        gridLayout->addWidget(camViewfinder, 0, 0, 2, 1);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMaximumSize(QSize(16777215, 100));
        btnCaptureImage = new QPushButton(groupBox_2);
        btnCaptureImage->setObjectName(QString::fromUtf8("btnCaptureImage"));
        btnCaptureImage->setEnabled(false);
        btnCaptureImage->setGeometry(QRect(320, 30, 108, 25));
        btnClose = new QPushButton(groupBox_2);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(670, 30, 51, 25));
        btnCloseCam = new QPushButton(groupBox_2);
        btnCloseCam->setObjectName(QString::fromUtf8("btnCloseCam"));
        btnCloseCam->setEnabled(false);
        btnCloseCam->setGeometry(QRect(220, 30, 93, 25));
        btnOpenCam = new QPushButton(groupBox_2);
        btnOpenCam->setObjectName(QString::fromUtf8("btnOpenCam"));
        btnOpenCam->setEnabled(false);
        btnOpenCam->setGeometry(QRect(130, 30, 80, 25));
        cbListCam = new QComboBox(groupBox_2);
        cbListCam->setObjectName(QString::fromUtf8("cbListCam"));
        cbListCam->setGeometry(QRect(10, 30, 111, 25));
        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(130, 60, 581, 25));
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(50, 60, 81, 31));
        gravarVideo = new QPushButton(groupBox_2);
        gravarVideo->setObjectName(QString::fromUtf8("gravarVideo"));
        gravarVideo->setEnabled(false);
        gravarVideo->setGeometry(QRect(440, 30, 89, 25));
        pararVideo = new QPushButton(groupBox_2);
        pararVideo->setObjectName(QString::fromUtf8("pararVideo"));
        pararVideo->setEnabled(false);
        pararVideo->setGeometry(QRect(540, 30, 121, 25));

        gridLayout->addWidget(groupBox_2, 2, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Qt5 - Explorando QCamera", nullptr));
#ifndef QT_NO_ACCESSIBILITY
        Widget->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        webcam->setTitle(QApplication::translate("Widget", "                                  CONTROLES", nullptr));
        console->setText(QString());
        servo_base->setSuffix(QApplication::translate("Widget", " \302\260", nullptr));
        down_webcam->setText(QString());
        distancia->setInputMask(QString());
        distancia->setText(QString());
        left_webcam->setText(QString());
        label_4->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-size:8pt;\">Servo motor Cam</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-size:8pt;\">Servo motor Base</span></p></body></html>", nullptr));
        up_webcam->setText(QString());
        servo_camera->setSuffix(QApplication::translate("Widget", " \302\260", nullptr));
        label_5->setText(QApplication::translate("Widget", "<html><head/><body><p>Controle da  Camera</p></body></html>", nullptr));
        right_webcam->setText(QString());
        carro->setText(QString());
        down_carro->setText(QString());
        left_carro->setText(QString());
        up_carro->setText(QString());
        right_carro->setText(QString());
        label_2->setText(QApplication::translate("Widget", "<html><head/><body><p>Controle do Carro </p></body></html>", nullptr));
        pwm_ctrl->setSuffix(QApplication::translate("Widget", " pwm", nullptr));
        ms_ctrl->setSuffix(QApplication::translate("Widget", " ms", nullptr));
        pushButton->setText(QString());
        label->setText(QApplication::translate("Widget", "Dist\303\242ncia", nullptr));
        label_6->setText(QApplication::translate("Widget", "Usu\303\241rio", nullptr));
        groupBox_2->setTitle(QApplication::translate("Widget", "bot\303\265es", nullptr));
        btnCaptureImage->setText(QApplication::translate("Widget", "Captura Image", nullptr));
        btnClose->setText(QApplication::translate("Widget", "Sair", nullptr));
        btnCloseCam->setText(QApplication::translate("Widget", "Desconectar", nullptr));
        btnOpenCam->setText(QApplication::translate("Widget", "Conectar", nullptr));
        label_7->setText(QApplication::translate("Widget", "Rede Insp", nullptr));
        gravarVideo->setText(QApplication::translate("Widget", "gravar v\303\255deo", nullptr));
        pararVideo->setText(QApplication::translate("Widget", "parar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
