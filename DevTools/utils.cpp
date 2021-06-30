#include <QDir>
#include <QJsonDocument>

#include "utils.h"

QJsonObject config;

void load_config()
{
    QFile file("data/config.json");
    if (!file.exists())
    {
        return;
    }
    else if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject())
        {
            config = doc.object();
        }
        file.close();
    }
}

void save_config(QJsonObject &json)
{
    // 更新config配置
    auto it = json.begin();
    for (; it != json.end(); ++it)
    {
        config.insert(it.key(), it.value());
    }

    // 写入文件保存
    QDir dir;
    if (!dir.exists("data"))
    {
        dir.mkdir("data");
    }
    QFile file("data/config.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(QJsonDocument(config).toJson());
        file.flush();
        file.close();
    }
}
