/*
 *  ProsimMainWindow.cpp
 *  prosim
 *
 *  Created by yoda on 11/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "ProsimMainWindow.h"
#include <QtDebug>
#include <QGraphicsItem>
#include <QList>
#include <qgraphicsview.h>
#include <QAbstractScrollArea>
#include <QGraphicsRectItem>

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

#define DEBUG

ProsimMainWindow::ProsimMainWindow(QMainWindow *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.memoryGraph->setScene(&(this->memoryScene));
	ui.processGraph->setScene(&(this->processScene));
	
}

void ProsimMainWindow::on_algorithmModes_activated(int value)
{
	switch (value) {
		case 0:
#ifdef DEBUG
			qDebug() << ui.algorithmModes->currentText();
#endif
			break;
		case 1:
#ifdef DEBUG
			qDebug() << ui.algorithmModes->currentText();
#endif
			break;
		case 2:
#ifdef DEBUG
			qDebug() << ui.algorithmModes->currentText();
#endif
			break;
		case 3:
#ifdef DEBUG
			qDebug() << ui.algorithmModes->currentText();
#endif
			break;
		default:
			break;
	}

}

void ProsimMainWindow::on_quantumSpinBox_valueChanged(int value)
{
#ifdef DEBUG
	qDebug() << "Quantum value: " << value;
#endif
}

void ProsimMainWindow::on_expirySpinBox_valueChanged(int value)
{
#ifdef DEBUG
	qDebug() << "Expiry value: " << value;
#endif
}

void ProsimMainWindow::on_simulationModes_activated(int value)
{
	switch (value) {
		case 0:
#ifdef DEBUG
			qDebug() << ui.simulationModes->currentText();
#endif
			break;
		case 1:
#ifdef DEBUG
			qDebug() << ui.simulationModes->currentText();
#endif
			break;
		default:
			break;
	}
	
}

void ProsimMainWindow::on_inputFileDialogue_clicked(void)
{
#ifdef DEBUG
	qDebug() << "inputFileDialogue clicked";
#endif
}

void ProsimMainWindow::on_inputFileTextBox_returnPressed(void)
{
#ifdef DEBUG
	qDebug() << "Filename entered in inputFileTextBox: " << ui.inputFileTextBox->text();
#endif
}


void ProsimMainWindow::on_inputFileTextBox_textChanged(void)
{
#ifdef DEBUG
	qDebug() << "Filename entered in inputFileTextBox: " << ui.inputFileTextBox->text();
#endif
}


void ProsimMainWindow::on_outputFileDialogue_clicked(void)
{
#ifdef DEBUG
        qDebug() << "outputFileDialogue clicked";
#endif
	
}

void ProsimMainWindow::on_outputFileTextBox_returnPressed(void)
{
#ifdef DEBUG
	qDebug() << "Filename entered in outputFileTextBox: " << ui.outputFileTextBox->text();
#endif
}

void ProsimMainWindow::on_outputFileTextBox_textChanged(void)
{
#ifdef DEBUG
	qDebug() << "Filename entered in outputFileTextBox: " << ui.outputFileTextBox->text();
#endif
}

void ProsimMainWindow::on_resetButton_clicked(void)
{
#ifdef DEBUG
	qDebug() << "Reset clicked";
	int quantum = 0;
	int maxlength = 0;
	quantum = ui.quantumSpinBox->value();
	if (quantum == 0) {
		quantum = 1;
	}
	maxlength = ui.expirySpinBox->value();
	if (maxlength == 0) {
		maxlength = 1;
	}
        //command string construction
        string runstr = "./prosimmain -v -i test.file -a ";
        char buffer[4*sizeof(int)+1];
        //append algorithm
        runstr.append(ui.algorithmModes->currentText().toStdString());
        //append quantum if RR
        if(ui.algorithmModes->currentText().toStdString() == "RR") {
            runstr.append(" -q ");
            sprintf(buffer,"%d",quantum);
            runstr.append(buffer);
        }
        //append expire time
        runstr.append(" -x ");
        sprintf(buffer,"%d",maxlength);
        runstr.append(buffer);
        system(runstr.c_str());

        qDebug() << runstr.c_str();
        drawSequenceGrid(quantum, maxlength, 5);
#endif
}

void ProsimMainWindow::on_runButton_clicked()
{
#ifdef DEBUG
	qDebug() << "Run clicked";
	drawGrid();
#endif
}

// totalLength in quantum as the unit.
void ProsimMainWindow::drawSequenceGrid(int quantum, int totalLength, int numberProcs)
{
	numberProcs = numberProcs + 2;
	this->processScene.clear();
	int hori_unit = 0;
	int vert_unit = 0;
	hori_unit = ui.processGraph->width() / (totalLength);
	vert_unit = (ui.processGraph->height()) / (numberProcs);
	QLineF line;
	QPointF p1;
	QPointF p2;
	QPen pen;
	pen.setColor(Qt::black);
	
	for (int x = 0; x <= totalLength; x++) {
		p1.setX(x * hori_unit);
		p1.setY(1 * vert_unit);
		p2.setX(x * hori_unit);
		p2.setY((numberProcs - 1) * vert_unit);
		line.setPoints(p1, p2);
		this->processScene.addLine(line, pen);
	}
	
	for (int x = 1; x < (numberProcs); x++) {
		p1.setX(0);
		p1.setY(x * vert_unit);
		p2.setX(totalLength * hori_unit);
		p2.setY(x * vert_unit);
		line.setPoints(p1, p2);
		this->processScene.addLine(line, pen);
	}
        //open the file
        ifstream inFile;

        inFile.open("out.file");
        if (!inFile) {
            qDebug() << "Unable to open file";
            exit(1); // terminate with error
        }

        //read string
        std::string *str = new std::string();

        //read the processing data
        int start;
        int finish;
        char temp[20];
        string temp2;
        int i = 1;
        //get the line from the file
        while(std::getline(inFile, *str) != NULL) {
            //grab the job name part
            sscanf(str->c_str(),"%s",temp);
            //set tooltip name here

            //cut the string the first time
            temp2 = str->substr(strlen(temp),str->length());

            //PRINTING LOOP
            char temp3[10];
            while(strlen(temp2.c_str()) >= 6) {
                //rip the first string from the remaining coords string
                sscanf(temp2.c_str(),"%s",temp3);

                //scan for the first two integers
                sscanf(temp3,"%*c %d %*c %d",&start,&finish);
               // qDebug() << start;
                //qDebug() << finish;

                //print data to grid
                drawSequence(start,i,finish,hori_unit,vert_unit);

                //cut the coords string in order to repeat
                //qDebug() << temp3;
                temp2 = temp2.substr(strlen(temp3)+1,temp2.length());
                //qDebug() << temp2.c_str();
            }
            //END PRINT LOOP
            i++;
        }
        //close the file
        inFile.close();

        this->itemList.clear();
        this->itemList.append(this->processScene.items());
        QGraphicsItem *tempItem;
        char tempthing[25];
        for(int x = 0; x < this->itemList.length(); x++) {
                tempItem = this->itemList[x];
                sprintf(tempthing,"GridBox%d",x-totalLength-6);

                QString *elite = new QString(tempthing);
                tempItem->setToolTip(*elite);
                update();
        }
}

void ProsimMainWindow::drawSequence(int start, int i, int finish, int hori_unit, int vert_unit)
{
    QBrush rectBrush;
    QColor *lightGrey = new QColor(160,160,160);
    QColor *darkSlateGrey = new QColor(0,0,0);
    rectBrush.setColor(*lightGrey);
    rectBrush.setStyle(Qt::SolidPattern);
    QPen rectPen;
    rectPen.setColor(*darkSlateGrey);
    QRect tempRect;
    for(int k=start;k<start+finish;k++) {
        tempRect = QRect((k*hori_unit), (i*vert_unit), hori_unit,vert_unit);
        this->processScene.addRect(tempRect, rectPen,rectBrush);
    }
}

void ProsimMainWindow::drawGrid()
{
	this->memoryScene.clear();
	QBrush rectBrush;
	QColor *darkSlateGrey = new QColor(49,79,79);
	QColor *pastelBlue = new QColor(30,144,255);
        QColor *lightGreen = new QColor(0,200,0);
        QColor *Red = new QColor(200,0,0);
	rectBrush.setColor(*pastelBlue);

	rectBrush.setStyle(Qt::SolidPattern);
	QPen rectPen;
	rectPen.setColor(*darkSlateGrey);

        //open the file
        ifstream inFile;

        inFile.open("vout.file");
        if (!inFile) {
            qDebug() << "Unable to open file";
            exit(1); // terminate with error
        }

        //read string
        std::string *str = new std::string();

        //read up to the first line of memory data
        for(int i = 0;i<56;i++) {
            std::getline(inFile, *str);
        }
        //create and fill the page array
        int page[50];
        string str2;
        int sw = 0;
        for(int i =0;i<50;i++) {
            std::getline(inFile, *str);
            if(*str == "0") {
                page[i] = 0;
            }else {
                str2 = str->substr(0,2);
                sw = atoi(str2.c_str());
                page[i] = sw;
            }
        }
/*
        //create and fill the swap array
        int swap[150];
        for(int i =0;i<150;i++) {
            std::getline(inFile, *str);
            if(*str == "0") {
                page[i] = 0;
            }else {
                str2 = str->substr(0,2);
                sw = atoi(str2.c_str());
                page[i] = sw;
            }
        }
*/
        //close the file
        inFile.close();

        //physical memory cells
        for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 25; x++) {
                        int cell = (y*25)+(x+1);
                        if(page[cell-1] == 0) {
                            rectBrush.setColor(*lightGreen);
                            QRect tempRect = QRect(x * (ui.memoryGraph->width() / 25), y * (ui.memoryGraph->height() / 5), 30, 30);
                            this->memoryScene.addRect(tempRect, rectPen,rectBrush);
                        }else {
                            rectBrush.setColor(*pastelBlue);
                            QRect tempRect = QRect(x * (ui.memoryGraph->width() / 25), y * (ui.memoryGraph->height() / 5), 30, 30);
                            this->memoryScene.addRect(tempRect, rectPen,rectBrush);
                        }
                }
        }


        this->itemList.clear();
        this->itemList.append(this->memoryScene.items());
        QGraphicsItem *tempItem;
        char tempthing[25];
        for(int x = 0; x < this->itemList.length(); x++) {
                tempItem = this->itemList[x];
                if(page[x] == 0) {sprintf(tempthing,"Page%d Empty",x+1);}
                else if(page[x] == -10) {sprintf(tempthing,"Page%d Unused Fragment",x+1);}
                else{sprintf(tempthing, "Frame%d Job%d",x+1,page[x]);}
                QString *elite = new QString(tempthing);
                tempItem->setToolTip(*elite);
                update();
        }
}
