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
#include <stack>
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
        ui.inputFileTextBox->setText("test.file");
        ui.quantumSpinBox->setEnabled(false);
        ui.quantumLabel->setEnabled(false);
        ui.expiryLabel->setEnabled(false);
        ui.expirySpinBox->setEnabled(false);
}

void ProsimMainWindow::on_algorithmModes_activated(int value)
{
	switch (value) {
		case 0:
#ifdef DEBUG
			qDebug() << ui.algorithmModes->currentText();
#endif
                        ui.quantumSpinBox->setEnabled(false);
                        ui.quantumLabel->setEnabled(false);
			break;
		case 1:
#ifdef DEBUG
			qDebug() << ui.algorithmModes->currentText();

                        ui.quantumSpinBox->setEnabled(true);
                        ui.quantumLabel->setEnabled(true);
#endif
			break;
		case 2:
#ifdef DEBUG
			qDebug() << ui.algorithmModes->currentText();
#endif
                        ui.quantumSpinBox->setEnabled(false);
                        ui.quantumLabel->setEnabled(false);
			break;
		case 3:
#ifdef DEBUG
			qDebug() << ui.algorithmModes->currentText();
#endif
                        ui.quantumSpinBox->setEnabled(false);
                        ui.quantumLabel->setEnabled(false);
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
                        ui.expiryLabel->setEnabled(false);
                        ui.expirySpinBox->setEnabled(false);
			break;
		case 1:
#ifdef DEBUG
			qDebug() << ui.simulationModes->currentText();
#endif
                        ui.expiryLabel->setEnabled(true);
                        ui.expirySpinBox->setEnabled(true);
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
        this->processScene.clear();
        this->memoryScene.clear();
        ui.quantumSpinBox->setValue(0);
        ui.expirySpinBox->setValue(0);
}
#endif
int ProsimMainWindow::getNumProcs(void) {
    //open the file
    ifstream inFile;
    int num = 0;

    inFile.open("out.file");
        if (!inFile) {
            qDebug() << "Unable to open file";
            exit(1); // terminate with error
        }

    //read string
    std::string *str = new std::string();

    //get the line from the file
    while(std::getline(inFile, *str) != NULL) {
        num++;
    }
    return num;
}

int ProsimMainWindow::getFinishTime(void) {
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
        int start = 1;
        int finish = 1;
        char temp[20];
        string temp2;
        int i = 1;
        //get the line from the file
        while(std::getline(inFile, *str) != NULL) {
                //grab the job name part
                sscanf(str->c_str(),"%s",temp);

                //cut the string the first time
                temp2 = str->substr(strlen(temp),str->length());

                char temp3[10];
                while(strlen(temp2.c_str()) >= 6) {
                        //rip the first string from the remaining coords string
                        sscanf(temp2.c_str(),"%s",temp3);

                        //scan for the first two integers
                        sscanf(temp3,"%*c %d %*c %d",&start,&finish);

                        //cut the coords string in order to repeat
                        temp2 = temp2.substr(strlen(temp3)+1,temp2.length());
                }
                i++;
        }
        inFile.close();
        //finish time calculation
        int final = start+finish;
        qDebug() << final;
        return final;
}

void ProsimMainWindow::on_runButton_clicked()
{
#ifdef DEBUG
	qDebug() << "Run clicked";

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
        string runstr = "./prosimmain ";
        if(ui.simulationModes->currentText().toStdString() == "Scheduler") {
            runstr.append("-s ");
        }else if(ui.simulationModes->currentText().toStdString() == "Virtual Memory") {
            runstr.append("-v ");
        }
        //append input file
        runstr.append("-i ");
        runstr.append(ui.inputFileTextBox->text().toStdString());
        runstr.append(" -a ");
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

        maxlength = getFinishTime();
        int numprocs = getNumProcs();

        if(ui.simulationModes->currentText().toStdString() == "Virtual Memory") {
            drawGrid();
        }else {
            this->memoryScene.clear();
        }
        drawSequenceGrid(quantum, maxlength, numprocs);
#endif
}

// totalLength in quantum as the unit.
void ProsimMainWindow::drawSequenceGrid(int quantum, int totalLength, int numberProcs)
{
	numberProcs = numberProcs + 2;
	this->processScene.clear();
	int hori_unit = 0;
	int vert_unit = 0;
        hori_unit = ui.processGraph->width() / (totalLength)-2;
	vert_unit = (ui.processGraph->height()) / (numberProcs);
	QLineF line;
	QPointF p1;
	QPointF p2;
        QColor *lightGreen = new QColor(0,220,0,100);
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
        stack<string> jobstack;

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

                //print data to grid & store nametags in the stack
                drawSequence(start,i,finish,hori_unit,vert_unit);
                for(int i=0;i<finish;i++) {
                    //qDebug() << temp;
                    jobstack.push(temp);
                }

                //cut the coords string in order to repeat
                temp2 = temp2.substr(strlen(temp3)+1,temp2.length());
            }
            //END PRINT LOOP
            i++;
        }

        //close the file
        inFile.close();

        //flip the namestack
        stack<string> final_name;
        for(int i=0;i<totalLength;i++) {
            final_name.push(jobstack.top());
            jobstack.pop();
        }

        this->itemList.clear();
        this->itemList.append(this->processScene.items());
        QGraphicsItem *tempItem;
        char tempthing[25];
        int griditemoffset = numberProcs + totalLength;
        char name[10];
        for(int x = griditemoffset; x < this->itemList.length(); x++) {
                tempItem = this->itemList[x];
                tempItem->setZValue(-1);
                sprintf(name,"%s",final_name.top().c_str());
                final_name.pop();
                QString *elite = new QString(name);
                tempItem->setToolTip(*elite);
                update();
        }

         //expiry indicator
        if(ui.simulationModes->currentText().toStdString() == "Virtual Memory") {
            pen.setColor(*lightGreen);
            pen.setWidth(8);
            int expire = ui.expirySpinBox->value();
            p1.setX(expire*hori_unit);
            p1.setY(vert_unit);
            p2.setX(expire*hori_unit);
            p2.setY(vert_unit*(numberProcs-1));
            line.setPoints(p1,p2);
            this->processScene.addLine(line,pen);
            QGraphicsItem * gip = this->itemList.last();
            gip->setZValue(1);
        }
        

        //print file to text area
        QFont *font = new QFont("Tahoma",9,9,true);
        ui.outputTextArea->setFont(*font);
        ui.outputTextArea->clear();
        if(ui.simulationModes->currentText().toStdString() == "Virtual Memory") {
            inFile.open("vout.file");
            while(std::getline(inFile, *str) != NULL) {
                ui.outputTextArea->insertPlainText(str->c_str());
                ui.outputTextArea->insertPlainText("\n");
            }
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

        //read up to the first line of physical memory data
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

        //read up to the first line of virtual memory data
        for(int i = 0;i<4;i++) {
            std::getline(inFile, *str);
            qDebug() << str->c_str();
        }

        //create and fill the swap array
        int swap[150];
        for(int i =0;i<150;i++) {
            std::getline(inFile, *str);
            if(*str == "0") {
                swap[i] = 0;
            }else {
                str2 = str->substr(0,2);
                sw = atoi(str2.c_str());
                swap[i] = sw;
            }
        }

        //close the file
        inFile.close();

        //physical memory cells
        for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 25; x++) {
                        int cell = (y*25)+(x+1);
                        if(page[cell-1] == 0) {
                            rectBrush.setColor(*lightGreen);
                            QRect tempRect = QRect(x * (ui.memoryGraph->width() / 25), y * (ui.memoryGraph->height() / 9), 30, 28);
                            this->memoryScene.addRect(tempRect, rectPen,rectBrush);
                        }else {
                            rectBrush.setColor(*pastelBlue);
                            QRect tempRect = QRect(x * (ui.memoryGraph->width() / 25), y * (ui.memoryGraph->height() / 9), 30, 28);
                            this->memoryScene.addRect(tempRect, rectPen,rectBrush);
                        }
                }
        }

        for (int y = 0; y < 6; y++) {
                for (int x = 0; x < 25; x++) {
                        int cell = (y*25)+(x+1);
                        if(swap[cell-1] == 0) {
                            rectBrush.setColor(*lightGreen);
                            QRect tempRect = QRect(x * (ui.memoryGraph->width() / 25), y * (ui.memoryGraph->height() / 9)+75, 30, 28);
                            this->memoryScene.addRect(tempRect, rectPen,rectBrush);
                        }else {
                            rectBrush.setColor(*Red);
                            QRect tempRect = QRect(x * (ui.memoryGraph->width() / 25), y * (ui.memoryGraph->height() / 9)+75, 30, 28);
                            this->memoryScene.addRect(tempRect, rectPen,rectBrush);
                        }
                }
        }


        this->itemList.clear();
        this->itemList.append(this->memoryScene.items());
        QGraphicsItem *tempItem;
        char tempthing[25];
        for(int x = 0; x < 50; x++) {
                tempItem = this->itemList[x];
                if(page[x] == 0) {sprintf(tempthing,"Frame%d Empty",x+1);}
                else{sprintf(tempthing, "Frame%d Job%d",x+1,page[x]);}
                QString *elite = new QString(tempthing);
                tempItem->setToolTip(*elite);
                update();
        }
        for(int x = 50; x < 200; x++) {
                tempItem = this->itemList[x];
                if(swap[x-50] == 0) {sprintf(tempthing,"Frame%d Empty",x-49);}
                else{sprintf(tempthing, "Frame%d Job%d",x-49,swap[x-50]);}
                QString *elite = new QString(tempthing);
                tempItem->setToolTip(*elite);
                update();
        }
}
