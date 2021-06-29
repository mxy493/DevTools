#pragma once

#include <QWidget>
#include "ui_base64_tool.h"

class Base64Tool : public QWidget
{
    Q_OBJECT

public:
    Base64Tool(QWidget *parent = Q_NULLPTR);

public slots:
    void open_file();
    void encode();
    void decode();
    void copy();
    void save();

private:
    Ui::Base64ToolClass ui;
    QByteArray byte_array;
};
