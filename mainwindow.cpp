#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QListWidget"
#include "QListWidgetItem"
#include "secondwindow.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QDebug>
#include <QSharedPointer>
#include <WSDiscoveryClient>
#include <WSDiscoveryProbeJob>
#include <WSDiscoveryTargetService>
#include <WSDiscoveryServiceAggregator>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QList>
#include <KDQName>

QList<Device> devices;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setColumnWidth(0, this->width()/2);

    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(doSomething(int, int)));

    m_client = new WSDiscoveryClient(this);
    m_probeJob = new WSDiscoveryProbeJob(m_client);
    m_aggregator = new WSDiscoveryServiceAggregator(this);

    connect(m_probeJob, &WSDiscoveryProbeJob::matchReceived, m_aggregator, &WSDiscoveryServiceAggregator::updateService);
    connect(m_aggregator, &WSDiscoveryServiceAggregator::serviceUpdated, this, &MainWindow::checkDevices);

    m_client->start();

}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    ui->tableWidget->setColumnWidth(0, this->width()/2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    if(ui->pushButton->text() == "Stop scan")
    {
        m_probeJob->stop();
        ui->pushButton->setText("Start scan");
        ui->statusbar->showMessage("Scanning stoped");
    }
    else
    {
        ui->tableWidget->setRowCount(0);
        ui->pushButton->setText("Stop scan");
        ui->statusbar->showMessage("Scanning started");
        m_probeJob->start();
    }
}

void MainWindow::checkDevices(const QSharedPointer<WSDiscoveryTargetService> &matchedService)
{


    Device new_device;
    new_device.uuid = matchedService->endpointReference();
    new_device.typeList = matchedService->typeList();
    new_device.scopeList = matchedService->scopeList();
    new_device.xAddrList = matchedService->xAddrList();

    QStringList types;
    std::transform(new_device.typeList.begin(),
                       new_device.typeList.end(),
                       std::back_inserter(types),
                       [](const KDQName &type){ return type.localName(); }
    );
    new_device.deviceType = types.join(" ");

    QString uuid = new_device.uuid;
    auto iterator = std::find_if(
      devices.begin(), devices.end(),
      [&uuid](Device x) {
            return x.uuid == uuid;
        }
    );

    if (iterator != devices.end())
    {
        return;
    }
    else
    {
        devices.append(new_device);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int target_row = ui->tableWidget->rowCount()-1;
        ui->tableWidget->setItem(target_row,0, new QTableWidgetItem(new_device.deviceType));
        ui->tableWidget->setItem(target_row,1, new QTableWidgetItem(new_device.uuid));
    }

}

void MainWindow::doSomething(int row, int column)
{
    Device target_device = devices[row];
    window = new SecondWindow(this);
    window->setData(target_device);
    window->setWindowTitle(target_device.uuid);
    window->show();
}
