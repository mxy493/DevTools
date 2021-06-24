#pragma once

#include <QWidget>
#include "ui_json_tool.h"

class JsonTool : public QWidget
{
    Q_OBJECT

public:
    JsonTool(QWidget *parent = Q_NULLPTR);

public slots:
    void format();
    void rmformat();
    void save();
    void find_next();
    void find_prev();

private:
    Ui::JsonToolClass ui;
};
