#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <QSound>

extern qint8 soundSwitch;

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    ui->listWidget->addItem("串口配置");
    ui->listWidget->addItem("串口终端");
    ui->listWidget->addItem("继电器控制");
    ui->listWidget->addItem("光耦计数");
    ui->listWidget->addItem("生成报告");
    ui->listWidget->addItem("设置");
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
   if(!soundSwitch)QSound::play(":/btn/sound/btn3.wav");
   ui->label_help->setText(current->text());
   if(current->text()==QString("串口配置"))
   {
       ui->textBrowser_help->clear();
       ui->textBrowser_help->append(
                               QString( "串口推荐配置如下：\n\n串口号：默认串口号\n（软件能自动识别活动的串口设备，请选择可连接的串口。）\n\n")
                              +QString("波特率：9600/38400\n（根据通信速率要求选择，测试结果：低速通信9600较稳定，高速通信38400较稳定，可根据实际需求选择。）\n\n")
                              +QString("校验位：NONE\n（奇校验或偶校验，一般不用。）\n\n")
                              +QString("数据位：8\n\n")
                              +QString("停止位：1\n\n")
                              +QString("----------------------------\n")
                              +QString("打开/关闭：打开或关闭串口设备\n")
                              +QString("刷新：用于连接发生变化时重新检测串口设备")
                              );

   }
   else if(current->text()==QString("串口终端"))
   {
       ui->textBrowser_help->clear();
       ui->textBrowser_help->append(
                               QString("串口终端包括接收区和发送区：\n\n")
                              +QString("接收区：\n显示接收到的数据，为方便串口解析指令，接收为单字节接收并打印。\n")
                              +QString("    光耦计数数据会实时在该窗口打印，上位机发送成功的数据，下位机回传也会打印在该区表示接收成功。\n")
                              +QString("    清空操作：点击清空显示图标可清空当前显示\n\n")
                              +QString("发送区：\n发送数据，同样以单字节的方式发送，支持字符指令。\n")
                              +QString("    已支持的字符指令：\n\n")
                              +QString("    ‘a’---控制继电器1翻转（与点击继电器1按钮效果相同）\n")
                              +QString("    ‘b’---控制继电器2翻转（与点击继电器2按钮效果相同）\n")
                              +QString("    ‘c’---控制继电器3翻转（与点击继电器3按钮效果相同）\n")
                              +QString("    ‘d’---控制继电器4翻转（与点击继电器4按钮效果相同）\n")
                              +QString("    ‘x’---获取光耦通道1的计数值（与点击通道1获取按钮效果相同）\n")
                              +QString("    ‘y’---获取光耦通道2的计数值（与点击通道1获取按钮效果相同）\n")
                              +QString("    ‘w’---获取光耦通道3的计数值（与点击通道1获取按钮效果相同）\n")
                              +QString("    ‘z’---获取光耦通道4的计数值（与点击通道1获取按钮效果相同）\n")
                              +QString("    为保证指令有效，建议单字符发送，避免类似“aabd”式的指令发送n\n")
                              );
   }
   else if(current->text()==QString("继电器控制"))
   {
       ui->textBrowser_help->clear();
       ui->textBrowser_help->append(
                               QString("支持指令和按钮两种方式操作（指令格式见<串口终端>帮助信息）\n\n")
                              +QString("指示灯用于显示继电器打开和关闭情况：\n绿灯：打开；红灯：关闭\n\n")
                              +QString("可选态用于开启和关闭继电器第三种状态。\n\n")
                              );
   }
   else if(current->text()==QString("光耦计数"))
   {
       ui->textBrowser_help->clear();
       ui->textBrowser_help->append(
                               QString("支持指令和按钮两种方式操作（指令格式见<串口终端>帮助信息）\n\n")
                              +QString("数值会打印在LcdNumber控件和串口终端接收区，数据解析方式为单字节，最大支持16位计数值。\n\n")
                              );
   }
   else if(current->text()==QString("生成报告"))
   {
       ui->textBrowser_help->clear();
       ui->textBrowser_help->append(
                               QString("生成本次操作的报告文本\n\n")
                              +QString("报告格式：.txt文件\n\n")
                              +QString("报告内容：包括报告日期、光耦计数结果、继电器翻转次数、串口数据流信息等\n\n")
                              +QString("生成报告：点击主界面的生成报告按钮会自动生成本次操作的报告，第一次使用会创建报告目录，默认路径为：C:/com_report/\n\n")
                              +QString("编辑报告：生成报告后选择查看会自动调用系统记事本打开，可以进行修改、保存等操作；也可在更多设置-编辑报告文件中打开\n\n")

                              );
   }
   else if(current->text()==QString("设置"))
   {
       ui->textBrowser_help->clear();
       ui->textBrowser_help->append(
                               QString("主界面点击更多设置按钮\n\n")
                              +QString("设定音效：点击关闭或打开全局音效（默认打开）\n\n")
                              +QString("编辑报告：返回报告所在路径的文件对话框\n\n")
                              +QString("安全退出：结束进程并退出\n\n")
                              );
   }
}
