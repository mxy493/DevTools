﻿#pragma once

#include <QWidget>
#include <QNetworkReply>
#include <QProcess>

#include "ui_net_tool.h"

class NetTool : public QWidget
{
    Q_OBJECT

public:
    NetTool(QWidget *parent = Q_NULLPTR);
    void update_geoip();

public slots:
    void update_interface_info(int index);
    void update_ip(QNetworkReply *reply);
    void ping();
    void clear();

private:
    Ui::NetToolClass ui;
    QProcess qprocess;
};
