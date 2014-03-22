#include "CodeEditor.h"
#include "LexerCPP.h"
#include <Qsci/qscilexermaxscript.h>

#include <QtGui>
#include <QDebug>

CodeEditor::CodeEditor(QWidget *parent) :
    QsciScintilla(parent),
    m_isUntitled(true)
{
    initLexer();

    this->setLexer(m_lexerMaxscript);
    this->setUtf8(true);
    this->setMarginLineNumbers(1, true);
    this->setMarginWidth(1, 35);
    this->setTabIndents(true);
    this->setIndentationGuides(true);
    this->setIndentationsUseTabs(true);
    this->setAutoIndent(true);
    this->setTabWidth(4);
    this->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    this->setBraceMatching(QsciScintilla::StrictBraceMatch);
    this->setWrapMode(QsciScintilla::WrapWord);
    this->setAutoCompletionSource(QsciScintilla::AcsAll);
    this->setAutoCompletionCaseSensitivity(false);
    this->setAutoCompletionFillupsEnabled(true);
    this->setAutoCompletionThreshold(1);
    this->setAutoCompletionShowSingle(true);
    this->setAutoCompletionReplaceWord(true);
}

void CodeEditor::initLexer()
{
    m_lexerMaxscript = new QsciLexerMAXScript;
    m_lexerMaxscript->setFoldCompact(true);
    m_lexerMaxscript->setColor(QColor(128, 128, 255), 5);
    m_lexerMaxscript->setFont(QFont("Courier", 11));
}

void CodeEditor::newFile()
{
    static int sequenceNumber = 1;

    m_isUntitled = true;
    m_curFile = tr("document%1.txt").arg(sequenceNumber++);
    setWindowTitle(m_curFile + "[*]");

    connect(this, SIGNAL(textChanged()), this, SLOT(documentWasModified()));
}

bool CodeEditor::loadFile(const QString &fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text))
     {
         QMessageBox::warning(this, tr("MAXScript Editor"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream in(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     this->setText(in.readAll());
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);

     connect(this, SIGNAL(textChanged()), this, SLOT(documentWasModified()));

     return true;
 }

 bool CodeEditor::save()
 {
     if ( m_isUntitled )
     {
         return saveAs();
     }
     else
     {
         return saveFile( m_curFile );
     }
 }

 bool CodeEditor::saveAs()
 {
     QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), m_curFile);
     if (fileName.isEmpty())
         return false;

     return saveFile(fileName);
 }

 bool CodeEditor::saveFile(const QString &fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text))
     {
         QMessageBox::warning(this, tr("MAXScript Editor"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream out(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     out << text();
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);
     return true;
 }

 QString CodeEditor::userFriendlyCurrentFile()
 {
     return strippedName(m_curFile);
 }

void CodeEditor::documentWasModified()
{
    setWindowModified(isModified());
}

bool CodeEditor::maybeSave()
{
    if (isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("MAXScript Editor"),
                            tr("'%1' has been modified.\n"
                            "Do you want to save your changes?")
                            .arg(userFriendlyCurrentFile()),
                            QMessageBox::Save | QMessageBox::Discard
                            | QMessageBox::Cancel);

        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void CodeEditor::setCurrentFile(const QString &fileName)
{
    m_curFile = QFileInfo(fileName).canonicalFilePath();
    m_isUntitled = false;
    this->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString CodeEditor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void CodeEditor::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }

    //    emit lastClose();
}

