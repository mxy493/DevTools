#pragma once

#include <QWidget>
#include "ui_digit_tool.h"

class DigitTool : public QWidget
{
    Q_OBJECT

public:
    DigitTool(QWidget *parent = Q_NULLPTR);
    void update_digit(qulonglong number);
    void clear_digit();

public slots:
    void on_lineEdit_2_text_changed();
    void on_lineEdit_8_text_changed();
    void on_lineEdit_10_text_changed();
    void on_lineEdit_16_text_changed();

private:
    Ui::DigitToolClass ui;
};
