#ifndef QtGUI_H
#define QtGUI_H

#include <QApplication>
#include <QTextEdit>
#include <QtGui>

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
private:
	QTextEdit *textEdit;

	QAction *openAction;
	QAction *saveAction;
	QAction *exitAction;

	QMenu *fileMenu;
};

#endif // !QtGUI_H
