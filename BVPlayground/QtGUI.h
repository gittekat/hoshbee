#ifndef QtGUI_H
#define QtGUI_H

#include <QApplication>
#include <QTextEdit>

class QtGUI{
public:
	QtGUI(){};
	~QtGUI(){};
	int show(int argv, char **args);
};

#endif // !QtGUI_H
