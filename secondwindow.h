#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include "device.h"

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();
    void setData(Device target_device);

public slots:
    void customMenuRequested(QPoint pos);

private:
    Ui::SecondWindow *ui;
};

#endif // SECONDWINDOW_H
