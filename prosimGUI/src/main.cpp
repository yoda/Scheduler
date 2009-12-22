#include <QtGui>
#include "ProsimMainWindow.h"

//! [0]
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	ProsimMainWindow mw;
	
	mw.show();
	return app.exec();
}
