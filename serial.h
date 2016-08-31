#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "aboutdialog.h"
#include "setdialog.h"
#include "helpdialog.h"

namespace Ui {
class serial;
}

class serial : public QWidget
{
    Q_OBJECT

public:
    explicit serial(QWidget *parent = 0);
    ~serial();

private:
    Ui::serial *ui;
    QSerialPort *my_serialport;
    AboutDialog *about;
    SetDialog *set;
    HelpDialog *help;
    //统计收发数据字节数
    qint64 cntRecvData=0;
    qint64 cntSendData=0;

    //统计继电器翻转次数
    qint8 statusFlag1=0;
    qint8 statusFlag2=0;
    qint8 statusFlag3=0;
    qint8 statusFlag4=0;
    //记录4路光耦的统计值
    int cntNum1=0;
    int cntNum2=0;
    int cntNum3=0;
    int cntNum4=0;

    //报告文件编号
    qint8 reportNum=0;
private slots:
    void my_readCnt();//串口接收数据槽函数
    void on_pushButton_switch_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_relay1_clicked();
    void on_pushButton_relay2_clicked();
    void on_pushButton_relay3_clicked();
    void on_pushButton_relay4_clicked();
    void showTime();
    void showDataCount();
    void on_pushButton_clear_clicked();
//    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_7_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_set_clicked();
    void on_pushButton_help_clicked();
    void on_pushButton_flush_clicked();
    void on_pushButton_clear1_clicked();
    void on_pushButton_clear2_clicked();
    void on_pushButton_clear3_clicked();
    void on_pushButton_clear4_clicked();
    void on_pushButton_clear_2_clicked();
    void on_pushButton_get1_clicked();
    void on_pushButton_get2_clicked();
    void on_pushButton_get3_clicked();
    void on_pushButton_get4_clicked();
};

#endif // SERIAL_H
