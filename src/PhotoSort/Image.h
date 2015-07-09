#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QPixmap>
#include "ImageWorker.h"

///Image representation.
class Image
{
public:
	///Constructor
	Image(QString folder, QString filename);
	///Destructor
	~Image();

	///Returns the pixmap either loading it from disk or from cache.
	QPixmap pixmap() const;
	///Returns the folder
	QString folder() const;
	///Returns the filename
	QString filename(bool include_folder = true) const;


	///Removes the file (and moves into the "+deleted" subfolder)
	void remove();
	///Creates a backup of the file in the "+backup" subfolder)
	void backup();

	///Triggers caching of pixmap
	void cache();
	///Clears cached pixmap
	void clearCache();

protected:
	QString folder_;
	QString name_;
	ImageWorker* worker_;

private:
	//Defined away
	Image();
};

#endif // IMAGE_H
