#include "widget_settings.h"

#include <QInputDialog>
#include <QMessageBox>

/*constructor*/ widget_settings::widget_settings(core_pattern_manager *c, QWidget *parent) : QDialog(parent)
{
	cpm = c;

	layout_main = new QVBoxLayout(this);

	layout_management = new QHBoxLayout;

	combo_patterns = new QComboBox(this);
	label_name = new QLabel(this);
	button_add = new QPushButton("Add", this);
	button_remove = new QPushButton("Remove", this);

	buttons = new QDialogButtonBox(this);
	button_ok = new QPushButton(tr("OK"), this);
	buttons->addButton(button_ok, QDialogButtonBox::AcceptRole);

	lineedit_pattern = new QLineEdit(this);

	layout_management->addWidget(combo_patterns);
	layout_management->addWidget(label_name, 1);
	layout_management->addWidget(button_add);
	layout_management->addWidget(button_remove);

	layout_main->addLayout(layout_management);
	layout_main->addWidget(lineedit_pattern);
	layout_main->addStretch(1);
	layout_main->addWidget(buttons);

	resize(640, 480);

	connect(button_ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(button_add, SIGNAL(clicked()), this, SLOT(slot_create_pattern()));
	connect(button_remove, SIGNAL(clicked()), this, SLOT(slot_remove()));
	connect(combo_patterns, SIGNAL(activated(QString)), this, SLOT(slot_activate(QString)));
	connect(lineedit_pattern, SIGNAL(textEdited(QString)), this, SLOT(slot_pattern_change(QString)));
	connect(cpm, SIGNAL(signal_pattern_set_changed()), this, SLOT(slot_update()));
	connect(cpm, SIGNAL(signal_current_pattern_set(QString)), this, SLOT(slot_current_pattern_set(QString)));

	slot_update();
	slot_current_pattern_set(cpm->get_current_pattern());
}

/*destructor*/ widget_settings::~widget_settings()
{
	//
}

void widget_settings::slot_create_pattern()
{
	QString name = QInputDialog::getText(this, tr("Input new pattern name"), tr("Name: "));
	if(cpm->create_pattern(name) == false)
	{
		QMessageBox::critical(this, tr("Error"), tr("Unable to create pattern ") + name);
	}
}

void widget_settings::slot_update()
{
	QString last_selected = combo_patterns->currentText();
	int id = combo_patterns->currentIndex();

	QStringList names = cpm->get_pattern_names();
	combo_patterns->clear();
	combo_patterns->addItems(names);

	int last_id = combo_patterns->findText(last_selected);
	if(last_id != -1)
	{
		combo_patterns->setCurrentIndex(last_id);
	}

	slot_activate(combo_patterns->currentText());
}

void widget_settings::slot_remove()
{
	if(combo_patterns->count() < 1)
	{
		return;
	}
	QString name = combo_patterns->currentText();
	cpm->remove_pattern(name);
}

void widget_settings::slot_activate(QString name)
{
	label_name->setText(name);

	if(lineedit_pattern->text() != cpm->get_pattern(name))
	{
		lineedit_pattern->setText(cpm->get_pattern(name));
	}
}

void widget_settings::slot_pattern_change(QString text)
{
	cpm->set_pattern(combo_patterns->currentText(), text);
}

void widget_settings::slot_current_pattern_set(QString name)
{
	int id = combo_patterns->findText(name);
	if(id == -1)
	{
		return;
	}
	combo_patterns->setCurrentIndex(id);
	slot_activate(combo_patterns->currentText());
}
