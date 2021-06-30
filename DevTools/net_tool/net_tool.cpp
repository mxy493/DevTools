#include <QNetworkInterface>
#include <QComboBox>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
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
    connect(manager, &QNetworkAccessManager::finished, this, &NetTool::replyFinished);
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

void NetTool::replyFinished(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        ui.label_public_ip->setText(obj.value("ipaddress").toString());
        ui.label_proxy_ip->setText(obj.value("proxy").toString());
    }
    reply->deleteLater();
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