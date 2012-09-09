#ifndef QtGUI_H
#define QtGUI_H

#include <QApplication>
#include <QTextEdit>
#include <QtGui>
#include <highgui.h>
#include "QtImageWidget.h"

#include "Fisheye.h"

class QtGUI : public QMainWindow {

	Q_OBJECT

public:
	QtGUI();
	//~QtGUI(){};
	//int show(int argv, char **args);

	private slots:
		void open();
		void save();
		void quit();
		void defisheye();
private:
	QTextEdit *textEdit;
	QtImageWidget *imageWidget;

	// file menu
	QAction *openAction;
	QAction *saveAction;
	QAction *exitAction;

	// fisheye menu
	QAction *defisheyeAction;

	IplImage *img;

	QMenu *fileMenu;
	QMenu *fisheyeMenu;
};

#endif // !QtGUI_H
