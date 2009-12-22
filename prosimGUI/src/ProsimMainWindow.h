#include "ui_prosim.h"
#include <iostream>

class ProsimMainWindow : public QMainWindow
{
     Q_OBJECT

 public:
     ProsimMainWindow(QMainWindow *parent = 0);

 private slots:
	
	// Events
	void on_algorithmModes_activated(int value);
	void on_quantumSpinBox_valueChanged(int value);
	void on_expirySpinBox_valueChanged(int value);
	void on_simulationModes_activated(int value);
	void on_inputFileDialogue_clicked(void);
	void on_inputFileTextBox_returnPressed(void);
	void on_inputFileTextBox_textChanged(void);
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
        int getFinishTime(void);
        int getNumProcs(void);

};
