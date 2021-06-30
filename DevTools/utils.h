#pragma once

#include <QJsonObject>

extern QJsonObject config;

void load_config();

void save_config(QJsonObject &json);