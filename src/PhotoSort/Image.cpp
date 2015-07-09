#include "Image.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>

Image::Image(QString folder, QString name)
	: folder_(folder.replace("\\\\", "\\"))
	, name_(name)
	, worker_(0)
{
}

Image::~Image()
{
	if (worker_!=0)
	{
		worker_->deleteLater();
		worker_ = 0;
	}
}

QPixmap Image::pixmap() const
{
	if(worker_!=0 && !worker_->pixmap().isNull())
	{
		return worker_->pixmap();
	}
	else
	{
		return QPixmap(folder_ + name_);
	}
}

QString Image::folder() const
{
	return folder_;
}

QString Image::filename(bool include_folder) const
{
	if (include_folder)
	{
		return folder_ + name_;
	}

	return name_;
}

void Image::remove()
{
	//create output folder if necessary
	QDir output(folder_ + "/+deleted/");
	if (!output.exists())
	{
		output.mkdir(folder_ + "/+deleted/");
	}

	//delete
	QFile::rename(folder_ + name_, output.path() + "\\" + name_);
}

void Image::backup()
{
	//create output folder if necessary
	QDir output(folder_ + "/+backup/");
	if (!output.exists())
	{
		output.mkdir(folder_ + "/+backup/");
	}

	//do not create a backup when backup already exists from previous image manipulation
	QString outname = output.path() + "\\" + name_;
	if (QFile::exists(outname)) return;

	//copy
	QFile::copy(folder_ + name_, output.path() + "\\" + name_);
}

void Image::cache()
{
	if (worker_==0)
	{
		worker_ = new ImageWorker(folder_ + name_);
		worker_->start();
	}
}

void Image::clearCache()
{
	if (worker_!=0)
	{
		worker_->deleteLater();
		worker_ = 0;
	}
}
