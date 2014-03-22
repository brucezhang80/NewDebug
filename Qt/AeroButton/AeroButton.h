/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/20 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef AEROBUTTON_H
#define AEROBUTTON_H

#include <QPushButton>

class AeroButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AeroButton(QWidget *parent = 0);
    explicit AeroButton(const QString &text, QWidget *parent = 0);
    explicit AeroButton(const QIcon &icon, const QString &text, QWidget *parent = 0);

    virtual ~AeroButton(){}
    void setColor(const QColor &color) { m_color = color; }
    void setHighlight(const QColor &highlight) { m_highlight = highlight; }
    void setShadow(const QColor &shadow) { m_shadow = shadow; }

    // 透明度范围: 0.0 ~ 1.0
    void setOpacity(qreal opacity) { m_opacity = opacity; }
    // 圆角范围:0 ~ 99
    void setRoundness(int roundness) { m_roundness = roundness; }

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QRect calculateIconPos(const QRect &button_rect, const QSize &icon_size);

private:
    bool    m_hovered;    // 鼠标略过
    bool    m_pressed;  // 鼠标按下

    QColor  m_color;    // 普通颜色
    QColor  m_highlight;// 高度颜色
    QColor  m_shadow;   // 渐变颜色

    qreal   m_opacity;      // 透明度
    int     m_roundness;    // 圆角孤度
};

#endif // AEROBUTTON_H
