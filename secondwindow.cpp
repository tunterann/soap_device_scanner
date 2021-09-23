#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "device.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QAction>
#include <qdebug.h>
#include <QClipboard>

Device device;

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
}

void SecondWindow::setData(const Device target_device)
{
    device = target_device;
    std::for_each(device.typeList.begin(), device.typeList.end(), [this](KDQName &name){
        ui->listWidget->addItem("Type " + name.localName() + " in namespace " + name.nameSpace());
    });
    std::for_each(device.scopeList.begin(), device.scopeList.end(), [this](QUrl &scope){
        ui->listWidget_2->addItem(scope.toString());
    });
    std::for_each(device.xAddrList.begin(), device.xAddrList.end(), [this](QUrl &xAddr){
        ui->listWidget_3->addItem(xAddr.toString());
    });

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(customMenuRequested(const QPoint&)));

    ui->listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_2, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(customMenuRequested(const QPoint&)));

    ui->listWidget_3->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_3, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(customMenuRequested(const QPoint&)));

}

void SecondWindow::customMenuRequested(QPoint pos){
    QListWidget *targetList = qobject_cast<QListWidget *>(QObject::sender());
    QPoint globalPos = targetList->mapToGlobal(pos);
       QMenu myMenu;
       myMenu.addAction("Copy to clipboard");

       QAction* selectedItem = myMenu.exec(globalPos);
       if (selectedItem)
       {
           QClipboard *clipboard = QApplication::clipboard();
           clipboard->setText(targetList->itemAt(pos)->text());
       }

 }

SecondWindow::~SecondWindow()
{
    delete ui;
}
