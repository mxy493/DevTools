#pragma once

#include <QWidget>
#include "ui_regex_tool.h"

class RegexTool : public QWidget
{
    Q_OBJECT

public:
    RegexTool(QWidget *parent = Q_NULLPTR);

public slots:
    void match();

private:
    Ui::RegexToolClass ui;
};
