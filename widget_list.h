#pragma once

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QCheckBox>

class widget_list : public QWidget
{
	Q_OBJECT

	QVBoxLayout					*layout_main;

	QHBoxLayout					*layout_path;
	QHBoxLayout					*layout_sort;

	QLabel						*label_field_name;
	QLineEdit					*editbox_dir;
	QPushButton					*button_select;
	QPushButton					*button_sort;

	QCheckBox					*cb_autosort;

	QTableWidget				*table_files;
public:
	/*constructor*/				widget_list					(QWidget *parent = NULL);
	/*destructor*/				~widget_list				();

private slots:
	void						slot_scan					();
	void						slot_get_path				();
	void						slot_selection_change		();
	void						slot_sort					();

signals:
	void						signal_select				(QString path);
};
