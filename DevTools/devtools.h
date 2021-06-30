#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_devtools.h"
#include "regex_tool/regex_tool.h"
#include "json_tool/json_tool.h"
#include "base64_tool/base64_tool.h"
#include "net_tool/net_tool.h"


class DevTools : public QMainWindow
{
    Q_OBJECT

public:
    DevTools(QWidget *parent = Q_NULLPTR);

    void on_btn_regex_clicked();
    void on_btn_json_clicked();
    void on_btn_base64_clicked();
    void on_btn_net_clicked();

private:
    Ui::DevToolsClass ui;

    RegexTool regex_tool;
    JsonTool json_tool;
    Base64Tool base64_tool;
    NetTool net_tool;
};
