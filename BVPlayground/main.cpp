#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "QtGUI.h"
#include "Fisheye.h"

int main(int argv, char **args) {
	QApplication app(argv, args);
	QtGUI gui;
	gui.show();

	return app.exec();
}