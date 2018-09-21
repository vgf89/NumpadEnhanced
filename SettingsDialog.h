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


#ifndef _SettingsDialog_h_
#define _SettingsDialog_h_

#include <QDialog>

class Numpad;
class NumpadManager;
class AddBtnsSettingsDialog;

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  SettingsDialog(NumpadManager *, Numpad *, int initBtnSize,
                 int initSpacing, QWidget *p_wid = 0);
  void setNumpad(Numpad *);

protected slots:
  void slot_buttonsSizeChanged(int);
  void slot_spacingChanged(int);
  void slot_btnsColorBtnClicked();
  void slot_btnsHoverColorBtnClicked();
  void slot_btnsPressedColorBtnClicked();
  void slot_backgroundColorBtnClicked();
  void slot_symbolsColorBtnClicked();
  void slot_symbolsHoverColorBtnClicked();
  void slot_symbolsPressedColorBtnClicked();
  void slot_resetColorsToDefault();
  void slot_fontBtnClicked();
  void slot_keyComboBoxItemActivated(const QString &);
  void slot_altCodeLblModeStateChanged(int);
  void slot_autoRunStateChanged(int);
  void slot_buttonsSizeSliderReleased();
  void slot_spacingSliderReleased();
  void slot_addBtnsBtnClicked();

private:
  void showNumpadOnBackground();
  void showNumpadOnTop();

  NumpadManager *pm_numpadManager;
  Numpad *pm_numpad;
  AddBtnsSettingsDialog *pm_addBtnsDlg;
};

#endif
