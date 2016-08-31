#include "serial.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    serial w;
    //设置背景图片
    w.setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/background/back/back4.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    w.setPalette(palette);

    w.show();

    return a.exec();
}
