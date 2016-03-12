#pragma once

#include <map>

#include <QFile>
#include <QDir>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileInfo>
#include <QPushButton>
#include <QMessageBox>

class widget_constructor : public QWidget
{
	Q_OBJECT

	QString					pattern;
	QString					filename;
	QString					result;

	QVBoxLayout				*layout_main;
	QVBoxLayout				*layout_result;
	QHBoxLayout				*layout_result_name;
	QTableWidget			*table_parts;
	QLabel					*label_preview;
	QPushButton				*button_apply;

	std::map<QString, QString>					cat_to_text;
	std::map<QString, QTableWidgetItem *>		cat_to_item;

	void					clean						(QString &str);
	void					capitalize					(QString &str);
	void					construct					();

public:
	/*constructor*/			widget_constructor			(QWidget *parent = 0);
	/*destructor*/			~widget_constructor			();

	void					set_pattern					(QString p);
	void					reset						();

public slots:
	void					slot_add_item				(QString cat, QString text);
	void					slot_filename				(QString name);
	void					slot_item_change			(int row, int col);
	void					slot_apply					();

signals:
	void					signal_updated				();
};
