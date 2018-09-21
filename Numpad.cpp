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
 

#include "Numpad.h"
#include "Buttons/Button.h"
#include "Buttons/BigVerticalButton.h"
#include "Buttons/BigHorizontalButton.h"
#include "Buttons/SvgButton.h"
#include <typeinfo>
#include <vector>
#include <QString>
#include <QColor>
#include <QPaintEvent>
#include <QPalette>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QLabel>
#include <QTimer>
#include <QIcon>
#include <ctime>
#include <cstdlib>

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")

Numpad::Numpad(ExtraBtnsType extraBtnsType, QWidget *p_wid/*= 0*/)
: QWidget(p_wid, Qt::WindowTitleHint | Qt::WindowStaysOnTopHint)
{
  srand(time(0));

  setWindowTitle("NumpadEnhanced");

  pm_gridLayout = new QGridLayout; 

  pm_altBtn = NULL;
  m_extraBtnsType = extraBtnsType;
  createAndConnectButtons();

  pm_buttonsBox = new QWidget;
  pm_buttonsBox->setLayout(pm_gridLayout);

  QHBoxLayout *p_middleLayout = new QHBoxLayout;
  p_middleLayout->addWidget(pm_buttonsBox);
  p_middleLayout->addStretch(1);
  p_middleLayout->setMargin(0);
   
  QVBoxLayout *p_mainLayout = new QVBoxLayout;
  p_mainLayout->addLayout(p_middleLayout);
  p_mainLayout->addStretch(1);
  p_mainLayout->setMargin(0);
   
  setLayout(p_mainLayout);
 
  m_minSpacing = 0;
  m_maxSpacing = 30;

  m_minButtonsSize = 40;
  
  m_maxButtonsSize = QApplication::desktop()->height() / 7;

  m_altBtnPress = false;

  m_firstShow = true;

  pm_altCodeLbl = NULL;

  m_altCodeLblMode = true;  

  setNoActivateStyle();
}


////////////////////////////////////////////////////////////////////////////////

Numpad::~Numpad()
{
    if (m_altBtnPress)
    {
        simulateReleaseKey(VK_MENU);
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::createAndConnectButtons()
{
    Button *p_button;   

    p_button = createButton("1", 3, 0);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_oneBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_oneBtnReleased()));

    p_button = createButton("2", 3, 1);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_twoBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_twoBtnReleased()));

    p_button = createButton("3", 3, 2);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_threeBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_threeBtnReleased()));

    p_button = createButton("4", 2, 0);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_fourBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_fourBtnReleased()));

    p_button = createButton("5", 2, 1);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_fiveBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_fiveBtnReleased()));

    p_button = createButton("6", 2, 2);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_sixBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_sixBtnReleased()));

    p_button = createButton("7", 1, 0);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_sevenBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_sevenBtnReleased()));

    p_button = createButton("8", 1, 1);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_eightBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_eightBtnReleased()));

    p_button = createButton("9", 1, 2);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_nineBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_nineBtnReleased()));

    p_button = createButton("/", 0, 1);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_divBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_divBtnReleased()));

    p_button = createButton("*", 0, 2);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_multiplyBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_multiplyBtnReleased()));

    p_button = createButton("-", 0, 3);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_minusBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_minusBtnReleased()));

    p_button = createButton("+", 1, 3);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_plusBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_plusBtnReleased()));

    p_button = createButton("E", 2, 3);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_EBtnPressed()));

    p_button = createBigVerticalButton("Enter", 3, 3);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_enterBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_enterBtnReleased()));

    p_button = createButton(".", 4, 2);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_decimalBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_decimalBtnReleased()));

    p_button = createSvgButton(":Icons/backspace.svg", 0, 0);
    connect(p_button, SIGNAL(pressed()), SLOT(slot_backspaceBtnPressed()));
    connect(p_button, SIGNAL(released()), SLOT(slot_backspaceBtnReleased()));

    createExtraBtns();

    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
        (*iter)->setAutoRepeat(true);
        iter++;
    }
    if (pm_altBtn)
    {
        pm_altBtn->setAutoRepeat(false);
    }
}

void Numpad::createExtraBtns()
{
    Button *p_btn;
    if (m_extraBtnsType == None || m_extraBtnsType == AltCodes)
    {
        p_btn = createBigHorizontalButton("0", 4, 0);
        connect(p_btn, SIGNAL(pressed()), SLOT(slot_nullBtnPressed()));
        connect(p_btn, SIGNAL(released()), SLOT(slot_nullBtnReleased()));
    }
    if (m_extraBtnsType == Excel || m_extraBtnsType == AltCodes_Excel)
    {
        p_btn = createButton("0", 4, 0);
        connect(p_btn, SIGNAL(pressed()), SLOT(slot_nullBtnPressed()));
        connect(p_btn, SIGNAL(released()), SLOT(slot_nullBtnReleased()));
        p_btn = createButton(QString::fromUtf8("\u2191"), 4, 1);
        connect(p_btn, SIGNAL(pressed()), SLOT(slot_upBtnPressed()));
        connect(p_btn, SIGNAL(released()), SLOT(slot_upBtnReleased()));
        p_btn = createButton(QString::fromUtf8("\u2190"), 5, 0);
        connect(p_btn, SIGNAL(pressed()), SLOT(slot_leftBtnPressed()));
        connect(p_btn, SIGNAL(released()), SLOT(slot_leftBtnReleased()));
        p_btn = createButton(QString::fromUtf8("\u2193"), 5, 1);
        connect(p_btn, SIGNAL(pressed()), SLOT(slot_downBtnPressed()));
        connect(p_btn, SIGNAL(released()), SLOT(slot_downBtnReleased()));
        p_btn = createButton(QString::fromUtf8("\u2192"), 5, 2);
        connect(p_btn, SIGNAL(pressed()), SLOT(slot_rightBtnPressed()));
        connect(p_btn, SIGNAL(released()), SLOT(slot_rightBtnReleased()));
        p_btn = createButton("=", 5, 3);
        connect(p_btn, SIGNAL(pressed()), SLOT(slot_equalBtnPressed()));
    }
    if (m_extraBtnsType == AltCodes || m_extraBtnsType == AltCodes_Excel)
    {
        int row = m_extraBtnsType == AltCodes ? 5 : 6;
        pm_altBtn = createBigHorizontalButton("alt", row, 0);
        pm_altBtn->setCheckable(true);
        pm_altBtn->setChecked(false);
        connect(pm_altBtn, SIGNAL(toggled(bool)),
                SLOT(slot_altBtnToggled(bool)));
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::createAltCodeLbl()
{
    initAltCodesList();
    int altCodeMsgIndex = rand() % m_altCodes.size();
    pm_altCodeLbl = new QLabel(m_altCodes[altCodeMsgIndex]);
    pm_altCodeLbl->setStyleSheet(QString("QLabel{color: %1;}").arg(m_buttonsTextColor.name()));
    int row;
    if (m_extraBtnsType == AltCodes)
    {
        row = 5;
    }
    else if (m_extraBtnsType == AltCodes_Excel)
    {
        row = 6;
    }
    else return;
    pm_gridLayout->addWidget(pm_altCodeLbl, row, 2, 1, 2);
}

////////////////////////////////////////////////////////////////////////////////

Button *Numpad::createButton(QString text, int row, int column)
{
  Button *p_btn = new Button(text);
  m_buttons.push_back(p_btn);
  pm_gridLayout->addWidget(p_btn, row, column);
  return p_btn;
}

////////////////////////////////////////////////////////////////////////////////

SvgIconButton *Numpad::createSvgButton(QString iconPath, int row, int column)
{
  SvgIconButton *p_btn = new SvgIconButton(iconPath);
  m_buttons.push_back(p_btn);
  pm_gridLayout->addWidget(p_btn, row, column);
  return p_btn;
}
  
////////////////////////////////////////////////////////////////////////////////

BigHorizontalButton *Numpad::createBigHorizontalButton(QString text, int row, int column)
{
  BigHorizontalButton *p_btn = new BigHorizontalButton(pm_gridLayout, text);
  m_buttons.push_back(p_btn);
  pm_gridLayout->addWidget(p_btn, row, column, 1, 2);
  return p_btn;
}  

////////////////////////////////////////////////////////////////////////////////

BigVerticalButton *Numpad::createBigVerticalButton(QString text, int row, int column)
{
  BigVerticalButton *p_btn = new BigVerticalButton(pm_gridLayout, text);
  m_buttons.push_back(p_btn);
  pm_gridLayout->addWidget(p_btn, row, column, 2, 1);
  return p_btn;
}  

////////////////////////////////////////////////////////////////////////////////

void Numpad::setButtonsSize(int size, bool correctFontSizeMode)
{
  m_buttonsSize = size;
  std::vector<Button *>::iterator iter = m_buttons.begin();
  while (iter != m_buttons.end())
  {    
    (*iter)->setSize(m_buttonsSize);
    iter++;
  }  

  correctAltCodeLblFontSize();

  pm_gridLayout->setMargin(m_buttonsSize / 20);

  if (correctFontSizeMode)
  {
      QFont buttonsFont = getButtonsFont();
      int ratioButtonsAndFontSizes = 3;
      buttonsFont.setPixelSize(m_buttonsSize / ratioButtonsAndFontSizes);
      setButtonsFont(buttonsFont);
  }
  setButtonsIconSize(size);
}    

void Numpad::setButtonsIconSize(int size)
{
    size /= 2;
    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
        (*iter)->setIconSize(QSize(size, size));
        iter++;
    }
}
////////////////////////////////////////////////////////////////////////////////

void Numpad::setSpacing(int spacing)
{
   pm_gridLayout->setSpacing(spacing);
   correctBigButtonsSize();
}  

////////////////////////////////////////////////////////////////////////////////

void Numpad::correctSize()
{
    int rowCount;
    switch (m_extraBtnsType) {
    case None:
        rowCount = 5;
        break;
    case AltCodes:
    case Excel:
        rowCount = 6;
        break;
    case AltCodes_Excel:
        rowCount = 7;
        break;
    }
   resize(getButtonsSize() * 4 + getSpacing() * 3,
         getButtonsSize() * rowCount + getSpacing() * (rowCount - 1));
}

///////////////////////////////////////////////////////////////////////////////

void Numpad::correctBigButtonsSize()
{
  std::vector<Button *>::iterator iter = m_buttons.begin();
  AbstractBigButton *p_bigButton;
  while (iter != m_buttons.end())
  {
    p_bigButton = dynamic_cast<AbstractBigButton *> (*iter);
    if (p_bigButton)
    {
      p_bigButton->correctSize();
    }    
    iter++;
  }  
}

////////////////////////////////////////////////////////////////////////////////  
      
void Numpad::setButtonsBackgroundColor(QColor color)
{
    m_buttonsBackgroundColor = color;
    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
      (*iter)->setBackgroundColor(color);
      iter++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::setButtonsTextColor(QColor color)
{
    m_buttonsTextColor = color;
    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
        (*iter)->setColor(color);
        iter++;
    }

    if (pm_altCodeLbl) {
        pm_altCodeLbl->setStyleSheet(QString("color: %1").arg(color.name()));
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::setButtonsHoverBackgroundColor(QColor color)
{
    m_buttonsHoverBackgroundColor = color;
    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
        (*iter)->setHoverBackgroundColor(color);
        iter++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::setButtonsHoverTextColor(QColor color)
{
    m_buttonsHoverTextColor = color;
    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
        (*iter)->setHoverColor(color);
        iter++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::setButtonsPressedBackgroundColor(QColor color)
{
    m_buttonsPressedBackgroundColor = color;
    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
        (*iter)->setPressedBackgroundColor(color);
        iter++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::setButtonsPressedTextColor(QColor color)
{
    m_buttonsPressedTextColor = color;
    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
        (*iter)->setPressedColor(color);
        iter++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::setBackgroundColor(QColor color)
{
  m_backgroundColor = color;
  QPalette pal = palette();
  pal.setColor(backgroundRole(), color);
  setPalette(pal);
}  
    
////////////////////////////////////////////////////////////////////////////////

QFont Numpad::getButtonsFont()
{
    return m_buttons[0]->font();
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::setButtonsFont(QFont font)
{
    std::vector<Button *>::iterator iter = m_buttons.begin();
    while (iter != m_buttons.end())
    {
        (*iter)->setFont(font);
        iter++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::correctAltCodeLblFontSize()
{
    if (pm_altCodeLbl)
    {
        QFont font;
        font.setPixelSize(m_buttonsSize / 3);
        pm_altCodeLbl->setFont(font);
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::simulatePressKey(int keyCode)
{
    keybd_event(keyCode, MapVirtualKey(keyCode, 0), 0, 0);
}

////////////////////////////////////////////////////////////////////////////////  

void Numpad::simulateReleaseKey(int keyCode)
{    
    keybd_event(keyCode, MapVirtualKey(keyCode, 0), KEYEVENTF_KEYUP, 0);   
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_nullBtnPressed()
{
  simulatePressKey(VK_NUMPAD0);
}

//////////////////////////////////////////////////////////////////////////////// 

void Numpad::slot_oneBtnPressed()
{   
  simulatePressKey(VK_NUMPAD1);
}

//////////////////////////////////////////////////////////////////////////////// 

void Numpad::slot_twoBtnPressed()
{    
  simulatePressKey(VK_NUMPAD2);
}

////////////////////////////////////////////////////////////////////////////////   

void Numpad::slot_threeBtnPressed()
{   
  simulatePressKey(VK_NUMPAD3);
}

////////////////////////////////////////////////////////////////////////////////  

void Numpad::slot_fourBtnPressed()
{
  simulatePressKey(VK_NUMPAD4);
}

////////////////////////////////////////////////////////////////////////////////   

void Numpad::slot_fiveBtnPressed()
{
  simulatePressKey(VK_NUMPAD5);
}

////////////////////////////////////////////////////////////////////////////////   

void Numpad::slot_sixBtnPressed()
{
  simulatePressKey(VK_NUMPAD6);
}

////////////////////////////////////////////////////////////////////////////////    

void Numpad::slot_sevenBtnPressed()
{
  simulatePressKey(VK_NUMPAD7);
}

////////////////////////////////////////////////////////////////////////////////      
  
void Numpad::slot_eightBtnPressed()
{
  simulatePressKey(VK_NUMPAD8);
}

////////////////////////////////////////////////////////////////////////////////      
    
void Numpad::slot_nineBtnPressed()
{
  simulatePressKey(VK_NUMPAD9);
}

////////////////////////////////////////////////////////////////////////////////   

void Numpad::slot_divBtnPressed()
{
  simulatePressKey(VK_DIVIDE);
}    
    
//////////////////////////////////////////////////////////////////////////////// 
    
void Numpad::slot_multiplyBtnPressed()
{
  simulatePressKey(VK_MULTIPLY);
}

////////////////////////////////////////////////////////////////////////////////    
    
void Numpad::slot_minusBtnPressed()
{
  simulatePressKey(VK_SUBTRACT);
}

////////////////////////////////////////////////////////////////////////////////      
  
void Numpad::slot_decimalBtnPressed()
{
  simulatePressKey(VK_DECIMAL);
}

////////////////////////////////////////////////////////////////////////////////   
    
void Numpad::slot_plusBtnPressed()
{
  simulatePressKey(VK_ADD);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_EBtnPressed()
{
    simulatePressKey(VK_SHIFT);
    simulatePressKey(0x45);
    simulateReleaseKey(0x45);
    simulateReleaseKey(VK_SHIFT);
}

////////////////////////////////////////////////////////////////////////////////   
    
void Numpad::slot_enterBtnPressed()
{
  simulatePressKey(VK_RETURN);  
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_backspaceBtnPressed()
{
  simulatePressKey(VK_BACK);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_nullBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD0);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_oneBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD1);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_twoBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD2);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_threeBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD3);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_fourBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD4);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_fiveBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD5);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_sixBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD6);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_sevenBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD7);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_eightBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD8);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_nineBtnReleased()
{
  simulateReleaseKey(VK_NUMPAD9);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_divBtnReleased()
{
  simulateReleaseKey(VK_DIVIDE);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_multiplyBtnReleased()
{
  simulateReleaseKey(VK_MULTIPLY);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_minusBtnReleased()
{
  simulateReleaseKey(VK_SUBTRACT);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_decimalBtnReleased()
{
  simulateReleaseKey(VK_DECIMAL);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_plusBtnReleased()
{
  simulateReleaseKey(VK_ADD);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_enterBtnReleased()
{
  simulateReleaseKey(VK_RETURN);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_backspaceBtnReleased()
{
  simulateReleaseKey(VK_BACK);
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_upBtnPressed()
{
    simulatePressKey(VK_UP);
}

void Numpad::slot_downBtnPressed()
{
    simulatePressKey(VK_DOWN);
}

void Numpad::slot_leftBtnPressed()
{
    simulatePressKey(VK_LEFT);
}

void Numpad::slot_rightBtnPressed()
{
    simulatePressKey(VK_RIGHT);
}

void Numpad::slot_upBtnReleased()
{
    simulateReleaseKey(VK_UP);
}

void Numpad::slot_downBtnReleased()
{
    simulateReleaseKey(VK_DOWN);
}

void Numpad::slot_leftBtnReleased()
{
    simulateReleaseKey(VK_LEFT);
}

void Numpad::slot_rightBtnReleased()
{
    simulateReleaseKey(VK_RIGHT);
}

void Numpad::slot_equalBtnPressed()
{
    if (pm_altBtn && pm_altBtn->isChecked())
    {
        return;
    }
    simulatePressKey(VK_MENU);
    simulatePressKey(VK_NUMPAD6);
    simulateReleaseKey(VK_NUMPAD6);
    simulatePressKey(VK_NUMPAD1);
    simulateReleaseKey(VK_NUMPAD1);
    simulateReleaseKey(VK_MENU);
}

void Numpad::slot_altBtnToggled(bool checked)
{    
    int altKeyCode = VK_MENU;
    if (checked)
    {
        simulatePressKey(altKeyCode);
        m_altBtnPress = true;
    }
    else
    {
        simulateReleaseKey(altKeyCode);
        m_altBtnPress = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
    
 /*override*/ void Numpad::paintEvent(QPaintEvent *)
{
  resize(pm_buttonsBox->width(), pm_buttonsBox->height());
  setFixedSize(sizeHint());
}

////////////////////////////////////////////////////////////////////////////////   
    
/*override*/ bool Numpad::winEvent(MSG  *p_msg, long *p_result)
{
  if (p_msg->message == WM_NCLBUTTONDOWN)
  {
    if (m_noActivateStyleSet)
    {
      HWND lastFocusWindow = GetForegroundWindow();
      if (lastFocusWindow != winId())
      {
        m_lastFocusWindow = lastFocusWindow;
      }   
      unsetNoActivateStyle();
      SetForegroundWindow(winId());
    } 
  } 

  if (p_msg->message == WM_NCMOUSELEAVE)
  {
    SHORT leftButtonPress = GetAsyncKeyState(VK_LBUTTON);
  	if (!m_noActivateStyleSet && (leftButtonPress == 1 || leftButtonPress == 0))
    {
      setNoActivateStyle();
      SetForegroundWindow(m_lastFocusWindow);
    }
  }
  return QWidget::winEvent(p_msg, p_result);
}	

////////////////////////////////////////////////////////////////////////////////

void Numpad::setNoActivateStyle()
{
  HWND hwnd = winId();
  LONG styles = GetWindowLong(hwnd, GWL_EXSTYLE);
  SetWindowLong(hwnd, GWL_EXSTYLE, styles | WS_EX_NOACTIVATE);
  m_noActivateStyleSet = true;
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::unsetNoActivateStyle()
{
  HWND hwnd = winId();
  LONG styles = GetWindowLong(hwnd, GWL_EXSTYLE);
  SetWindowLong(hwnd, GWL_EXSTYLE, styles & ~WS_EX_NOACTIVATE);
  m_noActivateStyleSet = false;
}   

////////////////////////////////////////////////////////////////////////////////

/*override*/ void Numpad::hideEvent(QHideEvent *)
{    
    if (pm_altBtn && pm_altBtn->isChecked())
    {
        pm_altBtn->setChecked(false);
    }    
}

////////////////////////////////////////////////////////////////////////////////

/*override*/ void Numpad::showEvent(QShowEvent *)
{
    if (m_firstShow && m_altCodeLblMode && pm_altBtn)
    {
        createAltCodeLbl();
        correctAltCodeLblFontSize();
        pm_showAltCodeLblTimer = new QTimer(this);
        connect(pm_showAltCodeLblTimer, SIGNAL(timeout()),
                this, SLOT(slot_showAltCodeLblTimeout()));
        pm_showAltCodeLblTimer->start(5000);        
    }
    m_firstShow = false;
    emit showed();
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::slot_showAltCodeLblTimeout()
{
    pm_showAltCodeLblTimer->stop();
    if (pm_altCodeLbl)
    {
        pm_gridLayout->removeWidget(pm_altCodeLbl);
        delete pm_altCodeLbl;
        pm_altCodeLbl = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Numpad::initAltCodesList()
{
    m_altCodes << "<center>alt 1 alt</center>"
                  "<center>&#x263A;</center>"
               << "<center>alt 2 alt</center>"
                  "<center>&#x263B;</center>"
               << "<center>alt 3 alt</center>"
                  "<center>&#x2665;</center>"
               << "<center>alt 4 alt</center>"
                  "<center>&#x2666;</center>"
               << "<center>alt 5 alt</center>"
                  "<center>&#x2663;</center>"
               << "<center>alt 6 alt</center>"
                  "<center>&#x2660;</center>"
               << "<center>alt 8 alt</center>"
                  "<center>&#x25D8;</center>"
               << "<center>alt 10 alt</center>"
                  "<center>&#x25D9;</center>"
               << "<center>alt 11 alt</center>"
                  "<center>&#x2642;</center>"
               << "<center>alt 12 alt</center>"
                  "<center>&#x2640;</center>"
               << "<center>alt 13 alt</center>"
                  "<center>&#x266A;</center>"
               << "<center>alt 14 alt</center>"
                  "<center>&#x266B;</center>"
               << "<center>alt 15 alt</center>"
                  "<center>&#x263C;</center>"
               << "<center>alt 16 alt</center>"
                  "<center>&#x25BA;</center>"
               << "<center>alt 17 alt</center>"
                  "<center>&#x25C4;</center>"
               << "<center>alt 18 alt</center>"
                  "<center>&#x2195;</center>"
               << "<center>alt 19 alt</center>"
                  "<center>&#x203C;</center>"
               << "<center>alt 24 alt</center>"
                  "<center>&#x2191;</center>"
               << "<center>alt 25 alt</center>"
                  "<center>&#x2193;</center>"
               << "<center>alt 26 alt</center>"
                  "<center>&#x2192;</center>"
               << "<center>alt 27 alt</center>"
                  "<center>&#x2190;</center>"
               << "<center>alt 28 alt</center>"
                  "<center>&#x221F;</center>"
               << "<center>alt 29 alt</center>"
                  "<center>&#x2194;</center>"
               << "<center>alt 30 alt</center>"
                  "<center>&#x25B2;</center>"
               << "<center>alt 31 alt</center>"
                  "<center>&#x25BC;</center>"
               << "<center>alt 127 alt</center>"
                  "<center>&#x2302;</center>";
}
