#include "MainWindow.h"
#include "Settings.h"
#include "ImageWorker.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include "ResizeDialog.h"
#include <fstream>
#include <QPrintPreviewDialog>
#include <QPainter>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui_()
	, images_()
	, timer_()
{
	ui_.setupUi(this);
	setWindowTitle(QApplication::applicationName());
	setWindowState(Qt::WindowMaximized);

	//set reload action group
	QActionGroup* group = new QActionGroup(this);
	group->addAction(ui_.actionReloadAsk);
	group->addAction(ui_.actionReloadNo);
	group->addAction(ui_.actionReloadAuto);
	ui_.actionReloadAuto->setChecked(true);

	//init file watcher
	watcher_.setDelayInSeconds(0.001);
	connect(&watcher_, SIGNAL(fileChanged()), this, SLOT(reloadFile()));

	//start timer for image size update
	connect(&timer_, SIGNAL(timeout()), this, SLOT(updateImageSize()));
	timer_.start(250);

	//init images
	QStringList args = QApplication::arguments();
	if (args.count()>1 && QFile::exists(args[1]))
	{
		QFileInfo file_info(args[1]);
		updateImageList(file_info.canonicalPath(), file_info.fileName());
	}
	else
	{
		updateImageList(Settings::path("folder"));
	}
}

void MainWindow::on_actionAbout_triggered(bool)
{
	QMessageBox::about(this, "About " + QApplication::applicationName(), QApplication::applicationName() + " " + QApplication::applicationVersion() +"\n\nThis program is free software.\n\nThis program is provided as is with no warranty of any kind, including the warranty of design, merchantability and fitness for a particular purpose.");
}


void MainWindow::on_actionOpenFile_triggered(bool)
{
	QString file = QFileDialog::getOpenFileName(this, "Select file", Settings::path("folder"), "All files (*.*);;PNG files(*.png);;JPG files (*.jpg)");
	if (file=="") return;

	QFileInfo file_info(file);
	Settings::setPath("folder", file_info.canonicalPath());
	updateImageList(file_info.canonicalPath(), file_info.fileName());
}

void MainWindow::on_actionOpenFolder_triggered(bool)
{
	QString folder = QFileDialog::getExistingDirectory(this, "Select folder", Settings::path("folder"));
	if (folder=="") return;

	Settings::setPath("folder", folder);
	updateImageList(folder);
}

void MainWindow::on_actionPrevious_triggered(bool)
{
	if (current_==0) return;

	updateImage(current_-1);
}

void MainWindow::on_actionNext_triggered(bool)
{
	if (current_==images_.count()-1) return;

	updateImage(current_+1);
}

void MainWindow::on_actionRotateLeft_triggered(bool)
{
	//no nothing without images
	if (images_.count()==0) return;

	//transform image
	QTransform transform;
	transform.rotate(270);
	pixmap_ = pixmap_.transformed(transform);

	//store it
	storeCurrentImage(false);

	//update GUI
	updateImageSize();
}

void MainWindow::on_actionRotateRight_triggered(bool)
{
	//no nothing without images
	if (images_.count()==0) return;

	//transform image
	QTransform transform;
	transform.rotate(90);
	pixmap_ = pixmap_.transformed(transform);

	//store it
	storeCurrentImage(false);

	//update GUI
	updateImageSize();
}

void MainWindow::on_actionDelete_triggered(bool)
{
	//no nothing without images
	if (images_.count()==0) return;

	//delete image
	images_[current_].remove();
	images_.removeAt(current_);

	//update gui
	if (current_!=images_.count())
	{
		updateImage(current_);
	}
	else //last image deleted
	{
		updateImage(images_.count()-1);
	}
}

void MainWindow::on_actionRename_triggered(bool)
{
	if (images_.count()==0) return;

	QString folder = images_[0].folder();
	for (int i=0; i<images_.count(); ++i)
	{
		//get EXIF date
		QByteArray date;
		if (!getExifDate(images_[i].filename(), date))
		{
			QMessageBox::warning(this, "EXIF data not found!", "EXIF creation date not found for file:\n" + images_[i].filename() + "\nMessage: " + date + "\nAborting!");
			break;
		}

		//format '2004:04:23 13:05:04' to '20040423_130504'
		date.replace(":", "").replace(" ","_");

		//determine new file name
		QString file_o = images_[i].filename(false);
		QString ext = file_o.right(4);
		int number = 1;
		while (QFile::exists(folder + "\\" + date + "-" + QString::number(number) + ext))
		{
			++number;
		}

		//rename
		QFile::rename(images_[i].filename(), folder + "\\" + date + "-" + QString::number(number) + ext);
	}

	updateImageList(folder);
	updateImage(0);
}

void MainWindow::on_actionResize_triggered(bool)
{
	//no nothing without images
	if (images_.count()==0) return;

	//show dialog
	ResizeDialog dlg(this);
	dlg.setSize(pixmap_.width(), pixmap_.height());
	if (dlg.exec()!=QDialog::Accepted) return;

	//transform image
	pixmap_ = pixmap_.scaled(dlg.width(), dlg.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	//store it
	storeCurrentImage(true);

	//update GUI
	updateImageSize();
}

void MainWindow::on_actionBlackWhite_triggered(bool)
{
	//no nothing without images
	if (images_.count()==0) return;

	//transform image
	QImage image = pixmap_.toImage();
	for (int i=0; i<pixmap_.width(); ++i)
	{
		for (int j=0; j<pixmap_.height(); ++j)
		{
			int gray = qGray(image.pixel(i, j));
			image.setPixel(i, j, qRgb(gray, gray, gray));
		}
	}
	pixmap_ = QPixmap::fromImage(image);

	//store it
	storeCurrentImage(true);
}

void MainWindow::on_actionPrint_triggered(bool)
{
	//no nothing without images
	if (images_.count()==0) return;

	QPrinter printer;
	printer.setResolution(180);
	printer.setPaperSize(QPrinter::A4);
	printer.setOrientation(QPrinter::Portrait);
	QPrintPreviewDialog dlg(&printer, this);
	dlg.setWindowFlags ( Qt::Window );
	connect(&dlg, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter *)));
	if(dlg.exec() == QDialog::Rejected) return;
}

void MainWindow::printPreview(QPrinter* printer)
{
	QImage image(images_[current_].filename());
	QPainter painter(printer);
	QRect rect = painter.viewport();
	QSize size = image.size();
	size.scale(rect.size(), Qt::KeepAspectRatio);
	painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
	painter.setWindow(image.rect());
	painter.drawImage(0, 0, image);
}

void MainWindow::reloadFile()
{
	if (ui_.actionReloadNo->isChecked()) return;

	if (ui_.actionReloadAsk->isChecked())
	{
		if (QMessageBox::question(this, "Reload?", "Image has changed on disk! Reload?")==QMessageBox::No) return;
	}

	images_[current_].clearCache();
	updateImage(current_);
}

void MainWindow::updateImage(int current)
{
	//clear image
	ui_.image->setPixmap(QPixmap());

	//update index
	current_ = current;
	if (current_==images_.count()) return;

	//reset file watcher
	watcher_.clearFile();

	//update image
	pixmap_ = images_[current_].pixmap();
	updateImageSize();

	//set file watcher
	watcher_.setFile(images_[current_].filename());

	//status bar
	ui_.statusBar->showMessage(images_[current_].filename() + " (" + QString::number(current_+1) + "\\" + QString::number(images_.count()) + ")");

	//cache/uncache images
	clearCache(current_-3);
	cache(current_-2);
	cache(current_-1);
	cache(current_+1);
	cache(current_+2);
	clearCache(current_+3);
}

void MainWindow::cache(int index)
{
	if (index<0) return;
	if (index>=images_.count()) return;

	images_[index].cache();
}

void MainWindow::clearCache(int index)
{
	if (index<0) return;
	if (index>=images_.count()) return;

	images_[index].clearCache();
}

void MainWindow::storeCurrentImage(bool backup)
{
	if (backup)
	{
		images_[current_].backup();
	}

	QString filename = images_[current_].filename();
	pixmap_.save(filename);
}

bool MainWindow::getExifDate(QString filename, QByteArray& output)
{
	//open file
	std::ifstream stream;
	stream.open(filename.toLatin1().data(), std::ios::binary);
	if (!stream.good())
	{
		output = "Could not open file!";
		return false;
	}

	QByteArray buffer(5120, ' ');
	stream.read(buffer.data(), 5120);

	if (!buffer.left(20).contains("Exif"))
	{
		output = "No EXIF data in file!";
		return false;
	}

	QRegExp rx("\\d\\d\\d\\d:\\d\\d:\\d\\d \\d\\d:\\d\\d:\\d\\d");
	for (int i = 0; i<5100; ++i)
	{
		QByteArray suffix = QByteArray(buffer.data()+i, 19);
		if (rx.exactMatch(suffix))
		{
			output = suffix;
			return true;
		}
	}

	output = "No EXIF date found!";
	return false;
}

void MainWindow::updateImageSize()
{
	//no nothing without images
	if (images_.count()==0) return;

	//bound width/height by picture size
	int width = std::min(pixmap_.width(), ui_.image->width());
	int height = std::min(pixmap_.height(), ui_.image->height());

	ui_.image->setPixmap(pixmap_.scaled(width, height, Qt::KeepAspectRatio));
}

void MainWindow::updateImageList(QString folder, QString name)
{
	//init
	setWindowTitle(QApplication::applicationName() + " - " + folder);
	images_.clear();
	int index = 0;

	//load image list
	QStringList files = QDir(folder).entryList(QDir::Files, QDir::Name);
	foreach(QString file, files)
	{
		if (file==name)
		{
			index = images_.count();
		}
		if (file.endsWith(".jpg", Qt::CaseInsensitive) || file.endsWith(".png", Qt::CaseInsensitive))
		{
			images_.append(Image(folder + "\\", file));
		}
	}

	//update index
	updateImage(index);
}
