
#include "AeroButton.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QColor>

#include <QDebug>

AeroButton::AeroButton(QWidget *parent) :
    QPushButton(parent),
    m_hovered(false),
    m_pressed(false),
    m_color(Qt::gray),
    m_highlight(Qt::lightGray),
    m_shadow(Qt::gray),
    m_opacity(1.0),
    m_roundness(0)
{

}

AeroButton::AeroButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent),
    m_hovered(false),
    m_pressed(false),
    m_color(Qt::gray),
    m_highlight(Qt::lightGray),
    m_shadow(Qt::gray),
    m_opacity(1.0),
    m_roundness(0)
{

}

AeroButton::AeroButton(const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton(icon, text, parent),
    m_hovered(false),
    m_pressed(false),
    m_color(Qt::gray),
    m_highlight(Qt::lightGray),
    m_shadow(Qt::gray),
    m_opacity(1.0),
    m_roundness(0)
{

}


void AeroButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor button_color;
    if(this->isEnabled())
    {
        m_hovered ? button_color = m_highlight : button_color = m_color;
        if ( m_pressed )
        {
            button_color = m_highlight.darker(250);
        }
    }
    else
    {
        button_color = QColor(50, 50, 50);
    }

    QRect button_rect = this->geometry();

    //outline
    painter.setPen(QPen(QBrush(Qt::gray), 2.0));
    QPainterPath outline;
    outline.addRoundedRect(0, 0, button_rect.width(), button_rect.height(), m_roundness, m_roundness);
    painter.setOpacity(m_opacity);
    painter.drawPath(outline);

    //gradient
    QLinearGradient gradient(0, 0, 0, button_rect.height());
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0.0, button_color);
    gradient.setColorAt(0.4, m_shadow);
    gradient.setColorAt(0.6, m_shadow);
    gradient.setColorAt(1.0, button_color);

    QBrush brush(gradient);
    painter.setBrush(brush);
    painter.setPen(QPen(QBrush(button_color), 2.0));

    // mian button
    QPainterPath painter_path;
    painter_path.addRoundedRect(1, 1, button_rect.width() - 2,
                                button_rect.height() - 2, m_roundness, m_roundness );
    painter.setClipPath(painter_path);
    painter.setOpacity(m_opacity);
    painter.drawRoundedRect(1, 1, button_rect.width() - 2,
                            button_rect.height() - 2, m_roundness, m_roundness);

    // glass highlight
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(QBrush(Qt::white), 0.1));
    painter.setOpacity(0.30);
    painter.drawRect(1, 1, button_rect.width() - 2, (button_rect.height() / 2) - 2);

    QString text = this->text();
    if( !text.isNull() )
    {
        QFont font = this->font();
        painter.setFont(font);
        painter.setPen((Qt::white));
        painter.setOpacity(1.0);
        painter.drawText(0, 0, button_rect.width(), button_rect.height(),
                         Qt::AlignCenter, text);
    }

    // icon
    QIcon icon = this->icon();
    if( !icon.isNull() )
    {
        QSize icon_size = this->iconSize();
        QRect icon_position = this->calculateIconPos(button_rect, icon_size);
        painter.setOpacity(1.0);
        painter.drawPixmap(icon_position, QPixmap(icon.pixmap(icon_size)));
    }
}

void AeroButton::enterEvent(QEvent *event)
{
    m_hovered = true;
    this->repaint();

    QPushButton::enterEvent(event);
}

void AeroButton::leaveEvent(QEvent *event)
{
    m_hovered = false;
    this->repaint();

    QPushButton::leaveEvent(event);
}

void AeroButton::mousePressEvent(QMouseEvent *event)
{
    m_pressed = true;
    this->repaint();

    QPushButton::mousePressEvent(event);
}

void AeroButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    this->repaint();

    QPushButton::mouseReleaseEvent(event);
}

QRect AeroButton::calculateIconPos(const QRect &button_rect, const QSize &icon_size)
{
    int x = (button_rect.width() - icon_size.width()) / 2;
    int y = (button_rect.height() - icon_size.height()) / 2;
    int width = icon_size.width();
    int height = icon_size.height();

    QRect icon_position;
    icon_position.setX(x);
    icon_position.setY(y);
    icon_position.setWidth(width);
    icon_position.setHeight(height);

    return icon_position;
}
