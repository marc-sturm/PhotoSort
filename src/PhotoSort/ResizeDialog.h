#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>
#include "ui_ResizeDialog.h"

class ResizeDialog
	: public QDialog
{
	Q_OBJECT

public:
	ResizeDialog(QWidget* parent = 0);
	void setSize(int width, int height);
	int width() const;
	int height() const;

private slots:
	void widthChanged(int width);
	void heightChanged(int height);

private:
	Ui::ResizeDialog ui_;
	double ratio_w_h_;
};

#endif
