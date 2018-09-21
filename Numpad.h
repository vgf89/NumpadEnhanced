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


#ifndef _Numpad_h_
#define _Numpad_h_

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include <Windows.h>
#include <QVector>

class Button;
class BigVerticalButton;
class BigHorizontalButton;
class SvgIconButton;
class QString;
class QPaintEvent;
class QLabel;
class QTimer;

enum ExtraBtnsType { None, AltCodes, Excel, AltCodes_Excel };

class Numpad : public QWidget
{
  Q_OBJECT

public:
  Numpad(ExtraBtnsType, QWidget *pwid = 0);
  ~Numpad();
  void setNoActivateStyle();
  void unsetNoActivateStyle();
  void correctSize();  
  void setButtonsSize(int, bool correctFontSizeMode);
  void setSpacing(int);
  void setButtonsBackgroundColor(QColor);
  void setButtonsTextColor(QColor);
  void setBackgroundColor(QColor);
  void setButtonsHoverBackgroundColor(QColor);
  void setButtonsHoverTextColor(QColor);
  void setButtonsPressedBackgroundColor(QColor);
  void setButtonsPressedTextColor(QColor);
  QColor getButtonsBackgroundColor() { return m_buttonsBackgroundColor; }
  QColor getButtonsTextColor() { return m_buttonsTextColor; }
  QColor getButtonsHoverBackgroundColor() { return m_buttonsHoverBackgroundColor; }
  QColor getButtonsHoverTextColor() { return m_buttonsHoverTextColor; }
  QColor getButtonsPressedBackgroundColor() { return m_buttonsPressedBackgroundColor; }
  QColor getButtonsPressedTextColor() { return m_buttonsPressedTextColor; }
  QColor getBackgroundColor() { return m_backgroundColor; }
  int getButtonsSize() { return m_buttonsSize; }
  int getSpacing() { return pm_gridLayout->spacing(); }
  int getMinButtonsSize() { return m_minButtonsSize; }
  int getMaxButtonsSize() { return m_maxButtonsSize; }
  int getMinSpacing() { return m_minSpacing; }
  int getMaxSpacing() { return m_maxSpacing; } 
  QFont getButtonsFont();
  void setButtonsFont(QFont font);
  bool getAltCodeLblMode() { return m_altCodeLblMode; }
  void setAltCodeLblMode(bool mode) { m_altCodeLblMode = mode; }  
  ExtraBtnsType getExtraBtnsType() { return m_extraBtnsType; }

signals:
  void showed();

protected:
  /*override*/ bool winEvent(MSG  *p_msg, long *p_result);
  /*override*/ void paintEvent(QPaintEvent *);
  /*override*/ void hideEvent(QHideEvent *);
  /*override*/ void showEvent(QShowEvent *);

protected slots:
  void slot_nullBtnPressed();
  void slot_oneBtnPressed();
  void slot_twoBtnPressed();
  void slot_threeBtnPressed();
  void slot_fourBtnPressed();
  void slot_fiveBtnPressed();
  void slot_sixBtnPressed();
  void slot_sevenBtnPressed();
  void slot_eightBtnPressed();
  void slot_nineBtnPressed();
  void slot_divBtnPressed();
  void slot_multiplyBtnPressed();
  void slot_minusBtnPressed();
  void slot_decimalBtnPressed();
  void slot_plusBtnPressed();
  void slot_EBtnPressed();
  void slot_enterBtnPressed();
  void slot_backspaceBtnPressed();
  void slot_nullBtnReleased();
  void slot_oneBtnReleased();
  void slot_twoBtnReleased();
  void slot_threeBtnReleased();
  void slot_fourBtnReleased();
  void slot_fiveBtnReleased();
  void slot_sixBtnReleased();
  void slot_sevenBtnReleased();
  void slot_eightBtnReleased();
  void slot_nineBtnReleased();
  void slot_divBtnReleased();
  void slot_multiplyBtnReleased();
  void slot_minusBtnReleased();
  void slot_decimalBtnReleased();
  void slot_plusBtnReleased();
  void slot_enterBtnReleased();
  void slot_backspaceBtnReleased();
  void slot_altBtnToggled(bool checked);
  void slot_showAltCodeLblTimeout();
  void slot_upBtnPressed();
  void slot_downBtnPressed();
  void slot_leftBtnPressed();
  void slot_rightBtnPressed();
  void slot_upBtnReleased();
  void slot_downBtnReleased();
  void slot_leftBtnReleased();
  void slot_rightBtnReleased();
  void slot_equalBtnPressed();

private:
  void createAndConnectButtons();
  Button *createButton(QString, int row, int column);
  SvgIconButton *createSvgButton(QString, int row, int column);
  BigHorizontalButton *createBigHorizontalButton(QString, int row, int column);
  BigVerticalButton *createBigVerticalButton(QString, int row, int column);
  void correctBigButtonsSize();
  void setButtonsIconSize(int size);
  void simulatePressKey(int);
  void simulateReleaseKey(int);  
  void initAltCodesList();
  void correctAltCodeLblFontSize();
  void createAltCodeLbl();
  void createExtraBtns();

  std::vector<Button *> m_buttons;
  QGridLayout *pm_gridLayout;
  QColor m_buttonsBackgroundColor;
  QColor m_buttonsTextColor;
  QColor m_buttonsHoverBackgroundColor;
  QColor m_buttonsHoverTextColor;
  QColor m_buttonsPressedBackgroundColor;
  QColor m_buttonsPressedTextColor;
  QColor m_backgroundColor;
  int m_buttonsSize;
  QWidget *pm_buttonsBox;
  int m_minButtonsSize;
  int m_maxButtonsSize;
  int m_minSpacing;
  int m_maxSpacing;
  HWND m_lastFocusWindow;
  bool m_noActivateStyleSet; 
  Button *pm_altBtn;
  bool m_altBtnPress;
  bool m_firstShow;
  QLabel *pm_altCodeLbl;
  QTimer *pm_showAltCodeLblTimer;
  QVector<QString> m_altCodes;  
  bool m_altCodeLblMode;
  ExtraBtnsType m_extraBtnsType;

};

#endif
