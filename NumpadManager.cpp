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


#include "NumpadManager.h"
#include "KeyboardHookMsg.h"
#include "Numpad.h"
#include "SettingsDialog.h"
#include "AboutWindow.h"
#include "Key.h"
#include "FirstRunDialog.h"
#include <QMenu>
#include <QApplication>
#include <QColor>
#include <QString>
#include <QCursor>
#include <QDesktopWidget>
#include <QFontMetrics>
#include <QSettings>
#include <QAction>
#include <QDir>
#include <QTimer>

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")

NumpadManager::NumpadManager(QWidget *p_parent/*= 0*/)
: QWidget(p_parent)
{
  setWindowTitle("NumpadEnhancedManager_1.2");
  winId();

  QApplication::setWindowIcon(QIcon(":/numpad_icon.png"));
  
  QApplication::setQuitOnLastWindowClosed(false);

  m_defaultButtonsSize = 60;
  m_defaultSpacing = 0;
  
  m_defaultButtonsBackgroundColor = "#333333";
  m_defaultButtonsTextColor = "#FFFFFF";
  m_defaultButtonsHoverBackgroundColor = "#419FFE";
  m_defaultButtonsHoverTextColor = "#FFFFFF";
  m_defaultButtonsPressedBackgroundColor = "#E5E5E5";
  m_defaultButtonsPressedTextColor = "#000000";
  m_defaultBackgroundColor = "#1A1A1A";

  m_defaultFont.setPixelSize(20);  
   
  pm_settings = new QSettings("NumpadEnhanced", "NumpadEnhanced_1.2", this);

  pm_autoRunSettings = new QSettings ("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\"
                     "CurrentVersion\\Run", QSettings::NativeFormat, this);
    
  m_firstRun = pm_settings->value("/Settings/FirstRun", true).toBool();

  createNumpad(readExtraBtnsType());

  createSystemTray();  
  
  pm_systemTray->show();
                              
  pm_settingsDialog = NULL;    
  pm_aboutWindow = NULL;         
  
  QFontMetrics numpadFont(pm_numpad->font());
  m_diffPosNumpadAndCursor = numpadFont.width("num");               
 
  m_keyboardHookSetGood = false;
  m_hookKeyboard = NULL;
    
  m_keyboardHookDll = LoadLibrary(TEXT("KeyboardHook.dll"));
  if (m_keyboardHookDll)
  {
    typedef void (*SetHwnd) (HWND);
    SetHwnd setHwnd = (SetHwnd) GetProcAddress(m_keyboardHookDll, "setAppHwnd");
    if (setHwnd)
    {
      setHwnd(winId());
      HOOKPROC keyboardHookProc = (HOOKPROC) GetProcAddress(m_keyboardHookDll, 
                                                           "_LowLevelKeyboardProc@12");
      if (keyboardHookProc)
      {
        m_hookKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc,
                                          m_keyboardHookDll, 0);
        if (m_hookKeyboard)
        {
          m_keyboardHookSetGood = true;
        }        
      }
    }
  }                                            
               
  int funcKeyNumber = 2;
  int qtCode = Qt::Key_F2;
  for (int winCode = VK_F2; winCode <= VK_F12; winCode++)
  {
    m_keysForShowHideNumpad.push_back(new Key("F" + QString().setNum(funcKeyNumber), 
                                              winCode, qtCode));
    funcKeyNumber++;
    qtCode++;
  }
  
  m_defaultStrKeyForShowHideNumpad = "F9";
  setKeyForShowHideNumpad(readKeyFromSettings());
  
  if (m_keyboardHookSetGood)
  {
      pm_systemTray->showMessage("", "NumpadEnhanced (<" +
                               pm_keyForShowHideNumpad->strRepresent +
                               ">)");
  }
  else 
  {
     pm_systemTray->showMessage("", "NumpadEnhanced");
  }            
}  

////////////////////////////////////////////////////////////////////////////////

NumpadManager::~NumpadManager()
{
  if (m_hookKeyboard)
  {
    UnhookWindowsHookEx(m_hookKeyboard);
  }
  if (m_keyboardHookDll)
  {
    FreeLibrary(m_keyboardHookDll);
  }

  std::vector<Key *>::iterator iter = m_keysForShowHideNumpad.begin();
  while (iter != m_keysForShowHideNumpad.end())
  {
    delete *iter;
    iter++;
  }
  
  delete pm_numpad;
}  

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::createNumpad(ExtraBtnsType type)
{    
  pm_numpad = new Numpad(type);

  pm_numpad->setAltCodeLblMode(readAltCodeLblMode());

  QColor initButtonsBackgroundColor; 
  QColor initButtonsTextColor;
  QColor initButtonsHoverBackgroundColor;
  QColor initButtonsHoverTextColor;
  QColor initButtonsPressedBackgroundColor;
  QColor initButtonsPressedTextColor;

  QColor initBackgroundColor;
  readColorsFromSettings(&initButtonsBackgroundColor,
                         &initButtonsTextColor,
                         &initButtonsHoverBackgroundColor,
                         &initButtonsHoverTextColor,
                         &initButtonsPressedBackgroundColor,
                         &initButtonsPressedTextColor,
                         &initBackgroundColor);
  pm_numpad->setButtonsBackgroundColor(initButtonsBackgroundColor);
  pm_numpad->setButtonsTextColor(initButtonsTextColor);
  pm_numpad->setButtonsHoverBackgroundColor(initButtonsHoverBackgroundColor);
  pm_numpad->setButtonsHoverTextColor(initButtonsHoverTextColor);
  pm_numpad->setButtonsPressedBackgroundColor(initButtonsPressedBackgroundColor);
  pm_numpad->setButtonsPressedTextColor(initButtonsPressedTextColor);
  pm_numpad->setBackgroundColor(initBackgroundColor);
  
  readSizeFromSettings();
  pm_numpad->setButtonsSize(m_initButtonsSize, false);
  pm_numpad->setSpacing(m_initSpacing);  

  pm_numpad->correctSize();

  QFont font = readFontFromSettings();
  pm_numpad->setButtonsFont(font);

  connect(pm_numpad, SIGNAL(showed()), SLOT(slot_numpadShowed()));
}


void NumpadManager::createExtraBtns(ExtraBtnsType type)
{
    pm_settingsDialog->setEnabled(false);

    bool wasVisible = false;
    int xNumpad, yNumpad;
    if (pm_numpad->isVisible())
    {
        wasVisible = true;
        xNumpad = pm_numpad->x();
        yNumpad = pm_numpad->y();
        pm_numpad->hide();
    }
    delete pm_numpad;

    createNumpad(type);
    if (wasVisible)
    {
        pm_numpad->move(xNumpad, yNumpad);
    }
    pm_numpad->setNoActivateStyle();
    pm_numpad->show();

    pm_settingsDialog->setNumpad(pm_numpad);

    pm_settings->setValue("/Settings/ExtraBtnsType", (int)type);

    pm_settingsDialog->setEnabled(true);
}


ExtraBtnsType NumpadManager::readExtraBtnsType()
{
    int val = pm_settings->value("/Settings/ExtraBtnsType", 0).toInt();
    ExtraBtnsType type;
    switch (val) {
    case 0:
        type = None;
        break;
    case 1:
        type = AltCodes;
        break;
    case 2:
        type = Excel;
        break;
    case 3:
        type = AltCodes_Excel;
        break;
    default:
        type = None;
    }
    return type;
}


void NumpadManager::slot_numpadShowed()
{
    disconnect(pm_numpad, SIGNAL(showed()), this, SLOT(slot_numpadShowed()));

    if (m_firstRun)
    {
        m_firstRun = false;
        pm_settings->setValue("/Settings/FirstRun", false);
        pm_timer = new QTimer();
        connect(pm_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
        pm_timer->start(5000);
    }
}

void NumpadManager::slot_timeout()
{
    pm_timer->stop();
    delete pm_timer;
    pm_firstRunDlg = new FirstRunDialog(pm_numpad);
    connect(pm_firstRunDlg, SIGNAL(okClicked()), this, SLOT(slot_okClicked()));
    connect(pm_firstRunDlg, SIGNAL(openSetDlgClicked()), this, SLOT(slot_openSetDlgClicked()));
    pm_firstRunDlg->show();
}

void NumpadManager::slot_okClicked()
{
    pm_firstRunDlg->hide();
    delete pm_firstRunDlg;
}

void NumpadManager::slot_openSetDlgClicked()
{
    pm_firstRunDlg->hide();
    delete pm_firstRunDlg;
    slot_numpadSettings();
}

////////////////////////////////////////////////////////////////////////////////  

void NumpadManager::createSystemTray()
{
  pm_systemTray = new QSystemTrayIcon(QIcon(":/numpad_icon.png"));
  
  QMenu *p_trayMenu = new QMenu();
  pm_showHideNumpadAction = p_trayMenu->addAction("Show/Hide numpad", this,
                                                  SLOT(slot_showHideNumpad()));
  p_trayMenu->addAction("Settings", this, SLOT(slot_numpadSettings()));   
  p_trayMenu->addAction("About", this, SLOT(slot_about()));                   
  p_trayMenu->addAction("Quit", qApp, SLOT(quit()));
  
  pm_systemTray->setContextMenu(p_trayMenu);
    
  connect(pm_systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          SLOT(slot_systemTrayClicked(QSystemTrayIcon::ActivationReason)));
}

////////////////////////////////////////////////////////////////////////////////

bool NumpadManager::readAltCodeLblMode()
{
    return pm_settings->value("/Settings/AltCodeLblMode",
                                          true).toBool();
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeAltCodeLblMode()
{
    pm_settings->setValue("/Settings/AltCodeLblMode",
                          pm_numpad->getAltCodeLblMode());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::readColorsFromSettings(QColor *p_buttonsBackgroundColor,
                                           QColor *p_buttonsTextColor,
                                           QColor *p_buttonsHoverBackgroundColor,
                                           QColor *p_buttonsHoverTextColor,
                                           QColor *p_buttonsPressedBackgroundColor,
                                           QColor *p_buttonsPressedTextColor,
                                           QColor *p_backgroundColor)
{

    *p_buttonsBackgroundColor = pm_settings->value("/Settings/Colors/ButtonsBackground",
                                                 m_defaultButtonsBackgroundColor.name()).toString();
    *p_buttonsTextColor = pm_settings->value("/Settings/Colors/ButtonsText",
                                                 m_defaultButtonsTextColor.name()).toString();

    *p_buttonsHoverBackgroundColor = pm_settings->value("/Settings/Colors/ButtonsHoverBackground",
                                                 m_defaultButtonsHoverBackgroundColor.name()).toString();
    *p_buttonsHoverTextColor = pm_settings->value("/Settings/Colors/ButtonsHoverText",
                                                 m_defaultButtonsHoverTextColor.name()).toString();

    *p_buttonsPressedBackgroundColor = pm_settings->value("/Settings/Colors/ButtonsPressedBackground",
                                                 m_defaultButtonsPressedBackgroundColor.name()).toString();
    *p_buttonsPressedTextColor = pm_settings->value("/Settings/Colors/ButtonsPressedText",
                                                 m_defaultButtonsPressedTextColor.name()).toString();



    *p_backgroundColor = pm_settings->value("/Settings/Colors/Background",
                                                   m_defaultBackgroundColor.name()).toString();
}
    
////////////////////////////////////////////////////////////////////////////////                                           

void NumpadManager::writeButtonsBackgroundColorToSettings()
{
    pm_settings->setValue("/Settings/Colors/ButtonsBackground", pm_numpad->getButtonsBackgroundColor().name());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeButtonsTextColorToSettings()
{
    pm_settings->setValue("/Settings/Colors/ButtonsText", pm_numpad->getButtonsTextColor().name());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeButtonsHoverBackgroundColorToSettings()
{
    pm_settings->setValue("/Settings/Colors/ButtonsHoverBackground", pm_numpad->getButtonsHoverBackgroundColor().name());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeButtonsHoverTextColorToSettings()
{
    pm_settings->setValue("/Settings/Colors/ButtonsHoverText", pm_numpad->getButtonsHoverTextColor().name());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeButtonsPressedBackgroundColorToSettings()
{
    pm_settings->setValue("/Settings/Colors/ButtonsPressedBackground", pm_numpad->getButtonsPressedBackgroundColor().name());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeButtonsPressedTextColorToSettings()
{
    pm_settings->setValue("/Settings/Colors/ButtonsPressedText", pm_numpad->getButtonsPressedTextColor().name());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeBackgroundColorToSettings()
{
    pm_settings->setValue("/Settings/Colors/Background", pm_numpad->getBackgroundColor().name());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::readSizeFromSettings()
{
  m_initButtonsSize = pm_settings->value("/Settings/Sizes/Buttons", 
                                             m_defaultButtonsSize).toInt(); 
  m_initSpacing = pm_settings->value("/Settings/Sizes/Spacing", 
                                         m_defaultSpacing).toInt();
}


////////////////////////////////////////////////////////////////////////////////    

void NumpadManager::writeButtonsSizeToSettings()
{
    pm_settings->setValue("/Settings/Sizes/Buttons", pm_numpad->getButtonsSize());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeSpacingToSettings()
{
    pm_settings->setValue("/Settings/Sizes/Spacing", pm_numpad->getSpacing());
}

////////////////////////////////////////////////////////////////////////////////

QString NumpadManager::readKeyFromSettings()
{
  return pm_settings->value("/Settings/KeyForShowHideNumpad", 
                            m_defaultStrKeyForShowHideNumpad).toString();
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeKeyToSettings()
{
  pm_settings->setValue("/Settings/KeyForShowHideNumpad", 
                        pm_keyForShowHideNumpad->strRepresent);
}

///////////////////////////////////////////////////////////////////////////////

QFont NumpadManager::readFontFromSettings()
{
    QVariant variant = pm_settings->value("/Settings/Font", m_defaultFont);
    return variant.value<QFont>();
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::writeFontToSettings()
{
    pm_settings->setValue("/Settings/Font", pm_numpad->getButtonsFont());
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::resetColorsToDefault()
{
        pm_settings->setValue("/Settings/Colors/ButtonsBackground", m_defaultButtonsBackgroundColor.name());
        pm_settings->setValue("/Settings/Colors/ButtonsText", m_defaultButtonsTextColor.name());
        pm_settings->setValue("/Settings/Colors/ButtonsHoverBackground", m_defaultButtonsHoverBackgroundColor.name());
        pm_settings->setValue("/Settings/Colors/ButtonsHoverText", m_defaultButtonsHoverTextColor.name());
        pm_settings->setValue("/Settings/Colors/ButtonsPressedBackground", m_defaultButtonsPressedBackgroundColor.name());
        pm_settings->setValue("/Settings/Colors/ButtonsPressedText", m_defaultButtonsPressedTextColor.name());
        pm_settings->setValue("/Settings/Colors/Background", m_defaultBackgroundColor.name());

        pm_numpad->setButtonsBackgroundColor(m_defaultButtonsBackgroundColor);
        pm_numpad->setButtonsTextColor(m_defaultButtonsTextColor);
        pm_numpad->setButtonsHoverBackgroundColor(m_defaultButtonsHoverBackgroundColor);
        pm_numpad->setButtonsHoverTextColor(m_defaultButtonsHoverTextColor);
        pm_numpad->setButtonsPressedBackgroundColor(m_defaultButtonsPressedBackgroundColor);
        pm_numpad->setButtonsPressedTextColor(m_defaultButtonsPressedTextColor);
        pm_numpad->setBackgroundColor(m_defaultBackgroundColor);
}

////////////////////////////////////////////////////////////////////////////////

bool NumpadManager::setKeyForShowHideNumpad(const QString & strRepresent)
{
  std::vector<Key *>::iterator iter = m_keysForShowHideNumpad.begin();
  while (iter != m_keysForShowHideNumpad.end())
  {
    if (strRepresent == (*iter)->strRepresent)
    {
      pm_keyForShowHideNumpad = *iter;
      if (m_keyboardHookSetGood)
      {
        pm_systemTray->setToolTip("NumpadEnhanced (<" +
                                  pm_keyForShowHideNumpad->strRepresent + ">)");
        pm_showHideNumpadAction->setShortcut(pm_keyForShowHideNumpad->qtCode);                                     
      }
      else
      {
        pm_systemTray->setToolTip("NumpadEnhanced");
      }      
      return true;
    }
    iter++;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////                                    

void NumpadManager::slot_showHideNumpad()
{
  if (pm_numpad->isVisible())
  {
    pm_numpad->unsetNoActivateStyle();
  	pm_numpad->hide();
  }
  else
  {
    pm_numpad->setNoActivateStyle();
  	pm_numpad->show();
  }
}  

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::slot_numpadSettings()
{
  if (!pm_settingsDialog)
  {
    pm_settingsDialog = new SettingsDialog(this, pm_numpad, m_initButtonsSize,
                                           m_initSpacing);
  }
  pm_settingsDialog->show();
  pm_settingsDialog->raise();
  pm_settingsDialog->activateWindow();
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::slot_about()
{
  if (!pm_aboutWindow)
  {
    pm_aboutWindow = new AboutWindow();
  }
  pm_aboutWindow->show();
  pm_aboutWindow->raise();
  pm_aboutWindow->activateWindow();
}

////////////////////////////////////////////////////////////////////////////////    

void NumpadManager::slot_systemTrayClicked(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::Trigger)
  {
    slot_showHideNumpad();
  }  
}  

/////////////////////////////////////////////////////////////////////////////////

/*override*/ bool NumpadManager::winEvent(MSG  *p_msg, long *p_result)
{
  if (p_msg->message == KEYBOARDHOOKMSG)
  {
    if (p_msg->wParam == pm_keyForShowHideNumpad->winCode)
    {
      showHideNumpadThroPressKey();
    }
  }
  return QWidget::winEvent(p_msg, p_result);
}

/////////////////////////////////////////////////////////////////////////////////

void NumpadManager::showHideNumpadThroPressKey()
{
  if (pm_numpad->isVisible())
  {
    pm_numpad->unsetNoActivateStyle();
  	pm_numpad->hide();
  }
  else
  {
    int widthScreen = QApplication::desktop()->width();
    int heightScreen = QApplication::desktop()->height();
    int xCenter =  widthScreen / 2;
    int yCenter =  heightScreen / 2;
    int xCursor = QCursor::pos().x();
    int yCursor = QCursor::pos().y();
    
    int xNumpad;
    int yNumpad;
             
    RECT rectNumpad;
    GetWindowRect(pm_numpad->winId(), &rectNumpad);
    int widthNumpad = rectNumpad.right - rectNumpad.left;
    int heightNumpad = rectNumpad.bottom - rectNumpad.top;
              
    if (xCursor < xCenter && yCursor <= yCenter)
    {
      xNumpad = xCursor + m_diffPosNumpadAndCursor;
      yNumpad = yCursor + m_diffPosNumpadAndCursor;
    }
    if (xCursor >= xCenter && yCursor < yCenter)
    {
      xNumpad = xCursor - m_diffPosNumpadAndCursor - widthNumpad;
      yNumpad = yCursor + m_diffPosNumpadAndCursor;
    }
    if (xCursor > xCenter && yCursor >= yCenter)
    {
      xNumpad = xCursor - m_diffPosNumpadAndCursor - widthNumpad;
      yNumpad = yCursor - m_diffPosNumpadAndCursor - heightNumpad;
    }
    if (xCursor <= xCenter && yCursor > yCenter)
    {
      xNumpad = xCursor + m_diffPosNumpadAndCursor;
      yNumpad = yCursor - m_diffPosNumpadAndCursor - heightNumpad;
    }
    if (xCursor == xCenter && yCursor == yCenter)
    {
      xNumpad = xCursor + m_diffPosNumpadAndCursor;
      yNumpad = yCursor + m_diffPosNumpadAndCursor;
    }    
    
    if (xNumpad + widthNumpad > widthScreen)
    {
      xNumpad = widthScreen - widthNumpad;
    }
    if (yNumpad + heightNumpad > heightScreen)
    {
      yNumpad = heightScreen - heightNumpad;
    }  
    if (xNumpad < 0)
    {
      xNumpad = 0;
    }
    if (yNumpad < 0)
    {
      yNumpad = 0;
    } 
    
    pm_numpad->move(xNumpad, yNumpad);
    pm_numpad->setNoActivateStyle();
  	pm_numpad->show();
  }
}  

////////////////////////////////////////////////////////////////////////////////

bool NumpadManager::isAutoRunSet()
{
    if (pm_autoRunSettings->contains("NumpadEnhanced_1.2"))
    {
        QString autoRunPath =
                pm_autoRunSettings->value("NumpadEnhanced_1.2").toString();
        QString correctAutoRunPath =
                QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        if (autoRunPath == correctAutoRunPath)
        {
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////

void NumpadManager::setAutoRun(bool state)
{
    if (state)
    {
        pm_autoRunSettings->setValue("NumpadEnhanced_1.2",
             QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    }
    else
    {
        pm_autoRunSettings->remove("NumpadEnhanced_1.2");
    }
}
