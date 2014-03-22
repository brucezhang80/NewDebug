#ifndef CODEEDITOR_H
#define CODEEDITOR_H
// 必须要定义下面这个宏，以便连接到QsciScintilla的dll，否则出现连接错误
//#define QSCINTILLA_DLL

#include <QObject>
#include <QWidget>
#include <Qsci/qsciscintilla.h>

QT_BEGIN_NAMESPACE
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LexerCPP;
class QsciLexerMAXScript;

class CodeEditor : public QsciScintilla//QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);
    ~CodeEditor(){}

    void    newFile();
    bool    loadFile(const QString &fileName);
    bool    save();
    bool    saveAs();
    bool    saveFile(const QString &fileName);
    QString currentFile() { return m_curFile; }
    QString userFriendlyCurrentFile();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void lastClose();

private slots:
    void documentWasModified();

private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    void initLexer();

private:
    bool                    m_isUntitled;
    QString                 m_curFile;
    LexerCPP*               m_lexerCpp;
    QsciLexerMAXScript*     m_lexerMaxscript;

};

#endif
