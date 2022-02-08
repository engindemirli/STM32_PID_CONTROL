#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIODevice>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    Serial_connect_timer = new QTimer(this);
    Serial_connect_timer->start(1000);
    connect(Serial_connect_timer, SIGNAL(timeout()),this, SLOT(Serial_connect_timerSlot()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(Serial_read()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::Serial_get_port()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {

        if(info.vendorIdentifier() == Vendor_id && info.productIdentifier() == Product_id)
        {
          return info.portName();
        }
   }
    return 0;
}

void MainWindow::Serial_connect_timerSlot()
{

    if(!Serial_is_serial_connected)
    {
        serial->setBaudRate(QSerialPort::Baud115200);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::TwoStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setPortName(Serial_get_port());
        serial->open(QIODevice::ReadWrite);
        if(serial->isOpen())
        {
            Serial_is_serial_connected = 1 ;
            Serial_connect_timer->stop();
            QMessageBox::about(this,"serial","connected");

        }
        if(!serial->isOpen())
        {
            //QMessageBox::about(this,"serial","not connected");
        }
    }
}

void MainWindow::Serial_send(const char *data , uint8_t len)
{
    serial->write(data,len);
}

void MainWindow::Serial_read()
{
    char buffer[1];
    unsigned char chk = 0;

    forever
    {
        if(serial->bytesAvailable()==0)
            break;

        serial->read(buffer, 1);

        for (int i = 1 ; i < 16 ; i++)
        {
         Serial_rx_buff[(i-1)]  = Serial_rx_buff[i];
        }
        Serial_rx_buff[15] = buffer[0];

        if(Serial_rx_buff[15] == 0xff &&  Serial_rx_buff[14] == 0x48 && Serial_rx_buff[1] == 0xff )
        {
            for(int i = 1; i<16 ; i++)
            {
                chk = chk + Serial_rx_buff[i];
            }
            if(chk == Serial_rx_buff[0])
            {
                Tim_clock.B0 = Serial_rx_buff[2];
                Tim_clock.B1 = Serial_rx_buff[3];
                Tim_clock.B2 = Serial_rx_buff[4];
                Tim_clock.B3 = Serial_rx_buff[5];

                Pid_upid.B0 = Serial_rx_buff[6];
                Pid_upid.B1 = Serial_rx_buff[7];
                Pid_upid.B2 = Serial_rx_buff[8];
                Pid_upid.B3 = Serial_rx_buff[9];
                ui->lcdNumber_pid_upid->display((int)Pid_upid.FV);
                ui->label->setNum((int)Pid_upid.FV);

                Pid_fed_frequancy.B0 = Serial_rx_buff[10];
                Pid_fed_frequancy.B1 = Serial_rx_buff[11];
                Pid_fed_frequancy.B2 = Serial_rx_buff[12];
                Pid_fed_frequancy.B3 = Serial_rx_buff[13];
                ui->lcdNumber_fed_frequancy->display((int)Pid_fed_frequancy.FV);
            }
        }
    }
}

void MainWindow::on_spinBox_gain_kp_valueChanged(int arg1)
{
    Pid_gain_kp = arg1 ;
}

void MainWindow::on_spinBox_gain_ki_valueChanged(int arg1)
{
    Pid_gain_ki = arg1 ;
}

void MainWindow::on_spinBox_gain_kd_valueChanged(int arg1)
{
    Pid_gain_kd = arg1 ;
}

void MainWindow::on_pushButton_send_kpid_clicked()
{
    Serial_tx_buff[6] = 0xfc;
    Serial_tx_buff[5] = 0x26;
    Serial_tx_buff[4] = Pid_gain_kd;
    Serial_tx_buff[3] = Pid_gain_ki;
    Serial_tx_buff[2] = Pid_gain_kp;
    Serial_tx_buff[1] = 0xfc;
    Serial_tx_buff[0] = 0xfc+0x26+Pid_gain_kd+Pid_gain_ki+Pid_gain_kp+0xfc;
    Serial_send(Serial_tx_buff,7);
}

void MainWindow::on_verticalSlider_req_frequancy_valueChanged(int value)
{
    ui->lcdNumber_req_frequancy->display(value);
    Pid_req_frequancy.FV = value;
    Serial_tx_buff[7] = 0xff;
    Serial_tx_buff[6] = 0x48;
    Serial_tx_buff[5] = Pid_req_frequancy.B3;
    Serial_tx_buff[4] = Pid_req_frequancy.B2;
    Serial_tx_buff[3] = Pid_req_frequancy.B1;
    Serial_tx_buff[2] = Pid_req_frequancy.B0;
    Serial_tx_buff[1] = 0xff;
    Serial_tx_buff[0] = 0xff+0x48+Pid_req_frequancy.B3+Pid_req_frequancy.B2+Pid_req_frequancy.B1+Pid_req_frequancy.B0+0xff;
    Serial_send(Serial_tx_buff,8);
}
