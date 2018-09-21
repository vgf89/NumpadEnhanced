#ifndef FIRSTRUNDIALOG_H
#define FIRSTRUNDIALOG_H

#include <QDialog>


class FirstRunDialog : public QDialog
{
    Q_OBJECT
public:
    FirstRunDialog(QWidget *p_wid = 0);
signals:
    void okClicked();
    void openSetDlgClicked();
};

#endif // FIRSTRUNDIALOG_H
