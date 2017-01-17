#include "widget_list.h"

#include <QHeaderView>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

/*constructor*/ widget_list::widget_list(QWidget *parent) : QWidget(parent)
{
	layout_main = new QVBoxLayout(this);
	layout_main->setMargin(0);

	layout_path = new QHBoxLayout();
	layout_sort = new QHBoxLayout();

	QString default_location = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
	editbox_dir = new QLineEdit(default_location, this);
	//editbox_dir = new QLineEdit("/home/dmitry/documents/литература/всё вместе/", this);
	button_select = new QPushButton("...", this);
	button_sort = new QPushButton("sort", this);
	cb_autosort = new QCheckBox("Show sorted", this);
	cb_autosort->setChecked(true);

	layout_path->addWidget(editbox_dir, 1);
	layout_path->addWidget(button_select);

	table_files = new QTableWidget(this);

	table_files->setColumnCount(1);
	table_files->horizontalHeader()->setVisible(false);
	table_files->verticalHeader()->setVisible(false);
	table_files->insertRow(0);
	table_files->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	table_files->setSelectionMode(QAbstractItemView::SingleSelection);
	table_files->setRowCount(0);

	layout_sort->addWidget(cb_autosort);
	layout_sort->addWidget(button_sort);

	layout_main->addLayout(layout_path);
	layout_main->addWidget(table_files);
	layout_main->addLayout(layout_sort);

	connect(editbox_dir, SIGNAL(returnPressed()), this, SLOT(slot_scan()));
	connect(button_select, SIGNAL(clicked()), this, SLOT(slot_get_path()));
	connect(table_files, SIGNAL(itemSelectionChanged()), this, SLOT(slot_selection_change()));
	connect(button_sort, SIGNAL(clicked()), this, SLOT(slot_sort()));
}

/*destructor*/ widget_list::~widget_list()
{
	//
}

void widget_list::slot_scan()
{
	int selected_row = 0;
	if(table_files->rowCount() > 0)
	{
		selected_row = table_files->currentRow();
	}

	QString path = editbox_dir->text();
	if(QDir(path).exists() == false)
	{
		QMessageBox::critical(this, "Error", QString("Directory ") + path + " does not exist");
		return;
	}

	QDirIterator dir_item(path);

	table_files->setRowCount(0);
	table_files->clear();

	for( ; dir_item.hasNext() ; )
	{
		QString item = dir_item.next();

		QFileInfo item_info(item);
		QString suffix = item_info.suffix();
		if(suffix != "pdf")
		{
			continue;
		}
		table_files->insertRow(0);
		QTableWidgetItem *table_item = new QTableWidgetItem(item_info.completeBaseName());
		table_item->setData(Qt::UserRole, item);
		table_files->setItem(0, 0, table_item);
	}

	if(cb_autosort->isChecked())
	{
		slot_sort();
	}

	table_files->resizeRowsToContents();

	if((selected_row > -1) && (table_files->rowCount() > selected_row))
	{
		table_files->setCurrentCell(selected_row, 0);
	}
}

void widget_list::slot_get_path()
{
	QString path = QFileDialog::getExistingDirectory(this, "Select dir to scan", editbox_dir->text());

	if(path.size() == 0)
	{
		return;
	}

	editbox_dir->setText(path);

	slot_scan();
}

void widget_list::slot_selection_change()
{
	int row = table_files->currentRow();
	if(row < 0)
	{
		return;
	}

	QString file = table_files->item(row, 0)->data(Qt::UserRole).toString();
	emit signal_select(file);
}

void widget_list::slot_sort()
{
	table_files->sortItems(0);
}
