#include <QPushButton>

#include "devtools.h"

DevTools::DevTools(QWidget *parent): QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btn_regex, &QPushButton::clicked, this, &DevTools::on_btn_regex_clicked);
    connect(ui.btn_json, &QPushButton::clicked, this, &DevTools::on_btn_json_clicked);
}

void DevTools::on_btn_regex_clicked()
{
    regex_tool.show();
}

void DevTools::on_btn_json_clicked()
{
    json_tool.show();
}
