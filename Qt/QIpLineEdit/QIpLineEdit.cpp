/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/3 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "QIpLineEdit.h"

#include <QLineEdit>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QFont>
#include <QLabel>
#include <QKeyEvent>

QIpLineEdit::QIpLineEdit(QWidget *parent) :
    QFrame(parent)
{
    this->setFrameShape( QFrame::StyledPanel );
    this->setFrameShadow( QFrame::Sunken );
    QHBoxLayout* layout = new QHBoxLayout( this );
    setLayout( layout );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );

    for ( int i = 0; i != IP_SIZE; ++i )
    {
        if ( i != 0 )
        {
            QLabel* dot = new QLabel( ".", this );
            dot->setStyleSheet( "background: white" );
            layout->addWidget( dot );
            layout->setStretch( layout->count(), 0 );
        }
        m_lineEdit[i] = new QLineEdit( this );
        QLineEdit* pEdit = m_lineEdit[i];
        pEdit->installEventFilter( this );

        layout->addWidget( pEdit );
        layout->setStretch( layout->count(), 1 );

        pEdit->setFrame( false );
        pEdit->setAlignment( Qt::AlignCenter );

        QFont font = pEdit->font();
        font.setStyleHint( QFont::Monospace );
        font.setFixedPitch( true );
        pEdit->setFont( font );

        QRegExp rx ( "^(0|[1-9]|[1-9][0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))$" );
        QValidator *validator = new QRegExpValidator(rx, pEdit);
        pEdit->setValidator( validator );
    }
    setMaximumWidth( 30 * IP_SIZE );

    connect( this, SIGNAL(textChanged(QLineEdit*)),
                this, SLOT(setText(QLineEdit*)),
                    Qt::QueuedConnection );
}

void QIpLineEdit::setText(QLineEdit *edit)
{
    for ( unsigned int i = 0; i != IP_SIZE; ++i )
    {
        if ( edit == m_lineEdit[i] )
        {
            if ( ( edit->text().size() == MAX_DIGITS
                   &&edit->text().size() == edit->cursorPosition() )
                 || ( edit->text() == "0") )
            {
                // auto-move to next item
                if ( i+1 != IP_SIZE )
                {
                    m_lineEdit[i+1]->setFocus();
                    m_lineEdit[i+1]->selectAll();
                }
            }
        }
    }
}

bool QIpLineEdit::eventFilter(QObject *obj, QEvent *event)
{
    bool bRes = QFrame::eventFilter(obj, event);
    if ( event->type() == QEvent::KeyPress )
    {
        QKeyEvent* pEvent = dynamic_cast<QKeyEvent*>( event );
        if ( pEvent )
        {
            for ( unsigned int i = 0; i != IP_SIZE; ++i )
            {
                QLineEdit* pEdit = m_lineEdit[i];
                if ( pEdit == obj )
                {
                    switch ( pEvent->key() )
                    {
                    case Qt::Key_Left:
                        if ( pEdit->cursorPosition() == 0 )
                        {
                            // user wants to move to previous item
                            movePrevLineEdit(i);
                        }
                        break;

                    case Qt::Key_Right:
                        if ( pEdit->text().isEmpty()
                             || (pEdit->text().size()
                                 == pEdit->cursorPosition()) )
                        {
                            // user wants to move to next item
                            moveNextLineEdit(i);
                        }
                        break;

                    case Qt::Key_0:
                        if ( pEdit->text().isEmpty()
                             || pEdit->text() == "0" )
                        {
                            pEdit->setText("0");
                            // user wants to move to next item
                            moveNextLineEdit(i);
                        }
                        emit textChanged( pEdit );
                        break;

                    case Qt::Key_Backspace:
                        if ( pEdit->text().isEmpty()
                             || pEdit->cursorPosition() == 0)
                        {
                            // user wants to move to previous item
                            movePrevLineEdit(i);
                        }
                        break;

                    case Qt::Key_Comma:
                    case Qt::Key_Period:
                    case Qt::Key_Space:
                        moveNextLineEdit(i);
                        break;

                    default:
                        emit textChanged( pEdit );
                        break;

                    }
                }
            }
        }
    }

    return bRes;
}

void QIpLineEdit::moveNextLineEdit(int i)
{
    if ( i+1 != IP_SIZE )
    {
        m_lineEdit[i+1]->setFocus();
        m_lineEdit[i+1]->setCursorPosition( 0 );
        m_lineEdit[i+1]->selectAll();
    }
}

void QIpLineEdit::movePrevLineEdit(int i)
{
    if ( i != 0 )
    {
        m_lineEdit[i-1]->setFocus();
        m_lineEdit[i-1]->setCursorPosition( m_lineEdit[i-1]->text().size() );
        //m_pLineEdit[i-1]->selectAll();
    }
}
