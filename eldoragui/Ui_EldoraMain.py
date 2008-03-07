# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'EldoraMain.ui'
#
# Created: Fri Mar  7 05:48:34 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_EldoraMain(object):
    def setupUi(self, EldoraMain):
        EldoraMain.setObjectName("EldoraMain")
        EldoraMain.resize(QtCore.QSize(QtCore.QRect(0,0,240,96).size()).expandedTo(EldoraMain.minimumSizeHint()))

        self.centralwidget = QtGui.QWidget(EldoraMain)
        self.centralwidget.setObjectName("centralwidget")

        self.hboxlayout = QtGui.QHBoxLayout(self.centralwidget)
        self.hboxlayout.setMargin(9)
        self.hboxlayout.setSpacing(6)
        self.hboxlayout.setObjectName("hboxlayout")

        self.runButton = QtGui.QPushButton(self.centralwidget)
        self.runButton.setCheckable(True)
        self.runButton.setObjectName("runButton")
        self.hboxlayout.addWidget(self.runButton)

        self.dateTimeLabel = QtGui.QLabel(self.centralwidget)
        self.dateTimeLabel.setObjectName("dateTimeLabel")
        self.hboxlayout.addWidget(self.dateTimeLabel)
        EldoraMain.setCentralWidget(self.centralwidget)

        self.menubar = QtGui.QMenuBar(EldoraMain)
        self.menubar.setGeometry(QtCore.QRect(0,0,240,29))
        self.menubar.setObjectName("menubar")
        EldoraMain.setMenuBar(self.menubar)

        self.statusbar = QtGui.QStatusBar(EldoraMain)
        self.statusbar.setObjectName("statusbar")
        EldoraMain.setStatusBar(self.statusbar)

        self.retranslateUi(EldoraMain)
        QtCore.QMetaObject.connectSlotsByName(EldoraMain)

    def retranslateUi(self, EldoraMain):
        EldoraMain.setWindowTitle(QtGui.QApplication.translate("EldoraMain", "Eldora Control", None, QtGui.QApplication.UnicodeUTF8))
        self.runButton.setText(QtGui.QApplication.translate("EldoraMain", "Run", None, QtGui.QApplication.UnicodeUTF8))
        self.dateTimeLabel.setText(QtGui.QApplication.translate("EldoraMain", "Date and Time Go Here", None, QtGui.QApplication.UnicodeUTF8))

