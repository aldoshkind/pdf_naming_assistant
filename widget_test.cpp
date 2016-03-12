#include "widget_test.h"

#include <QDebug>

widget_test::widget_test(QWidget *parent)
	: QWidget(parent)
{
	layout = new QVBoxLayout(this);

	label = new QLabel(this);

	layout->addWidget(label);

	document = Poppler::Document::load("/home/dmitry/downloads/browser/ImageGradients.pdf");
	document->setRenderHint(Poppler::Document::TextAntialiasing);

	if(document == 0)
	{
	  return;
	}

	page = document->page(0);
	if (page == 0)
	{
		return;
	}

	Poppler::HighlightAnnotation ann;

	Poppler::HighlightAnnotation::Quad q;
	q.points[0] = QPointF(0, 0);
	q.points[1] = QPointF(0.1, 0);
	q.points[2] = QPointF(0.1, 0.1);
	q.points[3] = QPointF(0, 0.1);

	q.capStart = false;
	q.capEnd = false;

	ann.setHighlightQuads(QList<Poppler::HighlightAnnotation::Quad>() << q);
	ann.setHighlightType(Poppler::HighlightAnnotation::Highlight);
	ann.setBoundary(QRectF(0,0,256,256));
	page->addAnnotation(&ann);

	QImage image = page->renderToImage(96, 96);

	QString tx = page->text(QRectF(0, 0, 256, 256));

	qDebug() << tx;

	label->setPixmap(QPixmap::fromImage(image));
	if (image.isNull())
	{
	  return;
	}
}

widget_test::~widget_test()
{
	delete page;
	delete document;
}
