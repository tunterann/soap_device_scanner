#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTableWidget>
#include "secondwindow.h"
#ifndef ONVIFDISCOVER_H
#define ONVIFDISCOVER_H
#include "device.h"
#include <QObject>

class WSDiscoveryClient;
class WSDiscoveryProbeJob;
class WSDiscoveryTargetService;
class WSDiscoveryServiceAggregator;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void checkDevices(const QSharedPointer<WSDiscoveryTargetService>& matchedService);
    void on_pushButton_clicked();
    void doSomething(int row, int column);
    void resizeEvent(QResizeEvent* e);

private:
    Ui::MainWindow *ui;
    SecondWindow *window;
    WSDiscoveryClient * m_client;
    WSDiscoveryProbeJob * m_probeJob;
    WSDiscoveryServiceAggregator * m_aggregator;
};
#endif // MAINWINDOW_H
