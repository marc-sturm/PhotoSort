#ifndef FINGERPRINTWORKER_H
#define FINGERPRINTWORKER_H

#include "WorkerBase.h"
#include <QList>
#include <QColor>
#include <QPair>

struct FingerPrint
{
	QString filename;
	int w{0}, h{0}, r{0}, g{0}, b{0};

	void operator+=(const QColor& c)
	{
		r += c.red();
		g += c.green();
		b += c.blue();
	}

	QString toString()
	{
		return filename + " " + QString::number(w) + " " + QString::number(h) + " " + QString::number(r) + " " + QString::number(g) + " " + QString::number(b);
	}

	double diff(const FingerPrint& rhs)
	{
		//same width/height
		if ((w==rhs.w && h==rhs.h) || (w==rhs.h && h==rhs.w))
		{
			return std::min((abs(r-rhs.r) + abs(g-rhs.g) + abs(b-rhs.b)) / (2.0 * w + h), 100.0);
		}

		return 100.0;
	}
};

class FingerPrintWorker
		: public WorkerBase
{
	Q_OBJECT

public:
	FingerPrintWorker();
	void set(QStringList files, double max_diff);
	void process() override;

	QList<QPair<QString, QString>> duplicates() const;

signals:
	void message(QString);

private:
	QStringList files_;
	double max_diff_;
	QList<QPair<QString, QString>> duplicates_;
};

#endif // FINGERPRINTWORKER_H
