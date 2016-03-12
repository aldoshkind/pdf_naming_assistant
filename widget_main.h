#pragma once

#include "widget_list.h"
#include "widget_document.h"
#include "widget_constructor.h"

class widget_main : public QWidget
{
	Q_OBJECT

	QHBoxLayout				*layout_main;

	widget_list				*list;
	widget_document			*doc;
	widget_constructor		*constr;

public:
	/*constructor*/			widget_main			(QWidget *parent = 0);
	/*destructor*/			~widget_main		();
};
