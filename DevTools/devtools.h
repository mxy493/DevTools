#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_devtools.h"


class DevTools : public QMainWindow
{
    Q_OBJECT

public:
    DevTools(QWidget *parent = Q_NULLPTR);

    void on_btn_regex_clicked();
    void on_btn_json_clicked();
    void on_btn_base64_clicked();
    void on_btn_net_clicked();
    void on_btn_digit_clicked();

private:
    Ui::DevToolsClass ui;
};
