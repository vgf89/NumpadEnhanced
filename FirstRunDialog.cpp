#include "FirstRunDialog.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

FirstRunDialog::FirstRunDialog(QWidget *p_wid)
    : QDialog(p_wid, Qt::FramelessWindowHint)
{
    QLabel *p_lbl = new QLabel("You can select additional buttons in the settings.<br>"
                             "(Right-click on the system tray icon -> Settings -> Additional buttons...)");
    QPushButton *p_okBtn = new QPushButton("Ok");
    QPushButton *p_openSettingsBtn = new QPushButton("Open Settings...");
    QHBoxLayout *p_hLayout = new QHBoxLayout;
    p_hLayout->addStretch(1);
    p_hLayout->addWidget(p_okBtn);
    p_hLayout->addWidget(p_openSettingsBtn);
    p_hLayout->addStretch(1);
    QVBoxLayout *p_vLayout = new QVBoxLayout;
    p_vLayout->addWidget(p_lbl);
    p_vLayout->addStretch(1);
    p_vLayout->addLayout(p_hLayout);
    setLayout(p_vLayout);
    connect(p_okBtn, SIGNAL(clicked()), this, SIGNAL(okClicked()));
    connect(p_openSettingsBtn, SIGNAL(clicked()), this, SIGNAL(openSetDlgClicked()));

}
