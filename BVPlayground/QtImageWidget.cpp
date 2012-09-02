#include "QtImageWidget.h"

QtImageWidget::QtImageWidget(QWidget *parent) : QWidget(parent), imageData(0), imageWidth(0), imageHeight(0) {
	setAttribute(Qt::WA_OpaquePaintEvent, true); // don't clear the area before the paintEvent
	setAttribute(Qt::WA_PaintOnScreen, true); // disable double buffering
	connect(this, SIGNAL(frameSizeChanged(int, int)), this, SLOT(onFrameSizeChanged(int, int)), Qt::QueuedConnection);
}

void QtImageWidget::updatePixmap(const IplImage *frame) {

	if(frame->width != imageWidth || frame->height != imageHeight) { 
		if(imageData) {
			delete[] imageData;
		}
		imageWidth = frame->width;
		imageHeight = frame->height;
		emit(frameSizeChanged(imageWidth, imageHeight));
		imageData = new unsigned char[4*imageWidth*imageHeight];
		for(int i = 0; i < imageWidth*imageHeight; i++) {
			imageData[i*4+3] = 0xFF;
		}
	}

	int pixels = imageWidth * imageHeight;
	uchar* src = (uchar*)(frame->imageData);
	uchar* srcEnd = src + (3*pixels);
	uchar* dest = imageData;

	do {
		memcpy(dest, src, 3);
		dest += 4;
		src += 3;
	} while(src < srcEnd);

	update();
	qDebug() << "Done Copying data";
}

void QtImageWidget::paintEvent(QPaintEvent* evt) {
	QPainter painter(this);
	if(imageData) {
		QImage tImg(imageData, imageWidth, imageHeight, QImage::Format_RGB32);
		painter.drawImage(QPoint(0,0), tImg);
	}
	else {
		painter.setBrush(Qt::black);
		painter.drawRect(rect());
	}
}

void QtImageWidget::onFrameSizeChanged(int width, int height) {
	setFixedSize(width, height);

	qDebug() << "changing size" << width << " " << height;
} 