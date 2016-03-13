#include "widget_main.h"

/*constructor*/ widget_main::widget_main(QWidget *parent) : QWidget(parent)
{
	cpm = new core_pattern_manager(this);

	layout_main = new QHBoxLayout(this);

	list = new widget_list(this);
	doc = new widget_document(cpm, this);

	constr = new widget_constructor(cpm, this);
	constr->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	layout_main->addWidget(list, 1);
	layout_main->addWidget(doc, 1);
	layout_main->addWidget(constr, 1);

	connect(list, SIGNAL(signal_select(QString)), doc, SLOT(slot_file_select(QString)));
	connect(list, SIGNAL(signal_select(QString)), constr, SLOT(slot_filename(QString)));
	connect(doc, SIGNAL(signal_item(QString,QString)), constr, SLOT(slot_add_item(QString,QString)));
	connect(constr, SIGNAL(signal_updated()), list, SLOT(slot_scan()));
	connect(doc, SIGNAL(signal_pattern_changed()), constr, SLOT(slot_pattern_changed()));

	resize(800, 600);
}

/*destructor*/ widget_main::~widget_main()
{
	//
}
