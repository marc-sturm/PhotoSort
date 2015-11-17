#include "FingerPrintWorker.h"
#include <QImage>
#include <QDebug>
#include <QFileInfo>

FingerPrintWorker::FingerPrintWorker()
	: WorkerBase("FingerPrintWorker")
{
}

void FingerPrintWorker::set(QStringList files, double max_diff)
{
	files_ = files;
	max_diff_ = max_diff;
}

void FingerPrintWorker::process()
{
	//calculating fingerprints
	QList<FingerPrint> fps;
	for (int i=0; i<files_.count(); ++i)
	{
		QString file = files_[i];
		QImage img(file);
		FingerPrint fp;
		fp.filename = QFileInfo(file).canonicalFilePath();

		emit message("Calculating fingerprint of " + fp.filename + " (" + QString::number(i+1) + "/" + QString::number(files_.count()) + ")");

		fp.w = img.width();
		fp.h = img.height();
		for (int x=0; x<fp.w; ++x)
		{
			fp += img.pixel(x, 0);
			fp += img.pixel(x, fp.h-1);
		}
		for (int y=1; y<fp.h-1; ++y)
		{
			fp += img.pixel(0, y);
			fp += img.pixel(fp.w-1, y);
		}

		fps.append(fp);
	}

	//compare fingerprints
	for (int i=0; i<fps.count(); ++i)
	{
		for (int j=i+1; j<fps.count(); ++j)
		{
			emit message("Comparing fingerprint pair " + QString::number(i+1) + "/" + QString::number(j+1) + " (of " + QString::number(fps.count()) + " fingerprints)");
			double diff = fps[i].diff(fps[j]);
			if (diff<max_diff_)
			{
				duplicates_.append(qMakePair(fps[i].filename, fps[j].filename));
			}
		}
	}
}

QList<QPair<QString, QString>> FingerPrintWorker::duplicates() const
{
	return duplicates_;
}
