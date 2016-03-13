#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "core_pattern_manager.h"

class widget_settings : public QDialog
{
	Q_OBJECT

	core_pattern_manager			*cpm;

	QVBoxLayout						*layout_main;
	QHBoxLayout						*layout_management;

	QComboBox						*combo_patterns;
	QPushButton						*button_add;
	QPushButton						*button_remove;
	QLabel							*label_name;
	QLineEdit						*lineedit_pattern;

	QDialogButtonBox				*buttons;
	QPushButton						*button_ok;

public:
	/*constructor*/					widget_settings					(core_pattern_manager *cpm, QWidget *parent = NULL);
	/*destructor*/					~widget_settings				();

private slots:
	void							slot_create_pattern				();
	void							slot_remove						();
	void							slot_activate					(QString name);
	void							slot_pattern_change				(QString text);
	void							slot_current_pattern_set		(QString name);

public slots:
	void							slot_update						();
};
