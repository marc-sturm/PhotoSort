#include "DuplicateDialog.h"
#include "ui_DuplicateDialog.h"
#include "Helper.h"
#include <QDir>
#include <QDebug>

DuplicateDialog::DuplicateDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DuplicateDialog)
{
	ui->setupUi(this);
	connect(ui->search, SIGNAL(clicked(bool)), this, SLOT(search()));
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
	//create file list
	QStringList files;
	QStringList filters = ui->types->text().split(" ", QString::SkipEmptyParts);
	foreach(QString filter, filters)
	{
		QStringList tmp;
		Helper::findFiles(ui->folder->text(), filter, tmp);
		files << tmp;
	}

	qDebug() << files;
}
