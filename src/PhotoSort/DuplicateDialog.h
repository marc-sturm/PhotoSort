#ifndef DUPLICATEDIALOG_H
#define DUPLICATEDIALOG_H

#include <QDialog>

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

private:
	Ui::DuplicateDialog *ui;
};

#endif // DUPLICATEDIALOG_H
