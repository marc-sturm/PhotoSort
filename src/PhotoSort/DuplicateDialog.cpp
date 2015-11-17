#include "DuplicateDialog.h"
#include "DuplicatePairDialog.h"
#include "ui_DuplicateDialog.h"
#include "Helper.h"
#include <QFileDialog>

DuplicateDialog::DuplicateDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DuplicateDialog)
{
	ui->setupUi(this);
	connect(ui->search, SIGNAL(clicked(bool)), this, SLOT(search()));
	connect(&worker_, SIGNAL(finished(bool)), this, SLOT(workerFinished()));
	connect(&worker_, SIGNAL(message(QString)), this, SLOT(showMessage(QString)));
	connect(this, SIGNAL(checkPairs()), this, SLOT(showPairs()));
	connect(ui->browse, SIGNAL(clicked(bool)), this, SLOT(browse()));
}

void DuplicateDialog::setFolder(QString folder)
{
	ui->folder->setText(folder);
}

DuplicateDialog::~DuplicateDialog()
{
	delete ui;
}

void DuplicateDialog::search()
{
	ui->search->setEnabled(false);

	//create file list
	QStringList filters = ui->types->text().split(" ", QString::SkipEmptyParts);
	QStringList files;
	foreach(QString filter, filters)
	{
		QStringList tmp;
		Helper::findFiles(ui->folder->text(), filter, tmp);
		files << tmp;
	}

	//start worker
	worker_.set(files, 1.0);
	worker_.start();
}

void DuplicateDialog::workerFinished()
{
	dups_ = worker_.duplicates();
	showMessage(QString::number(dups_.count()) + " duplicates found!");
	emit checkPairs();
}

void DuplicateDialog::showMessage(QString message)
{
	ui->status->setText(message);
}

void DuplicateDialog::showPairs()
{
	QSet<QString> removed;
	for (int i=0; i<dups_.count(); ++i)
	{
		QString f1 = dups_[i].first;
		QString f2 = dups_[i].second;
		if (removed.contains(f1) || removed.contains(f2)) continue;

		DuplicatePairDialog dlg(this, f1, f2);
		if (dlg.exec()==QDialog::Accepted)
		{
			QString name = dlg.imageToDelete();
			removed.insert(name);
			QFile(name).remove();
		}
	}
}

void DuplicateDialog::browse()
{
	QString folder = QFileDialog::getExistingDirectory(this, "Select folder", ui->folder->text());
	if (folder!="")
	{
		ui->folder->setText(folder);
	}
}

