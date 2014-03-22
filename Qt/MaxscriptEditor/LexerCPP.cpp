#include "LexerCPP.h"
#include <Qsci/qsciapis.h>

LexerCPP::LexerCPP(QWidget *parent) :
    QsciLexerCPP(parent)
{
}

void LexerCPP::initLexer()
{
    this->setFoldCompact(false);
    this->setColor(QColor(128, 128, 255), 5);
    this->setColor(QColor(0, 220, 0), 1);
    this->setColor(QColor(0, 220, 0), 2);
    this->setColor(QColor(Qt::red), 6);
    this->setColor(QColor(Qt::red), 8);
    this->setColor(QColor(255, 128, 0), 13);
    this->setColor(QColor(51, 102, 255), 15);
    this->setColor(QColor(72, 61, 139), 10);
    this->setFont(QFont("Courier New", 11, QFont::Bold));
}
