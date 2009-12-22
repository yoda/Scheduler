/********************************************************************************
** Form generated from reading ui file 'prosimOe4153.ui'
**
** Created: Wed Sep 9 22:15:56 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#include "Prosim.h"


void Ui_Prosim::test() {
		printf("Testing button\n");
	}

void Ui_Prosim::setupUi(QMainWindow *Prosim)
    {
        if (Prosim->objectName().isEmpty())
            Prosim->setObjectName(QString::fromUtf8("Prosim"));
        Prosim->resize(720, 540);
        Prosim->setMinimumSize(QSize(720, 540));
        Prosim->setMaximumSize(QSize(720, 540));
        centralwidget = new QWidget(Prosim);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 701, 301));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridFrame = new QFrame(frame);
        gridFrame->setObjectName(QString::fromUtf8("gridFrame"));
        gridFrame->setGeometry(QRect(10, 10, 681, 281));
        gridLayout_2 = new QGridLayout(gridFrame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        processGraph = new QGraphicsView(gridFrame);
        processGraph->setObjectName(QString::fromUtf8("processGraph"));

        gridLayout_2->addWidget(processGraph, 0, 0, 1, 1);

        memoryGraph = new QGraphicsView(gridFrame);
        memoryGraph->setObjectName(QString::fromUtf8("memoryGraph"));

        gridLayout_2->addWidget(memoryGraph, 1, 0, 1, 1);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 400, 701, 111));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridFrame1 = new QFrame(frame_2);
        gridFrame1->setObjectName(QString::fromUtf8("gridFrame1"));
        gridFrame1->setGeometry(QRect(110, 0, 161, 111));
        gridLayout_3 = new QGridLayout(gridFrame1);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        expirySpinBox = new QSpinBox(gridFrame1);
        expirySpinBox->setObjectName(QString::fromUtf8("expirySpinBox"));

        gridLayout_3->addWidget(expirySpinBox, 3, 0, 1, 1);

        algorithmModes = new QComboBox(gridFrame1);
        algorithmModes->setObjectName(QString::fromUtf8("algorithmModes"));

        gridLayout_3->addWidget(algorithmModes, 1, 0, 1, 1);

        quantumSpinBox = new QSpinBox(gridFrame1);
        quantumSpinBox->setObjectName(QString::fromUtf8("quantumSpinBox"));

        gridLayout_3->addWidget(quantumSpinBox, 2, 0, 1, 1);

        gridFrame2 = new QFrame(frame_2);
        gridFrame2->setObjectName(QString::fromUtf8("gridFrame2"));
        gridFrame2->setGeometry(QRect(10, 0, 101, 111));
        gridLayout_4 = new QGridLayout(gridFrame2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        algorithmLabel = new QLabel(gridFrame2);
        algorithmLabel->setObjectName(QString::fromUtf8("algorithmLabel"));

        gridLayout_4->addWidget(algorithmLabel, 0, 0, 1, 1);

        expiryLabel = new QLabel(gridFrame2);
        expiryLabel->setObjectName(QString::fromUtf8("expiryLabel"));

        gridLayout_4->addWidget(expiryLabel, 2, 0, 1, 1);

        quantumLabel = new QLabel(gridFrame2);
        quantumLabel->setObjectName(QString::fromUtf8("quantumLabel"));

        gridLayout_4->addWidget(quantumLabel, 1, 0, 1, 1);

        gridFrame3 = new QFrame(frame_2);
        gridFrame3->setObjectName(QString::fromUtf8("gridFrame3"));
        gridFrame3->setGeometry(QRect(500, -1, 191, 111));
        gridLayout_5 = new QGridLayout(gridFrame3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        outputFileTextBox = new QLineEdit(gridFrame3);
        outputFileTextBox->setObjectName(QString::fromUtf8("outputFileTextBox"));

        gridLayout_5->addWidget(outputFileTextBox, 3, 0, 1, 1);

        simulationModes = new QComboBox(gridFrame3);
        simulationModes->setObjectName(QString::fromUtf8("simulationModes"));

        gridLayout_5->addWidget(simulationModes, 1, 0, 1, 1);

        inputFileTextBox = new QLineEdit(gridFrame3);
        inputFileTextBox->setObjectName(QString::fromUtf8("inputFileTextBox"));

        gridLayout_5->addWidget(inputFileTextBox, 2, 0, 1, 1);

        inputFileDialogue = new QToolButton(gridFrame3);
        inputFileDialogue->setObjectName(QString::fromUtf8("inputFileDialogue"));

        gridLayout_5->addWidget(inputFileDialogue, 2, 1, 1, 1);

        outputFileDialogue = new QToolButton(gridFrame3);
        outputFileDialogue->setObjectName(QString::fromUtf8("outputFileDialogue"));

        gridLayout_5->addWidget(outputFileDialogue, 3, 1, 1, 1);

        gridFrame4 = new QFrame(frame_2);
        gridFrame4->setObjectName(QString::fromUtf8("gridFrame4"));
        gridFrame4->setGeometry(QRect(360, 0, 141, 111));
        gridLayout_6 = new QGridLayout(gridFrame4);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        simulationModeLabel = new QLabel(gridFrame4);
        simulationModeLabel->setObjectName(QString::fromUtf8("simulationModeLabel"));

        gridLayout_6->addWidget(simulationModeLabel, 0, 0, 1, 1);

        outputFileLabel = new QLabel(gridFrame4);
        outputFileLabel->setObjectName(QString::fromUtf8("outputFileLabel"));

        gridLayout_6->addWidget(outputFileLabel, 2, 0, 1, 1);

        inputFileLabel = new QLabel(gridFrame4);
        inputFileLabel->setObjectName(QString::fromUtf8("inputFileLabel"));

        gridLayout_6->addWidget(inputFileLabel, 1, 0, 1, 1);

        horizontalFrame = new QFrame(centralwidget);
        horizontalFrame->setObjectName(QString::fromUtf8("horizontalFrame"));
        horizontalFrame->setGeometry(QRect(170, 330, 401, 51));
        horizontalLayout = new QHBoxLayout(horizontalFrame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        runButton = new QPushButton(horizontalFrame);
        runButton->setObjectName(QString::fromUtf8("runButton"));

        horizontalLayout->addWidget(runButton);

        resetButton = new QPushButton(horizontalFrame);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        horizontalLayout->addWidget(resetButton);

        Prosim->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Prosim);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Prosim->setStatusBar(statusbar);

        retranslateUi(Prosim);
        QObject::connect(algorithmModes, SIGNAL(activated(int)), Prosim, SLOT(update()));
        QObject::connect(quantumSpinBox, SIGNAL(valueChanged(int)), Prosim, SLOT(update()));
        QObject::connect(expirySpinBox, SIGNAL(valueChanged(int)), Prosim, SLOT(update()));
        QObject::connect(simulationModes, SIGNAL(activated(int)), Prosim, SLOT(update()));
        QObject::connect(inputFileDialogue, SIGNAL(clicked()), Prosim, SLOT(update()));
        QObject::connect(inputFileTextBox, SIGNAL(returnPressed()), Prosim, SLOT(update()));
        QObject::connect(outputFileDialogue, SIGNAL(clicked()), Prosim, SLOT(update()));
        QObject::connect(outputFileTextBox, SIGNAL(returnPressed()), Prosim, SLOT(update()));
        QObject::connect(runButton, SIGNAL(clicked()), Prosim, SLOT(Ui_Prosim::test()));
        QObject::connect(resetButton, SIGNAL(clicked()), Prosim, SLOT(update()));

        QMetaObject::connectSlotsByName(Prosim);
    } // setupUi

void Ui_Prosim::retranslateUi(QMainWindow *Prosim)
    {
        Prosim->setWindowTitle(QApplication::translate("Prosim", "Prosim", 0, QApplication::UnicodeUTF8));
        algorithmModes->clear();
        algorithmModes->insertItems(0, QStringList()
         << QApplication::translate("Prosim", "FCFS", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Prosim", "RR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Prosim", "SRT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Prosim", "SPN", 0, QApplication::UnicodeUTF8)
        );
        algorithmLabel->setText(QApplication::translate("Prosim", "Algorithm", 0, QApplication::UnicodeUTF8));
        expiryLabel->setText(QApplication::translate("Prosim", "Expiry", 0, QApplication::UnicodeUTF8));
        quantumLabel->setText(QApplication::translate("Prosim", "Quantum", 0, QApplication::UnicodeUTF8));
        simulationModes->clear();
        simulationModes->insertItems(0, QStringList()
         << QApplication::translate("Prosim", "P", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Prosim", "VM + P", 0, QApplication::UnicodeUTF8)
        );
        inputFileDialogue->setText(QApplication::translate("Prosim", "...", 0, QApplication::UnicodeUTF8));
        outputFileDialogue->setText(QApplication::translate("Prosim", "...", 0, QApplication::UnicodeUTF8));
        simulationModeLabel->setText(QApplication::translate("Prosim", "Simulation Mode", 0, QApplication::UnicodeUTF8));
        outputFileLabel->setText(QApplication::translate("Prosim", "Output File", 0, QApplication::UnicodeUTF8));
        inputFileLabel->setText(QApplication::translate("Prosim", "Input File", 0, QApplication::UnicodeUTF8));
        runButton->setText(QApplication::translate("Prosim", "Run", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("Prosim", "Reset", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Prosim);
    } // retranslateUi
    