#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QTextStream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "json_tool.h"

JsonTool::JsonTool(QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.btn_format, &QPushButton::clicked, this, &JsonTool::format);
    connect(ui.btn_rmformat, &QPushButton::clicked, this, &JsonTool::rmformat);
    connect(ui.btn_save, &QPushButton::clicked, this, &JsonTool::save);
    connect(ui.btn_next, &QPushButton::clicked, this, &JsonTool::find_next);
    connect(ui.btn_prev, &QPushButton::clicked, this, &JsonTool::find_prev);
}

void JsonTool::format()
{
    QString src = ui.te_src->toPlainText();

    json json_obj = json::parse(src.toUtf8().data(), nullptr, false);
    if (json_obj.is_discarded())
    {
        QMessageBox msg(QMessageBox::Warning, u8"注意", u8"输入的数据不是有效的JSON字符串！");
        msg.exec();
    }
    else
    {
        int indentation = ui.spin_indentation->value();
        std::string str = json_obj.dump(indentation);
        ui.te_dst->setPlainText(QString::fromUtf8(str.c_str()));
    }
}

void JsonTool::rmformat()
{
    ui.spin_indentation->setValue(-1);
    format();
}

void JsonTool::save()
{
    QString now = QDateTime::currentDateTime().toString(QString("yyyy-MM-dd_hh-mm-ss"));
    QString file_name = QString::fromUtf8("DevTools_%1.json").arg(now);
    QString file_path = QFileDialog::getSaveFileName(
        this, QString::fromUtf8(u8"保存"),
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QString("/") + file_name,
        QString::fromUtf8(u8"JSON (*.json)"));
    if (!file_path.isEmpty())
    {
        QFile file(file_path);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            QString text = ui.te_dst->toPlainText();
            out << text;
            out.flush();
        }
        file.close();
    }
}

void JsonTool::find_next()
{
    QString findtext = ui.line_find->text();
    if (ui.te_dst->find(findtext))
    {
        // 查找到后高亮显示
        QPalette palette = ui.te_dst->palette();
        palette.setColor(QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
        ui.te_dst->setPalette(palette);
    }
    else
    {
        QMessageBox::information(this, u8"注意", u8"已到达末尾，没有找到更多内容！", QMessageBox::Ok);
    }
}

void JsonTool::find_prev()
{
    QString findtext = ui.line_find->text();
    if (ui.te_dst->find(findtext, QTextDocument::FindBackward))
    {
        // 查找到后高亮显示
        QPalette palette = ui.te_dst->palette();
        palette.setColor(QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
        ui.te_dst->setPalette(palette);
    }
    else
    {
        QMessageBox::information(this, u8"注意", u8"已到达开头，没有找到更多内容！", QMessageBox::Ok);
    }
}
