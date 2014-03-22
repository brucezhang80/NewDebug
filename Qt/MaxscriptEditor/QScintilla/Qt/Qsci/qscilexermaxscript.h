#ifndef QSCILEXERMAXSCRIPT_H
#define QSCILEXERMAXSCRIPT_H

#ifdef __APPLE__
extern "C++" {
#endif

#include <qobject.h>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>

class /*QSCINTILLA_EXPORT*/ QsciLexerMAXScript : public QsciLexer
{
    Q_OBJECT
public:
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A comment line.
        CommentLine = 2,

        //! A number.
        Number = 3,

		//! A word
		Word = 4,
		
        //! A string.
        String = 5,

        //! An operator.
        Operator = 6,

        //! An identifier
        Identifier = 7,

        //! The end of a line where a string is not closed.
        UnclosedString = 8,

        //! A keyword.
        Keyword = 9,

        //! A standard operator.
        StandardOperator = 10,

        //! An attribute.
        Attribute = 11,

        //! A standard function.
        StandardFunction = 12,

        //! A standard package.
        StandardPackage = 13,

        //! A standard type.
        StandardType = 14,

        //! A keyword defined in keyword set number 7.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        KeywordSet7 = 15
    };

    QsciLexerMAXScript(QObject *parent = 0);
    virtual ~QsciLexerMAXScript();

    const char *language() const;

    const char *lexer() const;

    QStringList autoCompletionWordSeparators() const;

    const char *blockStartKeyword(int *style = 0) const;

    const char *blockStart(int *style = 0) const;

    const char *blockEnd(int *style = 0) const;

	//! \internal Returns the style used for braces for brace matching.
    int braceStyle() const;

    QColor defaultColor(int style) const;
	
    bool defaultEolFill(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the propertyChanged()
    //! signal as required.
    void refreshProperties();

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const;

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const;

    //! Returns true if else blocks can be folded.
    //!
    //! \sa setFoldAtElse()
    bool foldAtElse() const;

    //! Returns true if begin blocks can be folded.
    //!
    //! \sa setFoldAtBegin()
    bool foldAtBegin() const;

    //! Returns true if blocks can be folded at a parenthesis.
    //!
    //! \sa setFoldAtParenthesis()
    bool foldAtParenthesis() const;

signals:

public slots:
    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is true.
    //!
    //! \sa foldComments()
    virtual void setFoldComments(bool fold);

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    virtual void setFoldCompact(bool fold);

    //! If \a fold is true then else blocks can be folded.  The default is
    //! true.
    //!
    //! \sa foldAtElse()
    virtual void setFoldAtElse(bool fold);

    //! If \a fold is true then begin blocks can be folded.  The default is
    //! true.
    //!
    //! \sa foldAtBegin()
    virtual void setFoldAtBegin(bool fold);

    //! If \a fold is true then blocks can be folded at a parenthesis.  The
    //! default is true.
    //!
    //! \sa foldAtParenthesis()
    virtual void setFoldAtParenthesis(bool fold);

protected:
    //!
    bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    bool writeProperties(QSettings &qs,const QString &prefix) const;
private:
    void setCommentProp();
    void setCompactProp();
    void setAtElseProp();
    void setAtBeginProp();
    void setAtParenthProp();

    bool fold_comments;
    bool fold_compact;
    bool fold_atelse;
    bool fold_atbegin;
    bool fold_atparenth;
	
    QsciLexerMAXScript(const QsciLexerMAXScript &);
    QsciLexerMAXScript &operator=(const QsciLexerMAXScript &);
};

#ifdef __APPLE__
}
#endif

#endif // LEXERMAXSCRIPT_H
