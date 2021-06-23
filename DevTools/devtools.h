#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_devtools.h"
#include "regex_tool/regex_tool.h"

class DevTools : public QMainWindow
{
    Q_OBJECT

public:
    DevTools(QWidget *parent = Q_NULLPTR);

    void on_btn_regex_clicked();

private:
    Ui::DevToolsClass ui;

    RegexTool regex_tool;
};
