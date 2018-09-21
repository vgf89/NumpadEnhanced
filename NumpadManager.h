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


#ifndef _NumpadManager_h_
#define _NumpadManager_h_

#include <QWidget>
#include <QSystemTrayIcon>
#include <vector>
#include <Windows.h>
#include "Numpad.h"

class SettingsDialog;
class AboutWindow;
struct Key;
class QColor;
class QSettings;
class QAction;
class FirstRunDialog;

class NumpadManager : public QWidget
{
  Q_OBJECT

public:
  NumpadManager(QWidget *p_widget = 0);
  ~NumpadManager();
  bool setKeyForShowHideNumpad(const QString &);
  std::vector<Key *> getKeys() { return m_keysForShowHideNumpad; }
  Key *getKeyForShowHideNumpad() { return pm_keyForShowHideNumpad; }
  bool keyboardHookIsSetGood() { return m_keyboardHookSetGood; }
  bool isAutoRunSet();
  void setAutoRun(bool state);
  void writeButtonsBackgroundColorToSettings();
  void writeButtonsTextColorToSettings();
  void writeButtonsHoverBackgroundColorToSettings();
  void writeButtonsHoverTextColorToSettings();
  void writeButtonsPressedBackgroundColorToSettings();
  void writeButtonsPressedTextColorToSettings();
  void writeBackgroundColorToSettings();
  void writeButtonsSizeToSettings();
  void writeSpacingToSettings();
  void writeFontToSettings();
  void writeAltCodeLblMode();
  void writeKeyToSettings();
  void createExtraBtns(ExtraBtnsType);

  void resetColorsToDefault();

protected:
  /*override*/ bool winEvent(MSG  *p_msg, long *p_result);

protected slots:
  void slot_showHideNumpad();
  void slot_numpadSettings();
  void slot_about();
  void slot_systemTrayClicked(QSystemTrayIcon::ActivationReason reason);
  void slot_numpadShowed();
  void slot_timeout();
  void slot_okClicked();
  void slot_openSetDlgClicked();

private:
  void createNumpad(ExtraBtnsType);
  void createSystemTray();
  void readColorsFromSettings(QColor*, QColor*, QColor*, QColor*, QColor*, QColor*, QColor*);
  void readSizeFromSettings();
  QString readKeyFromSettings();  
  QFont readFontFromSettings();
  bool readAltCodeLblMode();
  void showHideNumpadThroPressKey();
  ExtraBtnsType readExtraBtnsType();

  Numpad *pm_numpad;
  QSystemTrayIcon *pm_systemTray;  
  SettingsDialog *pm_settingsDialog;
  AboutWindow *pm_aboutWindow;
  int m_initButtonsSize;
  int m_initSpacing;
  QColor m_defaultButtonsBackgroundColor;
  QColor m_defaultButtonsTextColor;
  QColor m_defaultBackgroundColor;
  QColor m_defaultButtonsHoverBackgroundColor;
  QColor m_defaultButtonsHoverTextColor;
  QColor m_defaultButtonsPressedBackgroundColor;
  QColor m_defaultButtonsPressedTextColor;
  int m_defaultButtonsSize;
  int m_defaultSpacing;
  QFont m_defaultFont;
  QSettings *pm_settings;
  QSettings *pm_autoRunSettings;
  int m_diffPosNumpadAndCursor;
  HHOOK m_hookKeyboard; 
  HINSTANCE m_keyboardHookDll; 
  Key *pm_keyForShowHideNumpad;
  std::vector<Key *> m_keysForShowHideNumpad;
  QString m_defaultStrKeyForShowHideNumpad;
  bool m_keyboardHookSetGood;
  QAction *pm_showHideNumpadAction;  
  bool m_firstRun;
  QTimer *pm_timer;
  FirstRunDialog *pm_firstRunDlg;
};

#endif
