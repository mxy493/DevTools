#include <QPushButton>
#include <QRegularExpression>
#include <QTextEdit>
#include <QLineEdit>

#include "regex_tool.h"

RegexTool::RegexTool(QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.btn_match, &QPushButton::clicked, this, &RegexTool::match);
    connect(ui.te_input, &QTextEdit::textChanged, this, &RegexTool::match);
    connect(ui.line_regex, &QLineEdit::textChanged, this, &RegexTool::match);
}

void RegexTool::match()
{
    ui.te_output->clear();
    QString input = ui.te_input->toPlainText();
    QString regex = ui.line_regex->text();
    QRegularExpression re(regex);
    QRegularExpressionMatchIterator i = re.globalMatch(input);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString word = match.captured(0);
        ui.te_output->append(word);
    }
}
