#include "setdialog.h"
#include "ui_setdialog.h"
#include "serial.h"
#include <QSound>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

qint8 soundSwitch=0;//1：关闭；0：打开
SetDialog::SetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);
    if(!soundSwitch)ui->pushButton_sound->setIcon(QIcon(":/new/btn/btn/sound.jpg"));
    else ui->pushButton_sound->setIcon(QIcon(":/new/btn/btn/no_sound.jpg"));
}




SetDialog::~SetDialog()
{
    delete ui;
}

void SetDialog::on_pushButton_report_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"编辑报告",
                             "请先在主窗口生成报告\n报告文件默认路径：C:/com_report\n确定编辑？",QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes)
     {
       if(!soundSwitch)QSound::play(":/prompt/sound/success.wav");
       QFileDialog::getOpenFileName(this,tr("open"),"C:/com_report");//打开文件浏览器展示路径
     }
}

void SetDialog::on_pushButton_quit_clicked()
{
    if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    serial *pSerial = (serial*)parentWidget();
    pSerial->close();
}

void SetDialog::on_pushButton_sound_clicked()
{
    soundSwitch=~soundSwitch;
    if(soundSwitch)ui->pushButton_sound->setIcon(QIcon(":/new/btn/btn/no_sound.jpg"));
    else
    {
        ui->pushButton_sound->setIcon(QIcon(":/new/btn/btn/sound.jpg"));
        if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
    }
}
