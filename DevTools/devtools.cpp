#include <QPushButton>

#include "devtools.h"
#include "utils.h"
#include "regex_tool/regex_tool.h"
#include "json_tool/json_tool.h"
#include "base64_tool/base64_tool.h"
#include "net_tool/net_tool.h"
#include "digit_tool/digit_tool.h"


DevTools::DevTools(QWidget *parent): QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btn_regex, &QPushButton::clicked, this, &DevTools::on_btn_regex_clicked);
    connect(ui.btn_json, &QPushButton::clicked, this, &DevTools::on_btn_json_clicked);
    connect(ui.btn_base64, &QPushButton::clicked, this, &DevTools::on_btn_base64_clicked);
    connect(ui.btn_net, &QPushButton::clicked, this, &DevTools::on_btn_net_clicked);
    connect(ui.btn_digit, &QPushButton::clicked, this, &DevTools::on_btn_digit_clicked);
}

void DevTools::on_btn_regex_clicked()
{
    RegexTool *regex_tool = new RegexTool();
    regex_tool->showNormal();

    this->showMinimized();
}

void DevTools::on_btn_json_clicked()
{
    JsonTool *json_tool = new JsonTool();
    json_tool->showNormal();

    this->showMinimized();
}

void DevTools::on_btn_base64_clicked()
{
    Base64Tool *base64_tool = new Base64Tool();
    base64_tool->showNormal();

    this->showMinimized();
}

void DevTools::on_btn_net_clicked()
{
    NetTool *net_tool = new NetTool();
    net_tool->showNormal();

    this->showMinimized();
}

void DevTools::on_btn_digit_clicked()
{
    DigitTool *digit_tool = new DigitTool();
    digit_tool->showNormal();

    this->showMinimized();
}
