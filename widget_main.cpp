#include "widget_main.h"

/*constructor*/ widget_main::widget_main(QWidget *parent) : QWidget(parent)
{
	layout_main = new QHBoxLayout(this);

	list = new widget_list(this);
	doc = new widget_document(this);
	doc->add_category("article name");
	doc->add_category("author");
	doc->add_category("year");

	constr = new widget_constructor(this);
	constr->set_pattern("{author}# - #{article name}@ (@{year}#)#.%suffix%");
	constr->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	layout_main->addWidget(list, 1);
	layout_main->addWidget(doc, 1);
	layout_main->addWidget(constr, 1);

	connect(list, SIGNAL(signal_select(QString)), doc, SLOT(slot_file_select(QString)));
	connect(list, SIGNAL(signal_select(QString)), constr, SLOT(slot_filename(QString)));
	connect(doc, SIGNAL(signal_item(QString,QString)), constr, SLOT(slot_add_item(QString,QString)));
	connect(constr, SIGNAL(signal_updated()), list, SLOT(slot_scan()));
}

/*destructor*/ widget_main::~widget_main()
{
	//
}
