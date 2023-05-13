#include <QClipboard>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>
#include <QRegularExpression>

#include "base64_tool.h"

Base64Tool::Base64Tool(QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);
    ui.edit_up->setFocus();

    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui.btn_file_select, &QPushButton::clicked, this, &Base64Tool::open_file);
    connect(ui.btn_encode, &QPushButton::clicked, this, &Base64Tool::encode);
    connect(ui.btn_decode, &QPushButton::clicked, this, &Base64Tool::decode);
    connect(ui.btn_copy, &QPushButton::clicked, this, &Base64Tool::copy);
    connect(ui.btn_save, &QPushButton::clicked, this, &Base64Tool::save);
}

void Base64Tool::open_file()
{
    QString file_name = QFileDialog::getOpenFileName(
        this, QString::fromUtf8(u8"保存"),
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    if (!file_name.isEmpty())
    {
        ui.edit_up->setPlainText("[FILE] " + file_name);
        QFile file(file_name);
        if (file.open(QIODevice::ReadOnly))
        {
            byte_array = file.readAll();
        }
    }
}

void Base64Tool::encode()
{
    QString text = ui.edit_up->toPlainText();
    if (text.startsWith("[FILE]"))
    {
        QByteArray ba = byte_array.toBase64();
        ui.edit_down->setPlainText(ba);
    }
    else
    {
        ui.edit_down->setPlainText(text.toUtf8().toBase64());
    }
}

void Base64Tool::decode()
{
    QString text = ui.edit_up->toPlainText();
    byte_array = QByteArray::fromBase64(text.toUtf8());
    bool is_file = false;
    std::string dict[] = { "png", "pdf" };
    QString front = byte_array.left(20);
    QString now = QDateTime::currentDateTime().toString(QString("yyyy-MM-dd_hh-mm-ss"));
    for (auto &suffix : dict)
    {
        if (front.contains(suffix.c_str(), Qt::CaseInsensitive))
        {
            is_file = true;
            QString file_name = QString::fromUtf8("Base64Tool_%1.%2").arg(now).arg(suffix.c_str());
            ui.edit_down->setPlainText("[FILE] " + file_name);
            save();
            break;
        }
    }
    if (!is_file)
    {
        // 匹配非中英文、非数字、非标点符号、非空白符，即匹配乱码
        if (front.contains(QRegularExpression("[^一-龥a-zA-Z0-9\\pP\\s]+")))
        {
            QString file_name = QString::fromUtf8("Base64Tool_%1.binary").arg(now);
            ui.edit_down->setPlainText("[FILE] " + file_name);
            save();
        }
        else
        {
            ui.edit_down->setPlainText(byte_array);
        }
    }
}

void Base64Tool::copy()
{
    QString text = ui.edit_down->toPlainText();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);

    ui.btn_copy->setText(u8"已复制！");
    QTimer::singleShot(1000, ui.btn_copy, [&]() { ui.btn_copy->setText(u8"复制"); });
}

void Base64Tool::save()
{
    QString text = ui.edit_down->toPlainText();
    std::string suffix = "txt";
    QString file_name;
    if (text.startsWith("[FILE]"))
    {
        file_name = text.split(' ').back();
    }
    else
    {
        byte_array = text.toUtf8();
        QString now = QDateTime::currentDateTime().toString(QString("yyyy-MM-dd_hh-mm-ss"));
        file_name = QString::fromUtf8("Base64Tool_%1.%2").arg(now).arg(suffix.c_str());
    }
    QString file_path = QFileDialog::getSaveFileName(
        this, QString::fromUtf8(u8"保存"),
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QString("/") + file_name);
    if (!file_path.isEmpty())
    {
        QFile file(file_path);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(byte_array);
            file.flush();
            file.close();
        }
    }
}
