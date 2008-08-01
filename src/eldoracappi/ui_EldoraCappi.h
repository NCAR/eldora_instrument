/********************************************************************************
** Form generated from reading ui file 'EldoraCappi.ui'
**
** Created: Fri Jul 25 07:11:23 2008
**      by: Qt User Interface Compiler version 4.2.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ELDORACAPPI_H
#define UI_ELDORACAPPI_H

#include "ColorBar.h"
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

class Ui_EldoraCappi
{
public:
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout1;
    QFrame *CAPPI_Parent;
    ColorBar *colorBarFor;
    QVBoxLayout *vboxLayout1;
    QGroupBox *groupBox_2;
    QVBoxLayout *vboxLayout2;
    QVBoxLayout *vboxLayout3;
    QPushButton *pauseRunButton;
    QPushButton *ringColorButton;
    QPushButton *backgroundButton;
    QPushButton *saveImage;
    QHBoxLayout *hboxLayout2;
    QCheckBox *ringsCheck;
    QCheckBox *gridCheck;
    QSpacerItem *spacerItem;
    QGroupBox *groupBox_4;
    QHBoxLayout *hboxLayout3;
    QRadioButton *panButton;
    QRadioButton *zoomButton;
    QGroupBox *groupBox_3;
    QVBoxLayout *vboxLayout4;
    QVBoxLayout *vboxLayout5;
    QVBoxLayout *vboxLayout6;
    QPushButton *zoomIn;
    QPushButton *zoomOut;
    QHBoxLayout *hboxLayout4;
    QHBoxLayout *hboxLayout5;
    QGridLayout *gridLayout;
    QPushButton *panDown;
    QPushButton *viewReset;
    QPushButton *panLeft;
    QPushButton *panRight;
    QPushButton *panUp;
    QSpacerItem *spacerItem1;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout7;
    QHBoxLayout *hboxLayout6;
    QLabel *label_4;
    QLabel *gatesText;
    QHBoxLayout *hboxLayout7;
    QLabel *label_3;
    QLabel *dwellWidthText;
    QHBoxLayout *hboxLayout8;
    QLabel *label_2;
    QLabel *beamsText;
    QHBoxLayout *hboxLayout9;
    QLabel *label_5;
    QLabel *rollText;
    QHBoxLayout *hboxLayout10;
    QLabel *label;
    QLabel *angleText;

    void setupUi(QDialog *EldoraCappi)
    {
    EldoraCappi->setObjectName(QString::fromUtf8("EldoraCappi"));
    hboxLayout = new QHBoxLayout(EldoraCappi);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(9);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    CAPPI_Parent = new QFrame(EldoraCappi);
    CAPPI_Parent->setObjectName(QString::fromUtf8("CAPPI_Parent"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(CAPPI_Parent->sizePolicy().hasHeightForWidth());
    CAPPI_Parent->setSizePolicy(sizePolicy);
    CAPPI_Parent->setMinimumSize(QSize(0, 0));
    CAPPI_Parent->setFrameShape(QFrame::StyledPanel);
    CAPPI_Parent->setFrameShadow(QFrame::Raised);

    hboxLayout1->addWidget(CAPPI_Parent);

    colorBarFor = new ColorBar(EldoraCappi);
    colorBarFor->setObjectName(QString::fromUtf8("colorBarFor"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(13), static_cast<QSizePolicy::Policy>(13));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(colorBarFor->sizePolicy().hasHeightForWidth());
    colorBarFor->setSizePolicy(sizePolicy1);
    colorBarFor->setMinimumSize(QSize(40, 500));

    hboxLayout1->addWidget(colorBarFor);


    vboxLayout->addLayout(hboxLayout1);


    hboxLayout->addLayout(vboxLayout);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    groupBox_2 = new QGroupBox(EldoraCappi);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    vboxLayout2 = new QVBoxLayout(groupBox_2);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(9);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout3 = new QVBoxLayout();
    vboxLayout3->setSpacing(6);
    vboxLayout3->setMargin(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    pauseRunButton = new QPushButton(groupBox_2);
    pauseRunButton->setObjectName(QString::fromUtf8("pauseRunButton"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(pauseRunButton->sizePolicy().hasHeightForWidth());
    pauseRunButton->setSizePolicy(sizePolicy2);
    pauseRunButton->setMinimumSize(QSize(1, 0));
    pauseRunButton->setCheckable(true);
    pauseRunButton->setChecked(false);

    vboxLayout3->addWidget(pauseRunButton);

    ringColorButton = new QPushButton(groupBox_2);
    ringColorButton->setObjectName(QString::fromUtf8("ringColorButton"));
    ringColorButton->setMinimumSize(QSize(1, 0));

    vboxLayout3->addWidget(ringColorButton);

    backgroundButton = new QPushButton(groupBox_2);
    backgroundButton->setObjectName(QString::fromUtf8("backgroundButton"));
    backgroundButton->setMinimumSize(QSize(1, 0));

    vboxLayout3->addWidget(backgroundButton);

    saveImage = new QPushButton(groupBox_2);
    saveImage->setObjectName(QString::fromUtf8("saveImage"));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(saveImage->sizePolicy().hasHeightForWidth());
    saveImage->setSizePolicy(sizePolicy3);
    saveImage->setMinimumSize(QSize(1, 0));

    vboxLayout3->addWidget(saveImage);


    vboxLayout2->addLayout(vboxLayout3);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    ringsCheck = new QCheckBox(groupBox_2);
    ringsCheck->setObjectName(QString::fromUtf8("ringsCheck"));
    ringsCheck->setMinimumSize(QSize(1, 0));
    ringsCheck->setChecked(true);

    hboxLayout2->addWidget(ringsCheck);

    gridCheck = new QCheckBox(groupBox_2);
    gridCheck->setObjectName(QString::fromUtf8("gridCheck"));
    gridCheck->setMinimumSize(QSize(1, 0));

    hboxLayout2->addWidget(gridCheck);


    vboxLayout2->addLayout(hboxLayout2);


    vboxLayout1->addWidget(groupBox_2);

    spacerItem = new QSpacerItem(144, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem);

    groupBox_4 = new QGroupBox(EldoraCappi);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    hboxLayout3 = new QHBoxLayout(groupBox_4);
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(9);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    panButton = new QRadioButton(groupBox_4);
    panButton->setObjectName(QString::fromUtf8("panButton"));

    hboxLayout3->addWidget(panButton);

    zoomButton = new QRadioButton(groupBox_4);
    zoomButton->setObjectName(QString::fromUtf8("zoomButton"));

    hboxLayout3->addWidget(zoomButton);


    vboxLayout1->addWidget(groupBox_4);

    groupBox_3 = new QGroupBox(EldoraCappi);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    vboxLayout4 = new QVBoxLayout(groupBox_3);
    vboxLayout4->setSpacing(6);
    vboxLayout4->setMargin(9);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    vboxLayout5 = new QVBoxLayout();
    vboxLayout5->setSpacing(6);
    vboxLayout5->setMargin(0);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    vboxLayout6 = new QVBoxLayout();
    vboxLayout6->setSpacing(6);
    vboxLayout6->setMargin(0);
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    zoomIn = new QPushButton(groupBox_3);
    zoomIn->setObjectName(QString::fromUtf8("zoomIn"));
    QSizePolicy sizePolicy4(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(zoomIn->sizePolicy().hasHeightForWidth());
    zoomIn->setSizePolicy(sizePolicy4);
    zoomIn->setMinimumSize(QSize(1, 0));

    vboxLayout6->addWidget(zoomIn);

    zoomOut = new QPushButton(groupBox_3);
    zoomOut->setObjectName(QString::fromUtf8("zoomOut"));
    QSizePolicy sizePolicy5(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
    sizePolicy5.setHorizontalStretch(0);
    sizePolicy5.setVerticalStretch(0);
    sizePolicy5.setHeightForWidth(zoomOut->sizePolicy().hasHeightForWidth());
    zoomOut->setSizePolicy(sizePolicy5);
    zoomOut->setMinimumSize(QSize(1, 0));

    vboxLayout6->addWidget(zoomOut);


    vboxLayout5->addLayout(vboxLayout6);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setSpacing(6);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));

    vboxLayout5->addLayout(hboxLayout4);


    vboxLayout4->addLayout(vboxLayout5);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setSpacing(6);
    hboxLayout5->setMargin(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(6);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    panDown = new QPushButton(groupBox_3);
    panDown->setObjectName(QString::fromUtf8("panDown"));
    QSizePolicy sizePolicy6(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy6.setHorizontalStretch(0);
    sizePolicy6.setVerticalStretch(0);
    sizePolicy6.setHeightForWidth(panDown->sizePolicy().hasHeightForWidth());
    panDown->setSizePolicy(sizePolicy6);
    panDown->setMinimumSize(QSize(30, 30));
    panDown->setMaximumSize(QSize(30, 30));

    gridLayout->addWidget(panDown, 2, 1, 1, 1);

    viewReset = new QPushButton(groupBox_3);
    viewReset->setObjectName(QString::fromUtf8("viewReset"));
    QSizePolicy sizePolicy7(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy7.setHorizontalStretch(0);
    sizePolicy7.setVerticalStretch(0);
    sizePolicy7.setHeightForWidth(viewReset->sizePolicy().hasHeightForWidth());
    viewReset->setSizePolicy(sizePolicy7);
    viewReset->setMinimumSize(QSize(30, 30));
    viewReset->setMaximumSize(QSize(30, 30));

    gridLayout->addWidget(viewReset, 1, 1, 1, 1);

    panLeft = new QPushButton(groupBox_3);
    panLeft->setObjectName(QString::fromUtf8("panLeft"));
    QSizePolicy sizePolicy8(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy8.setHorizontalStretch(0);
    sizePolicy8.setVerticalStretch(0);
    sizePolicy8.setHeightForWidth(panLeft->sizePolicy().hasHeightForWidth());
    panLeft->setSizePolicy(sizePolicy8);
    panLeft->setMinimumSize(QSize(30, 30));
    panLeft->setMaximumSize(QSize(30, 30));

    gridLayout->addWidget(panLeft, 1, 0, 1, 1);

    panRight = new QPushButton(groupBox_3);
    panRight->setObjectName(QString::fromUtf8("panRight"));
    QSizePolicy sizePolicy9(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy9.setHorizontalStretch(0);
    sizePolicy9.setVerticalStretch(0);
    sizePolicy9.setHeightForWidth(panRight->sizePolicy().hasHeightForWidth());
    panRight->setSizePolicy(sizePolicy9);
    panRight->setMinimumSize(QSize(30, 30));
    panRight->setMaximumSize(QSize(30, 30));

    gridLayout->addWidget(panRight, 1, 2, 1, 1);

    panUp = new QPushButton(groupBox_3);
    panUp->setObjectName(QString::fromUtf8("panUp"));
    QSizePolicy sizePolicy10(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy10.setHorizontalStretch(0);
    sizePolicy10.setVerticalStretch(0);
    sizePolicy10.setHeightForWidth(panUp->sizePolicy().hasHeightForWidth());
    panUp->setSizePolicy(sizePolicy10);
    panUp->setMinimumSize(QSize(30, 30));
    panUp->setMaximumSize(QSize(30, 30));

    gridLayout->addWidget(panUp, 0, 1, 1, 1);


    hboxLayout5->addLayout(gridLayout);


    vboxLayout4->addLayout(hboxLayout5);


    vboxLayout1->addWidget(groupBox_3);

    spacerItem1 = new QSpacerItem(144, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem1);

    groupBox = new QGroupBox(EldoraCappi);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout7 = new QVBoxLayout(groupBox);
    vboxLayout7->setSpacing(6);
    vboxLayout7->setMargin(9);
    vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
    hboxLayout6 = new QHBoxLayout();
    hboxLayout6->setSpacing(6);
    hboxLayout6->setMargin(0);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    hboxLayout6->addWidget(label_4);

    gatesText = new QLabel(groupBox);
    gatesText->setObjectName(QString::fromUtf8("gatesText"));
    gatesText->setTextFormat(Qt::PlainText);
    gatesText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout6->addWidget(gatesText);


    vboxLayout7->addLayout(hboxLayout6);

    hboxLayout7 = new QHBoxLayout();
    hboxLayout7->setSpacing(6);
    hboxLayout7->setMargin(0);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    hboxLayout7->addWidget(label_3);

    dwellWidthText = new QLabel(groupBox);
    dwellWidthText->setObjectName(QString::fromUtf8("dwellWidthText"));
    dwellWidthText->setTextFormat(Qt::PlainText);
    dwellWidthText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout7->addWidget(dwellWidthText);


    vboxLayout7->addLayout(hboxLayout7);

    hboxLayout8 = new QHBoxLayout();
    hboxLayout8->setSpacing(6);
    hboxLayout8->setMargin(0);
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    hboxLayout8->addWidget(label_2);

    beamsText = new QLabel(groupBox);
    beamsText->setObjectName(QString::fromUtf8("beamsText"));
    beamsText->setTextFormat(Qt::PlainText);
    beamsText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout8->addWidget(beamsText);


    vboxLayout7->addLayout(hboxLayout8);

    hboxLayout9 = new QHBoxLayout();
    hboxLayout9->setSpacing(6);
    hboxLayout9->setMargin(0);
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    hboxLayout9->addWidget(label_5);

    rollText = new QLabel(groupBox);
    rollText->setObjectName(QString::fromUtf8("rollText"));
    rollText->setTextFormat(Qt::PlainText);
    rollText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout9->addWidget(rollText);


    vboxLayout7->addLayout(hboxLayout9);

    hboxLayout10 = new QHBoxLayout();
    hboxLayout10->setSpacing(6);
    hboxLayout10->setMargin(0);
    hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout10->addWidget(label);

    angleText = new QLabel(groupBox);
    angleText->setObjectName(QString::fromUtf8("angleText"));
    angleText->setTextFormat(Qt::PlainText);
    angleText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout10->addWidget(angleText);


    vboxLayout7->addLayout(hboxLayout10);


    vboxLayout1->addWidget(groupBox);


    hboxLayout->addLayout(vboxLayout1);


    retranslateUi(EldoraCappi);

    QSize size(837, 677);
    size = size.expandedTo(EldoraCappi->minimumSizeHint());
    EldoraCappi->resize(size);


    QMetaObject::connectSlotsByName(EldoraCappi);
    } // setupUi

    void retranslateUi(QDialog *EldoraCappi)
    {
    EldoraCappi->setWindowTitle(QApplication::translate("EldoraCappi", "Dialog", 0, QApplication::UnicodeUTF8));
    colorBarFor->setToolTip(QApplication::translate("EldoraCappi", "Click to set scale", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QString());
    pauseRunButton->setToolTip(QApplication::translate("EldoraCappi", "Pause real-time data display", 0, QApplication::UnicodeUTF8));
    pauseRunButton->setText(QApplication::translate("EldoraCappi", "Pause", 0, QApplication::UnicodeUTF8));
    ringColorButton->setToolTip(QApplication::translate("EldoraCappi", "Select the color for rings and grid", 0, QApplication::UnicodeUTF8));
    ringColorButton->setText(QApplication::translate("EldoraCappi", "Ring/grid color", 0, QApplication::UnicodeUTF8));
    backgroundButton->setToolTip(QApplication::translate("EldoraCappi", "Select the background color", 0, QApplication::UnicodeUTF8));
    backgroundButton->setText(QApplication::translate("EldoraCappi", "Background color", 0, QApplication::UnicodeUTF8));
    saveImage->setToolTip(QApplication::translate("EldoraCappi", "Save the display to a file", 0, QApplication::UnicodeUTF8));
    saveImage->setText(QApplication::translate("EldoraCappi", "Save Image", 0, QApplication::UnicodeUTF8));
    ringsCheck->setToolTip(QApplication::translate("EldoraCappi", "Enable rings overlay", 0, QApplication::UnicodeUTF8));
    ringsCheck->setText(QApplication::translate("EldoraCappi", "Rings", 0, QApplication::UnicodeUTF8));
    gridCheck->setToolTip(QApplication::translate("EldoraCappi", "Enable Grid overlay", 0, QApplication::UnicodeUTF8));
    gridCheck->setText(QApplication::translate("EldoraCappi", "Grid", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QString());
    panButton->setToolTip(QApplication::translate("EldoraCappi", "Mouse will be used for panning", 0, QApplication::UnicodeUTF8));
    panButton->setText(QString());
    zoomButton->setToolTip(QApplication::translate("EldoraCappi", "Mouse will be used for zooming", 0, QApplication::UnicodeUTF8));
    zoomButton->setText(QString());
    groupBox_3->setTitle(QString());
    zoomIn->setToolTip(QApplication::translate("EldoraCappi", "Zoom in both displays", 0, QApplication::UnicodeUTF8));
    zoomIn->setText(QApplication::translate("EldoraCappi", "Zoom in", 0, QApplication::UnicodeUTF8));
    zoomOut->setToolTip(QApplication::translate("EldoraCappi", "Zoom out for displays", 0, QApplication::UnicodeUTF8));
    zoomOut->setText(QApplication::translate("EldoraCappi", "Zoom out", 0, QApplication::UnicodeUTF8));
    panDown->setToolTip(QApplication::translate("EldoraCappi", "Pan both displays down", 0, QApplication::UnicodeUTF8));
    panDown->setText(QApplication::translate("EldoraCappi", "v", 0, QApplication::UnicodeUTF8));
    viewReset->setToolTip(QApplication::translate("EldoraCappi", "Reset zoom and pan on both displays", 0, QApplication::UnicodeUTF8));
    viewReset->setText(QApplication::translate("EldoraCappi", "o", 0, QApplication::UnicodeUTF8));
    panLeft->setToolTip(QApplication::translate("EldoraCappi", "Pan both dsplays left", 0, QApplication::UnicodeUTF8));
    panLeft->setText(QApplication::translate("EldoraCappi", "<", 0, QApplication::UnicodeUTF8));
    panRight->setToolTip(QApplication::translate("EldoraCappi", "Pan both displays right", 0, QApplication::UnicodeUTF8));
    panRight->setText(QApplication::translate("EldoraCappi", ">", 0, QApplication::UnicodeUTF8));
    panUp->setToolTip(QApplication::translate("EldoraCappi", "Pan both displays up", 0, QApplication::UnicodeUTF8));
    panUp->setText(QApplication::translate("EldoraCappi", "^", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("EldoraCappi", "Vitals", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("EldoraCappi", "Gates", 0, QApplication::UnicodeUTF8));
    gatesText->setToolTip(QApplication::translate("EldoraCappi", "Number of gates", 0, QApplication::UnicodeUTF8));
    gatesText->setText(QApplication::translate("EldoraCappi", "0.0", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("EldoraCappi", "Width", 0, QApplication::UnicodeUTF8));
    dwellWidthText->setToolTip(QApplication::translate("EldoraCappi", "Angular width of one beam (deg)", 0, QApplication::UnicodeUTF8));
    dwellWidthText->setText(QApplication::translate("EldoraCappi", "0.0", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("EldoraCappi", "Beams", 0, QApplication::UnicodeUTF8));
    beamsText->setToolTip(QApplication::translate("EldoraCappi", "Number of allocated beams", 0, QApplication::UnicodeUTF8));
    beamsText->setText(QApplication::translate("EldoraCappi", "0.0", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("EldoraCappi", "Roll", 0, QApplication::UnicodeUTF8));
    rollText->setToolTip(QApplication::translate("EldoraCappi", "Aircraft roll (deg)", 0, QApplication::UnicodeUTF8));
    rollText->setText(QApplication::translate("EldoraCappi", "0.0", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("EldoraCappi", "Angle", 0, QApplication::UnicodeUTF8));
    angleText->setToolTip(QApplication::translate("EldoraCappi", "Current pointing angle (deg)", 0, QApplication::UnicodeUTF8));
    angleText->setText(QApplication::translate("EldoraCappi", "0.0", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(EldoraCappi);
    } // retranslateUi

};

namespace Ui {
    class EldoraCappi: public Ui_EldoraCappi {};
} // namespace Ui

#endif // UI_ELDORACAPPI_H
