#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H

#include "WorkerBase.h"
#include <QPixmap>

class ImageWorker
		: public WorkerBase
{
	Q_OBJECT

public:
	ImageWorker(QString filename);

public slots:
	virtual void process();
	QPixmap pixmap();

protected:
	QString filename_;
	QPixmap pixmap_;
};

#endif // IMAGEWORKER_H
