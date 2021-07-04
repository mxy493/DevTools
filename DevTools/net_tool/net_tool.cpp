#include <QNetworkInterface>
#include <QComboBox>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include <QRegularExpression>

#include "net_tool.h"
#include "../utils.h"

extern QJsonObject config;

NetTool::NetTool(QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);

    ui.line_ip->setFocus();

    connect(ui.combo_interfaces,
        static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged),
        this, &NetTool::update_interface_info);
    connect(ui.btn_query_ip_info, &QPushButton::clicked, this, &NetTool::query_ip_info);
    connect(&manager, &QNetworkAccessManager::finished, this, &NetTool::update_ip_info);
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

    query_ip_info();
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

void NetTool::query_ip_info()
{
    ui.label_info->setText(u8"查询中！");
    QString url("http://ifconfig.co/json");
    QString ip = ui.line_ip->text();
    if (!ip.isEmpty())
    {
        // 判断是否为合法的IPv4地址
        QRegularExpression re("/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"
            "\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"
            "\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"
            "\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/");
        if (re.match(ip).hasMatch())
        {
            url += "?ip=" + ip;
        }
        else
        {
            ui.label_info->setText(u8"不是合法的IPv4地址，无法查询！");
            QTimer::singleShot(3000, [&]() { ui.label_info->setText(u8""); });
            return;
        }
    }
    manager.get(QNetworkRequest(QUrl(url)));
}

void NetTool::update_ip_info(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        if (ui.line_ip->text().isEmpty())
        {
            ui.line_ip->setText(obj.value("ip").toString());
        }
        ui.label_decimal->setText(QString::number(obj.value("ip_decimal").toVariant().toLongLong()));
        ui.label_country->setText(obj.value("country").toString());
        ui.label_country_iso->setText(obj.value("country_iso").toString());
        ui.label_region->setText(obj.value("region_name").toString());
        ui.label_region_code->setText(obj.value("region_code").toString());
        ui.label_city->setText(obj.value("city").toString());
        ui.label_latitude->setText(QString::number(obj.value("latitude").toDouble()));
        ui.label_longitude->setText(QString::number(obj.value("longitude").toDouble()));
        ui.label_timezone->setText(obj.value("time_zone").toString());
        ui.label_asn->setText(obj.value("asn").toString());
        ui.label_asn_org->setText(obj.value("asn_org").toString());
    }
    reply->deleteLater();

    ui.label_info->setText(u8"已更新！");
    QTimer::singleShot(1000, [&]() { ui.label_info->setText(u8""); });
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