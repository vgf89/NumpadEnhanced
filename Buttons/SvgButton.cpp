#include "SvgButton.h"
#include <QtSvg/QSvgRenderer>
#include <QFile>
#include <QPainter>


SvgIconButton::SvgIconButton(const QString &svgPath, QWidget *p_wid)
    : Button("", p_wid)
{
    this->svgPath = svgPath;
    QFile f(svgPath);

    setMouseTracking(true);
}

QIcon SvgIconButton::makeColoredIcon(QColor color)
{
    // Change the color of all paths/fills in an SVG image, good for recoloring flat icons
    // https://stackoverflow.com/questions/15123544/change-the-color-of-an-svg-in-qt/33540671
    // open svg resource load contents to qbytearray
    QFile file(svgPath);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();
    // load svg contents to xml document and edit contents
    QDomDocument doc;
    doc.setContent(baData);
    // recursively change color
    SetAttrRecur(doc.documentElement(), "path", "fill", color.name());
    // create svg renderer with edited contents
    QSvgRenderer svgRenderer(doc.toByteArray());
    // create pixmap target (could be a QImage)
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);
    // create painter to act over pixmap
    QPainter pixPainter(&pix);
    // use renderer to render over painter which paints on pixmap
    svgRenderer.render(&pixPainter);
    QIcon myicon(pix);

    return myicon;
}

void SvgIconButton::setColor(QColor color)
{
    iconNormal = makeColoredIcon(color);

    setIcon(iconNormal);
}

void SvgIconButton::setHoverColor(QColor color) {
    iconHover = makeColoredIcon(color);

    setIcon(iconNormal);
}

void SvgIconButton::setPressedColor(QColor color) {
    iconPressed = makeColoredIcon(color);

    setIcon(iconNormal);
}

void SvgIconButton::mousePressEvent(QMouseEvent* e)
{
    Button::mousePressEvent(e);
    setIcon(iconPressed);
}


void SvgIconButton::mouseReleaseEvent(QMouseEvent* e)
{
    Button::mouseReleaseEvent(e);
    setIcon(iconNormal);
}

void SvgIconButton::enterEvent(QEvent* e)
{
    Button::enterEvent(e);
    setIcon(iconHover);
}

void SvgIconButton::leaveEvent(QEvent* e)
{
    Button::enterEvent(e);
    setIcon(iconNormal);
}


void SvgIconButton::SetAttrRecur(QDomElement elem, QString strtagname, QString strattr, QString strattrval)
{
    // if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname) == 0)
    {
        elem.setAttribute(strattr, strattrval);
    }
    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++)
    {
        if (!elem.childNodes().at(i).isElement())
        {
            continue;
        }
        SetAttrRecur(elem.childNodes().at(i).toElement(), strtagname, strattr, strattrval);
    }
}
