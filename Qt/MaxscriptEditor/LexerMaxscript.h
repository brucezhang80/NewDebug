#ifndef LEXERMAXSCRIPT_H
#define LEXERMAXSCRIPT_H
#define QSCINTILLA_DLL

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexercpp.h>

class LexerMaxscript : public QsciLexerCPP
{
    Q_OBJECT
public:

    // parent 通常是 QsciScintilla 类
    explicit LexerMaxscript(QObject *parent = 0);
    ~LexerMaxscript();

    //! 返回语法解析器名称.
    const char *language() const;

    //! 返回语法解析器
    const char *lexer() const;

    //! 自动完成单词操作符
    QStringList autoCompletionWordSeparators() const;

    //!构成代码块的开始关键字
    const char *blockStartKeyword(int *style = 0) const;

    //! 代码块开始标识符号
    const char *blockStart(int *style = 0) const;
    //! 代码块结束标识符号
    const char *blockEnd(int *style = 0) const;

    //! 括号风格
    int braceStyle() const;

    //! 变量/函数等用户自定义标识符允许的字体
    const char *wordCharacters() const;

    //! 关键字
    const char *keywords(int set) const;

signals:

public slots:

protected:

private:
    LexerMaxscript(const LexerMaxscript &);
    LexerMaxscript &operator=(const LexerMaxscript &);
};

#endif // LEXERMAXSCRIPT_H
