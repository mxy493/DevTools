#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QTextStream>
#include <QJsonDocument>
#include <QRegularExpression>

#include "json_tool.h"

JsonTool::JsonTool(QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);
    ui.te_src->setFocus();

    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui.btn_format, &QPushButton::clicked, this, &JsonTool::format);
    connect(ui.btn_rmformat, &QPushButton::clicked, this, &JsonTool::rmformat);
    connect(ui.btn_save, &QPushButton::clicked, this, &JsonTool::save);
    connect(ui.btn_next, &QPushButton::clicked, this, &JsonTool::find_next);
    connect(ui.btn_prev, &QPushButton::clicked, this, &JsonTool::find_prev);
}

void JsonTool::format()
{
    QString src = ui.te_src->toPlainText();
    src.remove(QRegularExpression("\\s"));

    QJsonDocument doc = QJsonDocument::fromJson(src.toUtf8());
    if (!doc.isObject())
    {
        QMessageBox msg(QMessageBox::Warning, u8"注意", u8"输入的数据不是有效的JSON字符串！");
        msg.exec();
    }
    else
    {
        ui.te_dst->clear();
        int indentation = ui.spin_indentation->value();
        if (indentation == -1)
        {
            ui.te_dst->setPlainText(src);
        }
        else
        {
            QVector<QChar> vec;
            bool escape = false;
            for (auto ch : src)
            {
                ui.te_dst->moveCursor(QTextCursor::End);
                if (escape)
                {
                    escape = false;
                    ui.te_dst->insertPlainText(ch);
                }
                else if (ch == '\\')
                {
                    escape = true;
                    ui.te_dst->insertPlainText(ch);
                }
                else if (ch == '{' || ch == '[')
                {
                    vec.push_back(ch);
                    ui.te_dst->insertPlainText(QString(vec.size() * indentation, ' ')
                        .prepend("\n").prepend(ch));
                }
                else if (ch == ':')
                {
                    ui.te_dst->insertPlainText(": ");
                }
                else if (ch == ',')
                {
                    ui.te_dst->insertPlainText(QString(vec.size() * indentation, ' ').prepend(",\n"));
                }
                else if (ch == '}' || ch == ']')
                {
                    vec.pop_back();
                    ui.te_dst->insertPlainText(QString(vec.size() * indentation, ' ')
                        .prepend("\n").append(ch));
                }
                else
                {
                    ui.te_dst->insertPlainText(ch);
                }
            }
        }
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
        ui.te_dst->setFocus();
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
        ui.te_dst->setFocus();
    }
    else
    {
        QMessageBox::information(this, u8"注意", u8"已到达开头，没有找到更多内容！", QMessageBox::Ok);
    }
}
