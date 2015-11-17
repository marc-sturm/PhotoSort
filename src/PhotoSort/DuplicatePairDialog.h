#ifndef DUPLICATEPAIRDIALOG_H
#define DUPLICATEPAIRDIALOG_H

#include <QDialog>

namespace Ui {
class DuplicatePairDialog;
}

class DuplicatePairDialog
	: public QDialog
{
	Q_OBJECT

public:
	explicit DuplicatePairDialog(QWidget* parent, QString file1, QString file2);
	~DuplicatePairDialog();
	QString imageToDelete();

private slots:
	void deleteImage1();
	void deleteImage2();

private:
	Ui::DuplicatePairDialog *ui;
	QString img_;
};

#endif // DUPLICATEPAIRDIALOG_H
