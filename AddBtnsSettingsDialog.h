#ifndef ADDBTNSSETTINGSDIALOG_H
#define ADDBTNSSETTINGSDIALOG_H

#include <QDialog>

class Numpad;
class QCheckBox;
class NumpadManager;

class AddBtnsSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    AddBtnsSettingsDialog(NumpadManager *, Numpad *, QWidget *p_wid = 0);
    void setNumpad(Numpad *p_numpad) { pm_numpad = p_numpad; }
protected:
    void showEvent(QShowEvent *);
protected slots:
    void slot_applyBtnClicked();
private:
    Numpad *pm_numpad;
    QCheckBox *pm_altCodesChBox;
    QCheckBox *pm_excelChBox;
    NumpadManager *pm_numpadManager;
};

#endif // ADDBTNSSETTINGSDIALOG_H
