#include "QtGUI.h"


QtGUI::QtGUI() {
	openAction = new QAction(tr("&Open"), this);
	saveAction = new QAction(tr("&Save"), this);
	exitAction = new QAction(tr("E&xit"), this);
	
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	defisheyeAction = new QAction(tr("&Defisheye"), this);

	connect(defisheyeAction, SIGNAL(triggered()), this, SLOT(defisheye()));

	fisheyeMenu = menuBar()->addMenu(tr("&Fisheye")); 
	fisheyeMenu->addAction(defisheyeAction);

	//textEdit = new QTextEdit;
	

	imageWidget = new QtImageWidget(this);
	img = cvLoadImage("..\\img\\fisheye_equisolid.png");
	imageWidget->updatePixmap(img);
	setCentralWidget(imageWidget);

	setWindowTitle(tr("Playground"));
}

void QtGUI::open() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png);;"));
	QByteArray byteArray = fileName.toUtf8();
	const char* cString = byteArray.constData();
	img = cvLoadImage(cString);
	imageWidget->updatePixmap(img);

	//if (fileName != "") {
	//	QFile file(fileName);
	//	if (!file.open(QIODevice::ReadOnly)) {
	//		QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
	//		return;
	//	}
	//	QTextStream in(&file);
	//	textEdit->setText(in.readAll());
	//	file.close();
	//}
}

void QtGUI::save() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

	if (fileName != "") {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			// error message
		} else {
			QTextStream stream(&file);
			stream << textEdit->toPlainText();
			stream.flush();
			file.close();
		}
	}
}

void QtGUI::quit() {
	exit(0);
}

void QtGUI::defisheye() {
	if (!img) {
		printf("Error: Couldn't open the image file.\n");
		return;
	}

	double angle = 180.0;
	double tmp = (angle / 360.0) * M_PI; // conversion DEG to RAD of half the angle of view
	double focal_length = 1.0 / (2 * sin(tmp/2));

	Fisheye* fisheye = new Fisheye(img, focal_length, 500.0, img->width/2, img->height/2);
	fisheye->deFisheye3();	
}
