#include "digit_tool.h"

#include <QRegularExpressionValidator>

DigitTool::DigitTool(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    ui.lineEdit_10->setFocus();

    setAttribute(Qt::WA_DeleteOnClose);

    QValidator *validator = new QRegularExpressionValidator(QRegularExpression("^1[01]*$"), this);
    ui.lineEdit_2->setValidator(validator);
    validator = new QRegularExpressionValidator(QRegularExpression("^[1-7][0-7]*$"), this);
    ui.lineEdit_8->setValidator(validator);
    validator = new QRegularExpressionValidator(QRegularExpression("^[1-9][0-9]*$"), this);
    ui.lineEdit_10->setValidator(validator);
    validator = new QRegularExpressionValidator(QRegularExpression("^[1-9a-fA-F][0-9a-fA-F]*$"), this);
    ui.lineEdit_16->setValidator(validator);

    connect(ui.lineEdit_2, &QLineEdit::textChanged, this, &DigitTool::on_lineEdit_2_text_changed);
    connect(ui.lineEdit_8, &QLineEdit::textChanged, this, &DigitTool::on_lineEdit_8_text_changed);
    connect(ui.lineEdit_10, &QLineEdit::textChanged, this, &DigitTool::on_lineEdit_10_text_changed);
    connect(ui.lineEdit_16, &QLineEdit::textChanged, this, &DigitTool::on_lineEdit_16_text_changed);
}

void DigitTool::update_digit(qulonglong number)
{
    ui.lineEdit_2->setText(QString::number(number, 2));
    ui.lineEdit_8->setText(QString::number(number, 8));
    ui.lineEdit_10->setText(QString::number(number, 10));
    ui.lineEdit_16->setText(QString::number(number, 16).toUpper());
}

void DigitTool::clear_digit()
{
    ui.lineEdit_2->clear();
    ui.lineEdit_8->clear();
    ui.lineEdit_10->clear();
    ui.lineEdit_16->clear();
}

void DigitTool::on_lineEdit_2_text_changed()
{
    QString text = ui.lineEdit_2->text();

    bool ok;
    qulonglong number = text.toULongLong(&ok, 2);
    if (ok)
        update_digit(number);
    else
        clear_digit();
}

void DigitTool::on_lineEdit_8_text_changed()
{
    QString text = ui.lineEdit_8->text();

    bool ok;
    qulonglong number = text.toULongLong(&ok, 8);
    if (ok)
        update_digit(number);
    else
        clear_digit();
}

void DigitTool::on_lineEdit_10_text_changed()
{
    QString text = ui.lineEdit_10->text();

    bool ok;
    qulonglong number = text.toULongLong(&ok, 10);
    if (ok)
        update_digit(number);
    else
        clear_digit();
}

void DigitTool::on_lineEdit_16_text_changed()
{
    QString text = ui.lineEdit_16->text();

    bool ok;
    qulonglong number = text.toULongLong(&ok, 16);
    if (ok)
        update_digit(number);
    else
        clear_digit();
}
