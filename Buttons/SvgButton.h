#ifndef SVGBUTTON_H
#define SVGBUTTON_H

#include "Buttons/Button.h"
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>


class SvgIconButton : public Button
{
public:
    SvgIconButton(const QString &, QWidget *p_wid = 0);
    void setColor(QColor);
    void setHoverColor(QColor);
    void setPressedColor(QColor);
private:
    QString svgPath;
    void SetAttrRecur(QDomElement elem, QString strtagname, QString strattr, QString strattrval);

    QIcon makeColoredIcon(QColor);
    QIcon iconNormal;
    QIcon iconHover;
    QIcon iconPressed;

protected slots:
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void enterEvent(QEvent* e);
    virtual void leaveEvent(QEvent* e);
};

#endif // SVGBUTTON_H
