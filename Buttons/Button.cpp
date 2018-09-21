/* 
 * This source file is part of Numpad.
 * Copyright (C) 2013 Timur Abdrazyakov
 * abtimal@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */


#include "Buttons/Button.h"
#include <QString>
#include <QIcon>

Button::Button(const QString &text, QWidget *p_wid/*= 0*/)
: QPushButton(text, p_wid)
{    
    setCheckable(false);
    m_autoRepeated = false;
    m_autoRepeatDelay = 600;
    m_autoRepeatInterval = 50;
    pm_delayTimer = NULL;
    pm_intervalTimer = NULL;

    stylesheet =
          "QPushButton {"
              "border: 0px none;"
              "margin: 1px;"
              "background-color: %1;"
              "color: %2;"
          "}"
          "QPushButton:hover {"
              "background-color: %3;"
              "color: %4;"
          "}"
          "QPushButton:pressed {"
              "background-color: %5;"
              "color: %6;"
          "}"
          "QPushButton:checked {"
              "background-color: %5;"
              "color: %6;"
          "}";

    setStyles();
}


void Button::setSize(int size)
{
  setFixedSize(size, size);
}


void Button::setStyles()
{
        setStyleSheet(
                    stylesheet.arg(
                        m_backgroundColor.name(),
                        m_textColor.name(),
                        m_hoverBackgroundColor.name(),
                        m_hoverTextColor.name(),
                        m_pressedBackgroundColor.name(),
                        m_pressedTextColor.name()
                        )
                    );
}


void Button::mousePressEvent(QMouseEvent* e)
{
    QPushButton::mousePressEvent(e);
    if (m_autoRepeated)
    {
        pm_delayTimer->start(m_autoRepeatDelay);
    }
}


void Button::mouseReleaseEvent(QMouseEvent* e)
{
    QPushButton::mouseReleaseEvent(e);
    if (m_autoRepeated)
    {
        pm_delayTimer->stop();
        pm_intervalTimer->stop();
    }
}


void Button::setAutoRepeat(bool mode)
{
    m_autoRepeated = mode;
    if (m_autoRepeated)
    {
        if (!pm_delayTimer)
        {
            pm_delayTimer = new QTimer();
            connect(pm_delayTimer, SIGNAL(timeout()),
                    this, SLOT(delayTimeout()));
            pm_intervalTimer = new QTimer();
            connect(pm_intervalTimer, SIGNAL(timeout()),
                    this, SLOT(intervalTimeout()));
        }
    }
    else if (pm_delayTimer)
    {
        delete pm_delayTimer;
        pm_delayTimer = NULL;
        delete pm_intervalTimer;
        pm_intervalTimer = NULL;
    }
}


void Button::delayTimeout()
{
    pm_delayTimer->stop();
    if (isDown())
    {
        pm_intervalTimer->start(m_autoRepeatInterval);
    }
}


void Button::intervalTimeout()
{
    if (isDown())
    {
        emit pressed();
    }
    else
    {
        pm_intervalTimer->stop();
    }
}

void Button::setColor(QColor color)
{
    this->m_textColor = color;
    setStyles();
}

void Button::setBackgroundColor(QColor color)
{
    m_backgroundColor = color;
    setStyles();
}

void Button::setHoverColor(QColor color)
{
    m_hoverTextColor = color;
    setStyles();
}

void Button::setHoverBackgroundColor(QColor color)
{
    m_hoverBackgroundColor = color;
    setStyles();
}

void Button::setPressedColor(QColor color)
{
    m_pressedTextColor = color;
    setStyles();
}

void Button::setPressedBackgroundColor(QColor color)
{
    m_pressedBackgroundColor = color;
    setStyles();
}
