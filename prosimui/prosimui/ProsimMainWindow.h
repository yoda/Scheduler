#include "ui_prosim.h"
#include <iostream>

class ProsimMainWindow : public QMainWindow
{
     Q_OBJECT

 public:
     ProsimMainWindow(QMainWindow *parent = 0);

 private slots:
	/*
	 QObject::connect(algorithmModes, SIGNAL(activated(int)), Prosim, SLOT(update()));
	 QObject::connect(quantumSpinBox, SIGNAL(valueChanged(int)), Prosim, SLOT(update()));
	 QObject::connect(expirySpinBox, SIGNAL(valueChanged(int)), Prosim, SLOT(update()));
	 QObject::connect(simulationModes, SIGNAL(activated(int)), Prosim, SLOT(update()));
	 QObject::connect(inputFileDialogue, SIGNAL(clicked()), Prosim, SLOT(update()));
	 QObject::connect(inputFileTextBox, SIGNAL(returnPressed()), Prosim, SLOT(update()));
	 QObject::connect(outputFileDialogue, SIGNAL(clicked()), Prosim, SLOT(update()));
	 QObject::connect(outputFileTextBox, SIGNAL(returnPressed()), Prosim, SLOT(update()));
	 QObject::connect(resetButton, SIGNAL(clicked()), Prosim, SLOT(update()));	 
	 */
	
	// Events
	void on_algorithmModes_activated(int value);
	void on_quantumSpinBox_valueChanged(int value);
	void on_expirySpinBox_valueChanged(int value);
	void on_simulationModes_activated(int value);
	void on_inputFileDialogue_clicked(void);
	void on_inputFileTextBox_returnPressed(void);
	void on_inputFileTextBox_textChanged(void);
	void on_outputFileDialogue_clicked(void);
	void on_outputFileTextBox_returnPressed(void);
	void on_outputFileTextBox_textChanged(void);
	void on_runButton_clicked(void);
	void on_resetButton_clicked(void);
	
 private:
	Ui_Prosim ui;
	QGraphicsScene memoryScene;
	QGraphicsScene processScene;
        QList<QGraphicsItem*> itemList;
	void drawGrid();
	void drawSequenceGrid(int, int, int);
        void drawSequence(int,int,int,int,int);
};
