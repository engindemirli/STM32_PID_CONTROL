#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIODevice>
#include <QTimer>
#include <qstring.h>
#define Vendor_id   0x04d8
#define Product_id  0x000A
#define tx_buff_size 0x08
#define rx_buff_size 0x16

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString Serial_get_port();
    void Serial_connect();
    void Serial_send(const char *data , uint8_t len);

private slots:

    void Serial_connect_timerSlot();
    void Serial_read();

    void on_spinBox_gain_kp_valueChanged(int arg1);

    void on_spinBox_gain_ki_valueChanged(int arg1);

    void on_spinBox_gain_kd_valueChanged(int arg1);

    void on_pushButton_send_kpid_clicked();

    void on_verticalSlider_req_frequancy_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    typedef union {

        struct {
            int32_t FV; //32 bit
        };

        struct {
            unsigned char B0;
            unsigned char B1;
            unsigned char B2;
            unsigned char B3;
        };
    } Uint32_t_char;

    QSerialPort *serial;
    bool Serial_is_serial_connected = false ;
    QTimer *Serial_connect_timer;
    char Serial_tx_buff[tx_buff_size];
    unsigned char Serial_rx_buff[rx_buff_size];

    uint8_t Pid_gain_kp;
    uint8_t Pid_gain_ki;
    uint8_t Pid_gain_kd;
    Uint32_t_char Pid_upid;
    Uint32_t_char Tim_clock;
    Uint32_t_char Pid_req_frequancy;
    Uint32_t_char Pid_fed_frequancy;
};
#endif // MAINWINDOW_H
