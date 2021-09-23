#ifndef DEVICE_H
#define DEVICE_H
#include <QString>
#include <QList>
#include <KDQName>
#include <QUrl>

struct Device
{
    QString uuid;
    QString deviceType;
    QList<KDQName> typeList;
    QList<QUrl> scopeList;
    QList<QUrl> xAddrList;
};
#endif // DEVICE_H
