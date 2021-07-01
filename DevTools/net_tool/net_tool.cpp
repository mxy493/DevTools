#include <QNetworkInterface>
#include <QComboBox>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QLineEdit>

#include "net_tool.h"
#include "../utils.h"

extern QJsonObject config;

NetTool::NetTool(QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.combo_interfaces,
        static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged),
        this, &NetTool::update_interface_info);
    connect(ui.btn_ping, &QPushButton::clicked, this, &NetTool::ping);
    connect(ui.line_ping, &QLineEdit::returnPressed, this, &NetTool::ping);
    connect(&qprocess, &QProcess::readyReadStandardOutput, [&]() {
        QString output = QString::fromLocal8Bit(qprocess.readAllStandardOutput());
        ui.text_edit->appendPlainText(output);
        });
    connect(ui.btn_clear, &QPushButton::clicked, this, &NetTool::clear);

    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    QString _default = "WLAN";
    if (config.contains("NetTool") && config["NetTool"].toObject().contains("network_interface"))
    {
        _default = config["NetTool"].toObject().value("network_interface").toString();
    }
    for (int i = 0; i < interfaces.size(); ++i)
    {
        QString name = interfaces[i].humanReadableName();
        ui.combo_interfaces->addItem(name, interfaces[i].index());
        if (name == _default)
        {
            ui.combo_interfaces->setCurrentIndex(i);
        }
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NetTool::update_ip);
    manager->get(QNetworkRequest(QUrl("http://api.ipaddress.com/myip?format=json")));
}

void NetTool::update_interface_info(int index)
{
    index = ui.combo_interfaces->currentData().toInt();
    QNetworkInterface interface = QNetworkInterface::interfaceFromIndex(index);
    QString mac = interface.hardwareAddress();
    ui.label_mac->setText(mac);

    QJsonObject obj
    {
        { "NetTool", QJsonObject({
            {"network_interface", interface.humanReadableName()}})
        }
    };
    save_config(obj);
}

void NetTool::update_ip(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        ui.label_public_ip->setText(obj.value("ipaddress").toString());
        ui.label_proxy_ip->setText(obj.value("proxy").toString());
    }
    reply->deleteLater();
    update_geoip();
}

void NetTool::update_geoip()
{
    QString ip = ui.label_public_ip->text();
    QString cmd = "./bin/maxmind/mmdbinspect.exe -db ./data/GeoLite2-Country.mmdb " + ip;
    QProcess process_geoip;
    process_geoip.start(cmd);
    process_geoip.waitForFinished();
    QString output = QString::fromUtf8(process_geoip.readAllStandardOutput());

    QJsonDocument doc = QJsonDocument::fromJson(output.toUtf8());
    if (doc.isArray())
    {
        QJsonArray array = doc.array();
        if (!array.isEmpty() && array.first().isObject())
        {
            // "Records": []
            QJsonObject obj = array.first().toObject();
            if (obj.contains("Records") && obj.value("Records").isArray())
            {
                array = obj.value("Records").toArray();
                if (!array.isEmpty() && array.first().isObject())
                {
                    // "Record": {}
                    obj = array.first().toObject();
                    QString location = obj.value("Record").toObject().value("country").toObject()
                        .value("names").toObject().value("zh-CN").toString();
                    ui.label_location->setText(location);
                }
            }
        }
    }
}

void NetTool::ping()
{
    QString cmd = "ping " + ui.line_ping->text();
    ui.text_edit->clear();

    qprocess.start(cmd);
}

void NetTool::clear()
{
    ui.line_ping->clear();
    ui.text_edit->clear();
    ui.line_ping->setFocus();
}