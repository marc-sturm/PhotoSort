#include "ResizeDialog.h"


ResizeDialog::ResizeDialog(QWidget *parent)
	: QDialog(parent),
	  ui_()
{
	ui_.setupUi(this);
	connect(ui_.width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
	connect(ui_.height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
}

void ResizeDialog::setSize(int width, int height)
{
	ratio_w_h_ = (double)width/(double)height;

	ui_.height->blockSignals(true);
	ui_.width->blockSignals(true);
	ui_.width->setValue(width);
	ui_.height->setValue(height);
	ui_.height->blockSignals(false);
	ui_.width->blockSignals(false);
}

int ResizeDialog::width() const
{
	return ui_.width->value();
}

int ResizeDialog::height() const
{
	return ui_.height->value();
}

void ResizeDialog::widthChanged(int width)
{
	if (!ui_.keep_aspect_ratio->isChecked()) return;

	ui_.height->blockSignals(true);
	ui_.height->setValue((int)(width / ratio_w_h_));
	ui_.height->blockSignals(false);
}

void ResizeDialog::heightChanged(int height)
{
	if (!ui_.keep_aspect_ratio->isChecked()) return;

	ui_.width->blockSignals(true);
	ui_.width->setValue((int)(height * ratio_w_h_));
	ui_.width->blockSignals(false);
}
