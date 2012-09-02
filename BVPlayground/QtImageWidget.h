#ifndef QtImageWidget_H
#define QtImageWidget_H

#include <QWidget>
#include <QtGui>
#include <QtOpenGL>
#include <cv.h>

class QtImageWidget : public QWidget {

	Q_OBJECT

public:
	QtImageWidget(QWidget *parent);
	~QtImageWidget(){};
	void updatePixmap(const IplImage *frame);

public slots:
		void onFrameSizeChanged(int width, int height);
signals:
		void frameSizeChanged(int width, int height);

protected:
	void paintEvent(QPaintEvent* evt);

private:
	int imageWidth, imageHeight;
	uchar* imageData;
};

#endif