#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDir>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_commandLinkButton_clicked()
{
    QString url=QDir::currentPath()+"/readme.txt";
    QDesktopServices::openUrl ( QUrl::fromLocalFile(url) );//打开更新说明
}
