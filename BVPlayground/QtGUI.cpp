#include "QtGUI.h"

int QtGUI::show(int argv, char **args)
{
	QApplication app(argv, args);
	QTextEdit textEdit;
	textEdit.show();

	return app.exec();
}