#include "AddBtnsSettingsDialog.h"
#include "Numpad.h"
#include "NumpadManager.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

AddBtnsSettingsDialog::AddBtnsSettingsDialog(NumpadManager *p_numpadManager,
                                             Numpad *p_numpad, QWidget *p_wid)
    : QDialog(p_wid, Qt::WindowTitleHint),
      pm_numpadManager(p_numpadManager),
      pm_numpad(p_numpad)
{
    setWindowTitle("Additional buttons");
    pm_altCodesChBox = new QCheckBox("Alt, Alt-codes");
    pm_excelChBox = new QCheckBox("Arrows, =");
    QVBoxLayout *p_addBtnsVertLayout = new QVBoxLayout;
    p_addBtnsVertLayout->addWidget(pm_altCodesChBox);
    p_addBtnsVertLayout->addWidget(pm_excelChBox);
    QPushButton *p_applyBtn = new QPushButton("Apply");
    connect(p_applyBtn, SIGNAL(clicked()), SLOT(slot_applyBtnClicked()));
    QHBoxLayout *p_addBtnsHorLayout = new QHBoxLayout;
    p_addBtnsHorLayout->addLayout(p_addBtnsVertLayout);
    p_addBtnsHorLayout->addWidget(p_applyBtn);
    p_addBtnsHorLayout->addStretch(1);
    setLayout(p_addBtnsHorLayout);
    setFixedSize(sizeHint());
}

void AddBtnsSettingsDialog::slot_applyBtnClicked()
{
    bool altCodesSelect = pm_altCodesChBox->isChecked();
    bool excelSelect = pm_excelChBox->isChecked();
    ExtraBtnsType type;
    if (!altCodesSelect && !excelSelect)
    {
        type = None;
    }
    else if (altCodesSelect && !excelSelect)
    {
        type = AltCodes;
    }
    else if (!altCodesSelect && excelSelect)
    {
        type = Excel;
    }
    else if (altCodesSelect && excelSelect)
    {
        type = AltCodes_Excel;
    }
    pm_numpadManager->createExtraBtns(type);
}

void AddBtnsSettingsDialog::showEvent(QShowEvent *)
{
    ExtraBtnsType type = pm_numpad->getExtraBtnsType();
    switch (type) {
    case None:
        pm_altCodesChBox->setChecked(false);
        pm_excelChBox->setChecked(false);
        break;
    case AltCodes:
        pm_altCodesChBox->setChecked(true);
        pm_excelChBox->setChecked(false);
        break;
    case Excel:
        pm_altCodesChBox->setChecked(false);
        pm_excelChBox->setChecked(true);
        break;
    case AltCodes_Excel:
        pm_altCodesChBox->setChecked(true);
        pm_excelChBox->setChecked(true);
        break;
    }
}


