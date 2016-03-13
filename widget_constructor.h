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

#include "widget_settings.h"
#include "core_pattern_manager.h"

class widget_constructor : public QWidget
{
	Q_OBJECT

	core_pattern_manager	*cpm;

	QString					pattern;
	QString					filename;
	QString					result;

	QVBoxLayout				*layout_main;
	QVBoxLayout				*layout_result;
	QHBoxLayout				*layout_result_name;
	QTableWidget			*table_parts;
	QLabel					*label_preview;
	QPushButton				*button_apply;
	QPushButton				*button_settings;

	widget_settings			*settings;

	std::map<QString, QString>					cat_to_text;
	std::map<QString, QTableWidgetItem *>		cat_to_item;

	void					clean						(QString &str);
	void					capitalize					(QString &str);
	void					construct					();

public:
	/*constructor*/			widget_constructor			(core_pattern_manager *cpm, QWidget *parent = 0);
	/*destructor*/			~widget_constructor			();

	void					reset						();

public slots:
	void					slot_add_item				(QString cat, QString text);
	void					slot_filename				(QString name);
	void					slot_item_change			(int row, int col);
	void					slot_apply					();
	void					slot_patterns_updated		();
	void					slot_pattern_changed		(QString name);

signals:
	void					signal_updated				();
};
