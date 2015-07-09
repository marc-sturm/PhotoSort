#include "ImageWorker.h"
#include <QDebug>

ImageWorker::ImageWorker(QString filename)
	: WorkerBase("Image prefetching")
	, filename_(filename)
{
}

void ImageWorker::process()
{
	pixmap_ = QPixmap(filename_);
}

QPixmap ImageWorker::pixmap()
{
	return pixmap_;
}
