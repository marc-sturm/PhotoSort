#ifndef MainWindow_H
#define MainWindow_H

#include "ui_MainWindow.h"
#include "Image.h"
#include "FileWatcher.h"
#include <QMainWindow>
#include <QTimer>
#include <QPrinter>

class MainWindow
		: public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);

	///Reloads the image list.
	void updateImageList(QString folder, QString name="");
	///Updates the current image.
	void updateImage(int current);
	///Triggers caching an image.
	void cache(int index);
	///Clears a cached image.
	void clearCache(int index);
	///Stores the current image and makes a backup if requested.
	void storeCurrentImage(bool backup);

	///Returns the EXIF data (probably creation date)
	static bool getExifDate(QString filename, QByteArray& output);

public slots:
	void on_actionAbout_triggered(bool);
	void on_actionOpenFile_triggered(bool);
	void on_actionOpenFolder_triggered(bool);
	void on_actionPrevious_triggered(bool);
	void on_actionNext_triggered(bool);
	void on_actionRotateLeft_triggered(bool);
	void on_actionRotateRight_triggered(bool);
	void on_actionDelete_triggered(bool);
	void on_actionRename_triggered(bool);
	void on_actionDuplicates_triggered(bool);
	void on_actionResize_triggered(bool);
	void on_actionBlackWhite_triggered(bool);
	void on_actionPrint_triggered(bool);

	///Reloads file after it has changed
	void reloadFile();
	///Update image size when window size changes.
	void updateImageSize();
	///Printing preview
	void printPreview(QPrinter* printer);

private:
	Ui::MainWindow ui_;
	QList<Image> images_;
	int current_;
	QPixmap pixmap_;
	QTimer timer_;
	FileWatcher watcher_;
};
#endif // MainWindow_H
