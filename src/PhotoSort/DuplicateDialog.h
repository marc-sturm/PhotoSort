#ifndef DUPLICATEDIALOG_H
#define DUPLICATEDIALOG_H

#include <QDialog>
#include "FingerPrintWorker.h"

namespace Ui {
class DuplicateDialog;
}

class DuplicateDialog : public QDialog
{
	Q_OBJECT

public:
	explicit DuplicateDialog(QWidget *parent = 0);
	void setFolder(QString folder);
	~DuplicateDialog();

private slots:
	void search();
	void workerFinished();
	void showMessage(QString message);
	void showPairs();
	void browse();

signals:
	void checkPairs();

private:
	Ui::DuplicateDialog *ui;
	FingerPrintWorker worker_;
	QList<QPair<QString, QString>>  dups_;
};

#endif // DUPLICATEDIALOG_H
