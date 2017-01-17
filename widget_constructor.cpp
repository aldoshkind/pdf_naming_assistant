#include "widget_constructor.h"

#include <QHeaderView>
#include <QRegularExpression>

/*constructor*/ widget_constructor::widget_constructor(core_pattern_manager *c, QWidget *parent) : QWidget(parent)
{
	cpm = c;

	layout_main = new QVBoxLayout(this);

	layout_result = new QVBoxLayout;
	layout_result_name = new QHBoxLayout;

	table_parts = new QTableWidget(this);
	table_parts->setWordWrap(true);
	table_parts->setColumnCount(2);
	table_parts->verticalHeader()->setVisible(false);
	table_parts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table_parts->setSelectionBehavior(QAbstractItemView::SelectRows);

	label_preview = new QLabel(this);
	label_preview->setWordWrap(true);
	button_apply = new QPushButton(tr("Apply"));

	button_settings = new QPushButton(tr("Settings"));

	settings = new widget_settings(cpm, this);

	layout_result_name->addWidget(label_preview);
	layout_result_name->addWidget(button_apply);

	layout_result->addLayout(layout_result_name);
	layout_result->addStretch(1);

	layout_main->addWidget(table_parts, 1);
	layout_main->addLayout(layout_result, 1);
	layout_main->addStretch(1);
	layout_main->addWidget(button_settings, 0, Qt::AlignRight);

	connect(table_parts, SIGNAL(cellChanged(int,int)), this, SLOT(slot_item_change(int,int)));
	connect(button_apply, SIGNAL(clicked()), this, SLOT(slot_apply()));

	connect(button_settings, SIGNAL(clicked()), settings, SLOT(exec()));
	connect(cpm, SIGNAL(signal_current_pattern_set(QString)), this, SLOT(slot_pattern_changed(QString)));
	connect(cpm, SIGNAL(signal_pattern_change(QString)), this, SLOT(slot_pattern_changed(QString)));

	slot_current_pattern_set(cpm->get_current_pattern());
}

/*destructor*/ widget_constructor::~widget_constructor()
{
	//
}

void widget_constructor::clean(QString &str)
{
	str.replace("/", "-");
	str.replace(QRegExp("-$"), "");

	QStringList parts = str.split(QRegExp("[\\s+]"), QString::SkipEmptyParts);

	str = parts.join(" ");
}

void widget_constructor::capitalize(QString &str)
{
	QStringList parts = str.split(QRegExp("[\\s+]"), QString::SkipEmptyParts);
	for(int i = 0 ; i < parts.size() ; i += 1)
	{
		parts[i] = parts[i].toLower();
		parts[i][0] = parts[i][0].toUpper();
	}
	str = parts.join(" ");
}

void widget_constructor::construct()
{
	result = pattern;

	for(std::map<QString, QString>::iterator it = cat_to_text.begin() ; it != cat_to_text.end() ; ++it)
	{
		result.replace("{" + it->first + "}", it->second);
	}

	result.replace("%suffix%", QFileInfo(filename).suffix());

	QRegularExpression re;
	re.setPattern("@.*?@\\{.*?\\}#.*?#");
	result.replace(re, "");
	re.setPattern("@.*?@\\{.*?\\}");
	result.replace(re, "");
	re.setPattern("\\{.*?\\}#.*?#");
	result.replace(re, "");
	re.setPattern("\\{.*?\\}");
	result.replace(re, "");
	re.setPattern("#(.*?)#");
	result.replace(re, "\\1");
	re.setPattern("@(.*?)@");
	result.replace(re, "\\1");

	label_preview->setText(result);
}

void widget_constructor::reset()
{
	table_parts->clear();
	table_parts->setRowCount(0);
	cat_to_item.clear();
	cat_to_text.clear();
}

void widget_constructor::slot_add_item(QString cat, QString text)
{
	clean(text);
	capitalize(text);
	cat_to_text[cat] = text;

	if(cat_to_item.find(cat) != cat_to_item.end())
	{
		cat_to_item[cat]->setText(text);
	}
	else
	{
		table_parts->insertRow(table_parts->rowCount());
		table_parts->setItem(table_parts->rowCount() - 1, 0, new QTableWidgetItem(cat));
		table_parts->item(table_parts->rowCount() - 1, 0)->setFlags(Qt::ItemIsSelectable);
		cat_to_item[cat] = new QTableWidgetItem(text);
		table_parts->setItem(table_parts->rowCount() - 1, 1, cat_to_item[cat]);
	}
	construct();
}

void widget_constructor::slot_filename(QString name)
{
	filename = name;
	reset();
}

void widget_constructor::slot_item_change(int row, int col)
{
	if((col >= table_parts->columnCount()) || (row >= table_parts->rowCount()) || (row < 0) || (col < 0))
	{
		return;
	}

	if(col < 1)
	{
		return;
	}

	QString cat = table_parts->item(row, col - 1)->text();
	QString text = table_parts->item(row, col)->text();
	capitalize(text);
	cat_to_text[cat] = text;
	construct();
}

void widget_constructor::slot_apply()
{
	QFile file(filename);
	QFileInfo info(filename);
	if(file.exists())
	{
		QString full_result = info.absoluteDir().absolutePath() + "/" + result;

		if(full_result == filename)
		{
			return;
		}

		bool renamed = file.rename(full_result);
		if(renamed == false)
		{
			QMessageBox::critical(this, "Error", "Renaming\n\n" + filename + "\n\nto\n\n" + full_result + "\n\nfailed." + (QFileInfo(full_result).exists() ? "\nFile already exists." : ""));
		}
		else
		{
			emit signal_updated();
		}
	}
	else
	{
		QMessageBox::critical(this, "Error", "File not found");
	}
}

void widget_constructor::slot_current_pattern_set(QString name)
{
	pattern = cpm->get_pattern(name);
	construct();
}

void widget_constructor::slot_pattern_changed(QString name)
{
	if(name == cpm->get_current_pattern())
	{
		slot_current_pattern_set(name);
	}
}
