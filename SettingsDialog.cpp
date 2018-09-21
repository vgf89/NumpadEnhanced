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


#include "SettingsDialog.h"
#include "Numpad.h"
#include "NumpadManager.h"
#include "Key.h"
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFontMetrics>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QFont>
#include <QComboBox>
#include <QCheckBox>
#include <vector>
#include "AddBtnsSettingsDialog.h"

SettingsDialog::SettingsDialog(NumpadManager *p_numpadManager, Numpad *p_numpad,
                               int initBtnSize, int initSpacing, QWidget *p_wid/*= 0*/)
: QDialog(p_wid, Qt::WindowTitleHint | Qt::WindowStaysOnTopHint),
  pm_numpadManager(p_numpadManager),
  pm_numpad(p_numpad),
  pm_addBtnsDlg(NULL)
{
  setWindowTitle("Settings");

  QCheckBox *p_autoRunCheckBox = new QCheckBox("Autorun");
  p_autoRunCheckBox->setChecked(pm_numpadManager->isAutoRunSet());

  QHBoxLayout *p_horAutoRunLayout = new QHBoxLayout;
  p_horAutoRunLayout->addWidget(p_autoRunCheckBox);
  p_horAutoRunLayout->addStretch(1);
  QGroupBox *p_autoRunGrpBox = new QGroupBox;
  p_autoRunGrpBox->setLayout(p_horAutoRunLayout);
  
  QLabel *p_keyLbl = new QLabel("Key for show/hide: ");
  
  QComboBox *p_keysComboBox = new QComboBox;
  std::vector<Key *> keys = pm_numpadManager->getKeys();
  std::vector<Key *>::iterator iter = keys.begin();
  while (iter != keys.end())
  {
    p_keysComboBox->addItem((*iter)->strRepresent);
    iter++;
  }
  Key *p_currentKey = pm_numpadManager->getKeyForShowHideNumpad();
  int currentIndex = p_keysComboBox->findText(p_currentKey->strRepresent);
  p_keysComboBox->setCurrentIndex(currentIndex);
   
  QHBoxLayout *p_keyLayout = new QHBoxLayout;
  p_keyLayout->addWidget(p_keyLbl);
  p_keyLayout->addWidget(p_keysComboBox);
  p_keyLayout->addStretch(1);
  QGroupBox *p_keyGroupBox = new QGroupBox;
  p_keyGroupBox->setLayout(p_keyLayout);
  p_keyGroupBox->setEnabled(pm_numpadManager->keyboardHookIsSetGood());
  
  QLabel *p_buttonsSizeLbl = new QLabel("Buttons size");
  QSlider *p_buttonsSizeSlider = new QSlider(Qt::Horizontal); 
  p_buttonsSizeSlider->setRange(pm_numpad->getMinButtonsSize(),
                                pm_numpad->getMaxButtonsSize());
  p_buttonsSizeSlider->setValue(initBtnSize); 
  p_buttonsSizeSlider->setSingleStep(1);
  p_buttonsSizeSlider->setPageStep(5);
  QHBoxLayout *p_buttonsSizeLayout = new QHBoxLayout;
  p_buttonsSizeLayout->addWidget(p_buttonsSizeLbl);
  p_buttonsSizeLayout->addWidget(p_buttonsSizeSlider);
  p_buttonsSizeLayout->addStretch(1);
      
  QLabel *p_spacingLbl = new QLabel("Spacing");
  QSlider *p_spacingSlider = new QSlider(Qt::Horizontal);  
  p_spacingSlider->setRange(pm_numpad->getMinSpacing(), pm_numpad->getMaxSpacing());
  p_spacingSlider->setValue(initSpacing);
  p_spacingSlider->setSingleStep(1);
  p_spacingSlider->setPageStep(5);
  QHBoxLayout *p_spacingLayout = new QHBoxLayout;
  p_spacingLayout->addWidget(p_spacingLbl);
  p_spacingLayout->addWidget(p_spacingSlider);
  p_spacingLayout->addStretch(1);
  
  QFontMetrics font = QFontMetrics(p_buttonsSizeLbl->font());
  int sizeGrpWidth = font.width("Buttons size");
  p_buttonsSizeLbl->setFixedWidth(sizeGrpWidth);
  p_spacingLbl->setFixedWidth(sizeGrpWidth);
  p_buttonsSizeSlider->setFixedWidth(sizeGrpWidth * 2);
  p_spacingSlider->setFixedWidth(sizeGrpWidth * 2);

  QVBoxLayout *p_sizesLayout = new QVBoxLayout;
  p_sizesLayout->addLayout(p_buttonsSizeLayout);
  p_sizesLayout->addLayout(p_spacingLayout);
   
  QGroupBox *p_sizesGrpBox = new QGroupBox;
  p_sizesGrpBox->setLayout(p_sizesLayout);

  QPushButton *p_addBtnsBtn = new QPushButton("Additional buttons...");
  QHBoxLayout *p_addBtnsLayout = new QHBoxLayout;
  p_addBtnsLayout->addWidget(p_addBtnsBtn);
  p_addBtnsLayout->addStretch(1);

  QGroupBox *p_addBtnsGrpBox = new QGroupBox;
  p_addBtnsGrpBox->setLayout(p_addBtnsLayout);

  QCheckBox *p_altCodeLblModeCheckBox = new QCheckBox("Show alt-code");
  p_altCodeLblModeCheckBox->setChecked(pm_numpad->getAltCodeLblMode());

  QHBoxLayout *p_horAltCodeLblModeLayout = new QHBoxLayout;
  p_horAltCodeLblModeLayout->addWidget(p_altCodeLblModeCheckBox);
  p_horAltCodeLblModeLayout->addStretch(1);
  QGroupBox *p_altCodeLblModeGrpBox = new QGroupBox;
  p_altCodeLblModeGrpBox->setLayout(p_horAltCodeLblModeLayout);

  QPushButton *p_fontBtn = new QPushButton("Font...");
  QHBoxLayout *p_fontLayout = new QHBoxLayout;
  p_fontLayout->addWidget(p_fontBtn);
  p_fontLayout->addStretch(1);

  QGroupBox *p_fontGrpBox = new QGroupBox;
  p_fontGrpBox->setLayout(p_fontLayout);

  QPushButton *p_backgroundColorBtn = new QPushButton("Background...");
  QPushButton *p_btnsColorBtn = new QPushButton("Buttons...");
  QPushButton *p_btnsHoverColorBtn = new QPushButton("Buttons Hover...");
  QPushButton *p_btnsPressedColorBtn = new QPushButton("Buttons Pressed...");
  QPushButton *p_symbolsColorBtn = new QPushButton("Symbols...");
  QPushButton *p_symbolsHoverColorBtn = new QPushButton("Symbols Hover...");
  QPushButton *p_symbolsPressedColorBtn = new QPushButton("Symbols Pressed...");

  QFrame *line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);

  QPushButton *p_resetDefaultColors = new QPushButton("Restore Defaults");

  
  QVBoxLayout *p_vertColorLayout = new QVBoxLayout;
  p_vertColorLayout->addWidget(p_backgroundColorBtn);
  p_vertColorLayout->addWidget(p_btnsColorBtn);
  p_vertColorLayout->addWidget(p_btnsHoverColorBtn);
  p_vertColorLayout->addWidget(p_btnsPressedColorBtn);
  p_vertColorLayout->addWidget(p_symbolsColorBtn);
  p_vertColorLayout->addWidget(p_symbolsHoverColorBtn);
  p_vertColorLayout->addWidget(p_symbolsPressedColorBtn);
  p_vertColorLayout->addWidget(line);
  p_vertColorLayout->addWidget(p_resetDefaultColors);
  
  QHBoxLayout *p_horColorLayout = new QHBoxLayout;
  p_horColorLayout->addLayout(p_vertColorLayout);
  p_horColorLayout->addStretch(1);
  
  QGroupBox *p_colorGrpBox = new QGroupBox;
  p_colorGrpBox->setLayout(p_horColorLayout);
  p_colorGrpBox->setTitle("Colors");   
  
  QVBoxLayout *p_mainLayout = new QVBoxLayout;
  p_mainLayout->addWidget(p_autoRunGrpBox);
  p_mainLayout->addWidget(p_keyGroupBox);
  p_mainLayout->addWidget(p_sizesGrpBox);
  p_mainLayout->addWidget(p_addBtnsGrpBox);
  p_mainLayout->addWidget(p_altCodeLblModeGrpBox);
  p_mainLayout->addWidget(p_fontGrpBox);
  p_mainLayout->addWidget(p_colorGrpBox);
  
  setLayout(p_mainLayout);
  
  connect(p_keysComboBox, SIGNAL(activated(const QString &)),
              SLOT(slot_keyComboBoxItemActivated(const QString &)));
  connect(p_buttonsSizeSlider, SIGNAL(valueChanged(int)), SLOT(slot_buttonsSizeChanged(int)));
  connect(p_spacingSlider, SIGNAL(valueChanged(int)), SLOT(slot_spacingChanged(int)));
  connect(p_btnsColorBtn, SIGNAL(clicked()), SLOT(slot_btnsColorBtnClicked()));
  connect(p_btnsHoverColorBtn, SIGNAL(clicked()), SLOT(slot_btnsHoverColorBtnClicked()));
  connect(p_btnsPressedColorBtn, SIGNAL(clicked()), SLOT(slot_btnsPressedColorBtnClicked()));
  connect(p_backgroundColorBtn, SIGNAL(clicked()), SLOT(slot_backgroundColorBtnClicked()));
  connect(p_symbolsColorBtn, SIGNAL(clicked()), SLOT(slot_symbolsColorBtnClicked()));
  connect(p_symbolsHoverColorBtn, SIGNAL(clicked()), SLOT(slot_symbolsHoverColorBtnClicked()));
  connect(p_symbolsPressedColorBtn, SIGNAL(clicked()), SLOT(slot_symbolsPressedColorBtnClicked()));
  connect(p_resetDefaultColors, SIGNAL(clicked()), SLOT(slot_resetColorsToDefault()));

  connect(p_fontBtn, SIGNAL(clicked()), SLOT(slot_fontBtnClicked()));
  connect(p_altCodeLblModeCheckBox, SIGNAL(stateChanged(int)),
          SLOT(slot_altCodeLblModeStateChanged(int)));
  connect(p_autoRunCheckBox, SIGNAL(stateChanged(int)),
          SLOT(slot_autoRunStateChanged(int)));
  connect(p_buttonsSizeSlider, SIGNAL(sliderReleased()),
          SLOT(slot_buttonsSizeSliderReleased()));
  connect(p_spacingSlider, SIGNAL(sliderReleased()),
          SLOT(slot_spacingSliderReleased()));
  connect(p_addBtnsBtn, SIGNAL(clicked()), SLOT(slot_addBtnsBtnClicked()));

  setFixedSize(sizeHint());
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::showNumpadOnBackground()
{
  if (pm_numpad->isHidden())
  {
    pm_numpad->show();
  }  
  activateWindow();
}

////////////////////////////////////////////////////////////////////////////////  

void SettingsDialog::slot_keyComboBoxItemActivated(const QString & keyStr)
{
  if (pm_numpadManager->setKeyForShowHideNumpad(keyStr))
  {
      pm_numpadManager->writeKeyToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_buttonsSizeChanged(int size)
{  
  pm_numpad->setButtonsSize(size, true);
  showNumpadOnBackground();  
}  

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_spacingChanged(int spacing)
{ 
  pm_numpad->setSpacing(spacing);
  showNumpadOnBackground();  
}  

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::showNumpadOnTop()
{
  if (pm_numpad->isHidden())
  {
    pm_numpad->show();
  }  
}

////////////////////////////////////////////////////////////////////////////////  

void SettingsDialog::slot_btnsColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getButtonsBackgroundColor(),
                                        this, "Buttons color",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {    
    pm_numpad->setButtonsBackgroundColor(color);
    showNumpadOnTop();
    pm_numpadManager->writeButtonsBackgroundColorToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_btnsHoverColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getButtonsHoverBackgroundColor(),
                                        this, "Buttons hover color",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {
    pm_numpad->setButtonsHoverBackgroundColor(color);
    showNumpadOnTop();
    pm_numpadManager->writeButtonsHoverBackgroundColorToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_btnsPressedColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getButtonsPressedBackgroundColor(),
                                        this, "Buttons pressed color",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {
    pm_numpad->setButtonsPressedBackgroundColor(color);
    showNumpadOnTop();
    pm_numpadManager->writeButtonsPressedBackgroundColorToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_backgroundColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getBackgroundColor(),
                                        this, "Background color", 
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {    
    pm_numpad->setBackgroundColor(color);
    showNumpadOnTop();
    pm_numpadManager->writeBackgroundColorToSettings();
  } 
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_symbolsColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getButtonsTextColor(),
                                        this, "Symbols color", 
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {    
    pm_numpad->setButtonsTextColor(color);
    showNumpadOnTop();
    pm_numpadManager->writeButtonsTextColorToSettings();
  } 
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_symbolsHoverColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getButtonsHoverTextColor(),
                                        this, "Symbols hover color",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {
    pm_numpad->setButtonsHoverTextColor(color);
    showNumpadOnTop();
    pm_numpadManager->writeButtonsHoverTextColorToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_symbolsPressedColorBtnClicked()
{
  QColor color = QColorDialog::getColor(pm_numpad->getButtonsPressedTextColor(),
                                        this, "Symbols pressed color",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {
    pm_numpad->setButtonsPressedTextColor(color);
    showNumpadOnTop();
    pm_numpadManager->writeButtonsPressedTextColorToSettings();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_resetColorsToDefault()
{
    pm_numpadManager->resetColorsToDefault();
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_fontBtnClicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, pm_numpad->getButtonsFont(),
                                      this, "Select Font");
    if (ok)
    {
        pm_numpad->setButtonsFont(font);
        showNumpadOnTop();
        pm_numpadManager->writeFontToSettings();
    }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_altCodeLblModeStateChanged(int state)
{
    if (state == Qt::Checked)
    {
        pm_numpad->setAltCodeLblMode(true);
    }
    else if (state == Qt::Unchecked)
    {
        pm_numpad->setAltCodeLblMode(false);
    }
    pm_numpadManager->writeAltCodeLblMode();
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_autoRunStateChanged(int state)
{
    if (state == Qt::Checked)
    {
        pm_numpadManager->setAutoRun(true);
    }
    else if (state == Qt::Unchecked)
    {
        pm_numpadManager->setAutoRun(false);
    }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_buttonsSizeSliderReleased()
{
    pm_numpadManager->writeButtonsSizeToSettings();
    pm_numpadManager->writeFontToSettings();
}

////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::slot_spacingSliderReleased()
{
    pm_numpadManager->writeSpacingToSettings();
}

void SettingsDialog::slot_addBtnsBtnClicked()
{
    if (!pm_addBtnsDlg)
    {
        pm_addBtnsDlg = new AddBtnsSettingsDialog(pm_numpadManager, pm_numpad, this);
    }
    pm_addBtnsDlg->show();
}

void SettingsDialog::setNumpad(Numpad *p_numpad)
{
    pm_numpad = p_numpad;
    pm_addBtnsDlg->setNumpad(p_numpad);
}
