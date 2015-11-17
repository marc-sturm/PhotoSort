#include "DuplicatePairDialog.h"
#include "ui_DuplicatePairDialog.h"

DuplicatePairDialog::DuplicatePairDialog(QWidget* parent, QString file1, QString file2)
	: QDialog(parent),
	ui(new Ui::DuplicatePairDialog)
{
	ui->setupUi(this);

	ui->name1->setText(file1);
	ui->img1->setPixmap(QPixmap(file1));
	connect(ui->del1, SIGNAL(clicked(bool)), this, SLOT(deleteImage1()));

	ui->name2->setText(file2);
	ui->img2->setPixmap(QPixmap(file2));
	connect(ui->del2, SIGNAL(clicked(bool)), this, SLOT(deleteImage2()));
}

DuplicatePairDialog::~DuplicatePairDialog()
{
	delete ui;
}

QString DuplicatePairDialog::imageToDelete()
{
	return img_;
}

void DuplicatePairDialog::deleteImage1()
{
	img_ = ui->name1->text();
	accept();
}

void DuplicatePairDialog::deleteImage2()
{
	img_ = ui->name2->text();
	accept();
}
