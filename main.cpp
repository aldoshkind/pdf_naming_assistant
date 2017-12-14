#include "widget_main.h"
#include <QApplication>
#include <QDebug>

extern QString default_location;

int main(int argc, char *argv[])
{
	QString path;
	if(argc > 1)
	{
		char *p = realpath(argv[1], NULL);
		printf("%s\n", p);
		path = QString::fromUtf8(p);
		free(p);
	}

	QApplication a(argc, argv);
	widget_main w(path);
	w.show();

	return a.exec();
}
