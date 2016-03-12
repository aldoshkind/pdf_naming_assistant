#include "widget_main.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	widget_main w;
	w.show();

	return a.exec();
}
