#include <QPushButton>

#include "devtools.h"

DevTools::DevTools(QWidget *parent): QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btn_regex, &QPushButton::clicked, this, &DevTools::on_btn_regex_clicked);
}

void DevTools::on_btn_regex_clicked()
{
    regex_tool.show();
}
