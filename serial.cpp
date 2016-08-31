#include "serial.h"
#include "ui_serial.h"
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QIcon>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QDesktopServices>
#include <QSound>
#include <QDebug>

extern qint8 soundSwitch;
serial::serial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);
    //固定窗口大小
    this->setFixedSize( this->width(),this->height());

    QTimer *qtimer1=new QTimer(this);
    QTimer *qtimer2=new QTimer(this);
    connect(qtimer1,SIGNAL(timeout()),this,SLOT(showTime()));
    connect(qtimer2,SIGNAL(timeout()),this,SLOT(showDataCount()));
    qtimer1->start(1000);
    qtimer2->start(500);
    ui->textEdit_recv->append(QString("检测到端口列表:"));

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(info.portName());
            ui->textEdit_recv->append(info.portName());
            ui->textEdit_recv->append(info.description());
            ui->textEdit_recv->append(info.manufacturer());
            serial.close();
        }
    }
    if(ui->comboBox->count()==0)ui->textEdit_recv->append(QString("未检测到串口设备！"));
    ui->comboBox_2->setCurrentIndex(3);//默认9600
    ui->pushButton_switch->setText(QString("打开"));
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_relay1->setEnabled(false);
    ui->pushButton_relay2->setEnabled(false);
    ui->pushButton_relay3->setEnabled(false);
    ui->pushButton_relay4->setEnabled(false);
    ui->pushButton_clear_2->setEnabled(false);
    ui->pushButton_clear1->setEnabled(false);
    ui->pushButton_clear2->setEnabled(false);
    ui->pushButton_clear3->setEnabled(false);
    ui->pushButton_clear4->setEnabled(false);
    ui->pushButton_get1->setEnabled(false);
    ui->pushButton_get2->setEnabled(false);
    ui->pushButton_get3->setEnabled(false);
    ui->pushButton_get4->setEnabled(false);
    ui->checkBox->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->checkBox_3->setEnabled(false);
    ui->checkBox_4->setEnabled(false);
}

serial::~serial()
{
    delete ui;
}


void serial::my_readCnt()
{
    char firstChar=0;
    char cntData1=0;//低8b
    char cntData2=0; //高8b
    int cnt=0;
    int cnt1=0;
    int cnt2=0;
    char otherChar=0;
    QString str;
    if(my_serialport->getChar(&firstChar)==true)
    {
        cntRecvData++;
        if(firstChar=='x'||firstChar=='y'||firstChar=='w'||firstChar=='z')
        {
            if(my_serialport->getChar(&cntData1)!=true)
                 ui->textEdit_recv->append(tr("统计数据丢失！"));
            if(my_serialport->getChar(&cntData2)!=true)
                 ui->textEdit_recv->append(tr("统计数据丢失！"));
            cntRecvData++;
            //数据转换，待修改，暂时这么处理。。
            if(cntData1<0)cnt1=cntData1+256;
            else cnt1=cntData1;
            if(cntData2<0)cnt1=cntData2+256;
            else cnt2=cntData2;
            cnt=cnt1+(cnt2<<8);//拼接成16位数据
            switch(firstChar)
            {
                 case 'x': ui->lcdNumber_1->display(cnt);
                           ui->textEdit_recv->append(QString("光耦通道1:")+
                                                     QString::number(cnt));
                           cntNum1=cnt;
                           break;
                 case 'y': ui->lcdNumber_2->display(cnt);
                           ui->textEdit_recv->append(QString("光耦通道2:")+
                                                     QString::number(cnt));
                           cntNum2=cnt;
                           break;
                 case 'w': ui->lcdNumber_3->display(cnt);
                           ui->textEdit_recv->append(QString("光耦通道3:")+
                                                     QString::number(cnt));
                           cntNum3=cnt;
                           break;
                 case 'z': ui->lcdNumber_4->display(cnt);
                           ui->textEdit_recv->append(QString("光耦通道4:")+
                                                     QString::number(cnt));
                           cntNum4=cnt;
                           break;
                 default:break;
            }
        }
        else
        {
            str+=QString(firstChar);
            while(my_serialport->getChar(&otherChar)&&otherChar!='\n')
            {
                str+=QString(otherChar);
            }
            ui->textEdit_recv->setText(str);
        }
    }
}

void serial::on_pushButton_switch_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn2.wav");
    if(ui->pushButton_switch->text()==QString("打开"))
    {

        //检测该设备是否仍处于连接（7-26更新：检测优化，避免打开串口前设备失去连接）
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            if(info.portName()==ui->comboBox->currentText())goto OPEN;//若设备确实还处于连接，则开始打开该串口
        }
        //否则提示
        QMessageBox::StandardButton reply;
        reply= QMessageBox::information(this,"提示","未检测到串口设备，请检查连接后重试！",QMessageBox::Cancel);
        if(reply==QMessageBox::Cancel)
             goto CLOSE;


OPEN:
        my_serialport= new QSerialPort();
        my_serialport->setPortName(ui->comboBox->currentText());
        my_serialport->open(QIODevice::ReadWrite);
        my_serialport->setBaudRate(ui->comboBox_2->currentText().toInt());
        switch(ui->comboBox_3->currentIndex()){
        case 0: my_serialport->setParity(QSerialPort::NoParity);break;
        case 1: my_serialport->setParity(QSerialPort::OddParity);break;
        case 2: my_serialport->setParity(QSerialPort::EvenParity);break;
        default: break;
        }
        switch(ui->comboBox_4->currentIndex()){
        case 0: my_serialport->setDataBits(QSerialPort::Data8);break;
        case 1: my_serialport->setDataBits(QSerialPort::Data7);break;
        case 2: my_serialport->setDataBits(QSerialPort::Data6);break;
        default: break;
        }
        switch(ui->comboBox_5->currentIndex()){
        case 0: my_serialport->setStopBits(QSerialPort::OneStop);break;
        case 1: my_serialport->setStopBits(QSerialPort::TwoStop);break;
        default: break;
        }
        my_serialport->setFlowControl(QSerialPort::NoFlowControl);
        //接收串口数据并显示
       // connect(my_serialport,SIGNAL(readyRead()),this,SLOT(my_readUart()));
        //接收光耦统计值并显示
        connect(my_serialport,SIGNAL(readyRead()),this,SLOT(my_readCnt()));

        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->comboBox_4->setEnabled(false);
        ui->comboBox_5->setEnabled(false);
        ui->pushButton_flush->setEnabled(false);
        ui->label_led->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->pushButton_switch->setText(QString("关闭"));
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_relay1->setEnabled(true);
        ui->pushButton_relay2->setEnabled(true);
        ui->pushButton_relay3->setEnabled(true);
        ui->pushButton_relay4->setEnabled(true);
        ui->pushButton_clear_2->setEnabled(true);
        ui->pushButton_clear1->setEnabled(true);
        ui->pushButton_clear2->setEnabled(true);
        ui->pushButton_clear3->setEnabled(true);
        ui->pushButton_clear4->setEnabled(true);
        ui->pushButton_get1->setEnabled(true);
        ui->pushButton_get2->setEnabled(true);
        ui->pushButton_get3->setEnabled(true);
        ui->pushButton_get4->setEnabled(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(true);
        ui->checkBox_4->setEnabled(true);
        ui->textEdit_recv->append(QString("当前连接上：")+my_serialport->portName());

    }
    else {
        my_serialport->clear();
        my_serialport->deleteLater();
        //  my_serialport->deleteLater();
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        ui->pushButton_flush->setEnabled(true);
        ui->label_led->setStyleSheet("background-color:red");

        ui->pushButton_switch->setText(QString("打开"));
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_relay1->setEnabled(false);
        ui->pushButton_relay2->setEnabled(false);
        ui->pushButton_relay3->setEnabled(false);
        ui->pushButton_relay4->setEnabled(false);
        ui->pushButton_clear_2->setEnabled(false);
        ui->pushButton_clear1->setEnabled(false);
        ui->pushButton_clear2->setEnabled(false);
        ui->pushButton_clear3->setEnabled(false);
        ui->pushButton_clear4->setEnabled(false);
        ui->pushButton_get1->setEnabled(false);
        ui->pushButton_get2->setEnabled(false);
        ui->pushButton_get3->setEnabled(false);
        ui->pushButton_get4->setEnabled(false);
        ui->checkBox->setEnabled(false);
        ui->checkBox_2->setEnabled(false);
        ui->checkBox_3->setEnabled(false);
        ui->checkBox_4->setEnabled(false);

    }

CLOSE:  ;

}

void serial::on_pushButton_2_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    my_serialport->write(ui->lineEdit_send->text().toLatin1().data());
    cntSendData+=ui->lineEdit_send->text().toLatin1().size();
    if(ui->lineEdit_send->text()==QString('a'))
    {
        statusFlag1++;
        if(statusFlag1%2==1)ui->label_status1->setStyleSheet("QLabel{border-image: url(:/status/status/greenstatus.png)}");
        else ui->label_status1->setStyleSheet("QLabel{border-image: url(:/status/status/redstatus.png)}");
    }
    else if(ui->lineEdit_send->text()==QString('b'))
    {
        statusFlag2++;
        if(statusFlag2%2==1)ui->label_status2->setStyleSheet("QLabel{border-image: url(:/status/status/greenstatus.png)}");
        else ui->label_status2->setStyleSheet("QLabel{border-image: url(:/status/status/redstatus.png)}");
    }
    else if(ui->lineEdit_send->text()==QString('c'))
    {
        statusFlag3++;
        if(statusFlag3%2==1)ui->label_status3->setStyleSheet("QLabel{border-image: url(:/status/status/greenstatus.png)}");
        else ui->label_status3->setStyleSheet("QLabel{border-image: url(:/status/status/redstatus.png)}");
    }
    else if(ui->lineEdit_send->text()==QString('d'))
    {
        statusFlag4++;
        if(statusFlag4%2==1)ui->label_status4->setStyleSheet("QLabel{border-image: url(:/status/status/greenstatus.png)}");
        else ui->label_status4->setStyleSheet("QLabel{border-image: url(:/status/status/redstatus.png)}");
    }
    ui->lineEdit_send->clear();
}

void serial::on_pushButton_relay1_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    my_serialport->putChar('a');
    cntSendData++;
    statusFlag1++;
    if(statusFlag1%2==1)ui->label_status1->setStyleSheet("QLabel{border-image: url(:/status/status/greenstatus.png)}");
    else ui->label_status1->setStyleSheet("QLabel{border-image: url(:/status/status/redstatus.png)}");
}

void serial::on_pushButton_relay2_clicked()
{
     if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
     my_serialport->putChar('b');
     cntSendData++;
     statusFlag2++;
     if(statusFlag2%2==1)ui->label_status2->setStyleSheet("QLabel{border-image: url(:/status/status/greenstatus.png)}");
     else ui->label_status2->setStyleSheet("QLabel{border-image: url(:/status/status/redstatus.png)}");

}

void serial::on_pushButton_relay3_clicked()
{
     if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
     my_serialport->putChar('c');
     cntSendData++;
     statusFlag3++;
     if(statusFlag3%2==1)ui->label_status3->setStyleSheet("QLabel{border-image: url(:/status/status/greenstatus.png)}");
     else ui->label_status3->setStyleSheet("QLabel{border-image: url(:/status/status/redstatus.png)}");

}


void serial::on_pushButton_relay4_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    my_serialport->putChar('d');
    cntSendData++;
    statusFlag4++;
    if(statusFlag4%2==1)ui->label_status4->setStyleSheet("QLabel{border-image: url(:/status/status/greenstatus.png)}");
    else ui->label_status4->setStyleSheet("QLabel{border-image: url(:/status/status/redstatus.png)}");
}


void serial::showTime()
{
    QDateTime time=QDateTime::currentDateTime();
    QString textTime=time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->label_time->setText(textTime);
}

void serial::on_pushButton_clear_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    ui->textEdit_recv->clear();
}

void serial::showDataCount()
{
    QString s1 = QString::number(cntRecvData, 10);
    QString s2 = QString::number(cntSendData, 10);
    ui->label_sendCount->setText(s1);
    ui->label_recvCount->setText(s2);
}



void serial::on_pushButton_7_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    about=new AboutDialog(this);
    about->show();
}

void serial::on_pushButton_5_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    reportNum++;
    QDir dir;
    dir.mkdir("C:/com_report");
    QFile myFile("C:/com_report/report"+QString::number(reportNum)+".txt");
    if(!myFile.open(QFile::WriteOnly|QFile::Text))//写
    {
        if(!soundSwitch)QSound::play(":/prompt/sound/wrong.wav");
        QMessageBox::information(this,"生成报告","报告生成失败！");
        exit(1);
    }
    QDateTime time_rp=QDateTime::currentDateTime();
    QString textTime_rp=time_rp.toString("yyyy-MM-dd hh:mm:ss dddd");

    QTextStream out(&myFile);

    out<<QStringLiteral("报告生成日期：")<<textTime_rp<<endl
       <<"-----------------------------"<<endl
       <<QStringLiteral("光耦统计结果:")<<endl
       <<QStringLiteral("通道1：")<<QString::number(cntNum1)<<endl
       <<QStringLiteral("通道2：")<<QString::number(cntNum2)<<endl
       <<QStringLiteral("通道3：")<<QString::number(cntNum3)<<endl
       <<QStringLiteral("通道4：")<<QString::number(cntNum4)<<endl
       <<QStringLiteral("继电器翻转次数:")<<endl
       <<QStringLiteral("继电器1：")<<statusFlag1<<endl
       <<QStringLiteral("继电器2：")<<statusFlag2<<endl
       <<QStringLiteral("继电器3：")<<statusFlag3<<endl
       <<QStringLiteral("继电器4：")<<statusFlag4<<endl
       <<QStringLiteral("串口通信数据流：")<<endl
       <<QStringLiteral("本次通信发送数据共计：")<<cntSendData<<endl
       <<QStringLiteral("本次通信接收数据共计：")<<cntRecvData<<endl
       <<"-----------------------------"<<endl;

    out.flush();
    myFile.close();
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"生成报告",
                             "已生成报告文件\n默认路径：C:/com_report\n是否查看报告？",QMessageBox::Yes|QMessageBox::No);
    //报告路径格式转换
    QString temp=QString("C:/com_report/report");
    temp=temp+QString::number(reportNum)+".txt";
    QByteArray ba = temp.toLatin1();
    char *url = ba.data();
    if(reply==QMessageBox::Yes)
     {
       QDesktopServices::openUrl ( QUrl::fromLocalFile(url) );//打开报告
       if(!soundSwitch)QSound::play(":/prompt/sound/success.wav");
       QFileDialog::getOpenFileName(this,nullptr,"C:/com_report");//打开文件浏览器展示路径
     }

}

void serial::on_pushButton_set_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    set=new SetDialog(this);
    set->show();
}

void serial::on_pushButton_help_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    help=new HelpDialog(this);
    help->show();
}


//7-26更新，加入手动刷新功能：重新检测串口设备
void serial::on_pushButton_flush_clicked()
{
    ui->comboBox->clear();//清空初始检测结果，重新检测
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(info.portName());
            ui->textEdit_recv->append(info.portName());
            ui->textEdit_recv->append(info.description());
            ui->textEdit_recv->append(info.manufacturer());
            serial.close();
        }
    }
}

void serial::on_pushButton_clear1_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    ui->lcdNumber_1->display(0);
}

void serial::on_pushButton_clear2_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    ui->lcdNumber_2->display(0);
}

void serial::on_pushButton_clear3_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    ui->lcdNumber_3->display(0);
}

void serial::on_pushButton_clear4_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    ui->lcdNumber_4->display(0);
}

void serial::on_pushButton_clear_2_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    ui->lcdNumber_1->display(0);
    ui->lcdNumber_2->display(0);
    ui->lcdNumber_3->display(0);
    ui->lcdNumber_4->display(0);
}

void serial::on_pushButton_get1_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    my_serialport->putChar('x');
    cntSendData++;
}

void serial::on_pushButton_get2_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    my_serialport->putChar('y');
    cntSendData++;
}

void serial::on_pushButton_get3_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    my_serialport->putChar('w');
    cntSendData++;
}

void serial::on_pushButton_get4_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn1.wav");
    my_serialport->putChar('z');
    cntSendData++;
}
