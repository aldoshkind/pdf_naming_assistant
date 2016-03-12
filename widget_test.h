#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include <poppler/qt5/poppler-qt5.h>

class widget_test : public QWidget
{
	Q_OBJECT

	QVBoxLayout			*layout;
	QLabel				*label;

	Poppler::Document	*document;
	Poppler::Page		*page;

public:
	widget_test(QWidget *parent = 0);
	~widget_test();
};
