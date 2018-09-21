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


#ifndef _Button_h_
#define _Button_h_

#include <QPushButton>
#include <QTimer>

class QString;

class Button : public QPushButton
{
    Q_OBJECT
public:
  Button(const QString &, QWidget *p_wid = 0);
  virtual void setSize(int);
  void setAutoRepeat(bool);

  virtual void setColor(QColor);
  virtual void setHoverColor(QColor);
  virtual void setPressedColor(QColor);
  virtual void setBackgroundColor(QColor);
  virtual void setHoverBackgroundColor(QColor);
  virtual void setPressedBackgroundColor(QColor);

private:
  void setStyles();

  QString m_colorStyle;
  QString m_borderStyle;
  bool m_autoRepeated;
  int m_autoRepeatInterval;
  int m_autoRepeatDelay;
  QTimer *pm_delayTimer;
  QTimer *pm_intervalTimer;

  QColor m_backgroundColor;
  QColor m_textColor;
  QColor m_hoverBackgroundColor;
  QColor m_hoverTextColor;
  QColor m_pressedBackgroundColor;
  QColor m_pressedTextColor;
  QString stylesheet;

protected slots:
  void intervalTimeout();
  void delayTimeout();
  virtual void mousePressEvent(QMouseEvent* e);
  virtual void mouseReleaseEvent(QMouseEvent* e);
};

#endif
