#ifndef LEXERCPP_H
#define LEXERCPP_H
//#define QSCINTILLA_DLL

#include <Qsci/qscilexercpp.h>

class LexerCPP : public QsciLexerCPP
{
    Q_OBJECT
public:
    explicit LexerCPP(QWidget *parent = 0);

signals:

public slots:

private:
    void initLexer();
};

#endif // LEXERCPP_H
