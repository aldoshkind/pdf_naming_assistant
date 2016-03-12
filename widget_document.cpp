#include "widget_document.h"

#include <QMenu>
#include <QEvent>
#include <QMouseEvent>
#include <QMessageBox>

/*constructor*/ widget_document::widget_document(QWidget *parent) : QWidget(parent)
{
	layout_main = new QVBoxLayout(this);

	label_image = new QLabel(this);
	label_image->installEventFilter(this);

	layout_control = new QHBoxLayout();

	button_next = new  QPushButton("next", this);
	button_prev = new  QPushButton("prev", this);
	label_current = new QLabel(this);

	layout_control->addWidget(button_prev);
	layout_control->addStretch(1);
	layout_control->addWidget(label_current);
	layout_control->addStretch(1);
	layout_control->addWidget(button_next);

	layout_main->addWidget(label_image);
	layout_main->addLayout(layout_control);

	rubber = new QRubberBand(QRubberBand::Rectangle, label_image);

	menu_fields = new QMenu(this);

	document = NULL;
	page = NULL;

	scale = 1.0;

	connect(menu_fields, SIGNAL(aboutToHide()), this, SLOT(slot_menu_hide()));
	connect(button_next, SIGNAL(clicked()), this, SLOT(slot_next_page()));
	connect(button_prev, SIGNAL(clicked()), this, SLOT(slot_prev_page()));
	connect(menu_fields, SIGNAL(triggered(QAction*)), this, SLOT(slot_action_triggered(QAction*)));
}

/*destructor*/ widget_document::~widget_document()
{
	//
}

void widget_document::slot_file_select(QString path)
{
	load_document(path);
	show_page(0);
}

void widget_document::slot_menu_hide()
{
	rubber->hide();
}

void widget_document::slot_next_page()
{
	show_page(current_page + 1);
}

void widget_document::slot_prev_page()
{
	show_page(current_page - 1);
}

bool widget_document::eventFilter(QObject *, QEvent *ev)
{
	if(ev->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mev = dynamic_cast<QMouseEvent *>(ev);
		if(mev == NULL)
		{
			return false;
		}
		rect_origin = mev->pos();
		rubber->setGeometry(QRect(rect_origin, QSize()));
		rubber->show();
	}

	if(ev->type() == QEvent::MouseMove)
	{
		QMouseEvent *mev = dynamic_cast<QMouseEvent*>(ev);
		if(mev == NULL)
		{
			return false;
		}
		rubber->setGeometry(QRect(rect_origin, mev->pos()).normalized());
	}
	if(ev->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mev = dynamic_cast<QMouseEvent*>(ev);
		if(mev == NULL)
		{
			return false;
		}
		if(rubber->rect().height() != 0 && rubber->rect().width() != 0)
		{
			menu_fields->popup(label_image->mapToGlobal(mev->pos()));
		}
	}
	return false;
}

void widget_document::load_document(QString path)
{
	if(document != NULL)
	{
		delete document;
		document = NULL;
	}

	document = Poppler::Document::load(path);

	if(document == NULL)
	{
		QMessageBox::critical(this, "Error", "Error opening file " + path);
		return;
	}

	document->setRenderHint(Poppler::Document::TextAntialiasing);

	document_path = path;

	current_page = 0;
}

void widget_document::show_page(unsigned int page_num)
{
	if(document == NULL)
	{
		return;
	}

	if(page_num >= document->numPages())
	{
		return;
	}

	if(page != NULL)
	{
		delete page;
		page = NULL;
	}

	current_page = page_num;
	page = document->page(page_num);
	if(page == NULL)
	{
		QMessageBox::critical(this, "Error", "Error reading page " + QString::number(page_num) + " of file " + document_path);
		return;
	}

	render_page();
}

void widget_document::slot_action_triggered(QAction *act)
{
	QRect r = rubber->geometry();

	int hor_diff = 0;
	int vert_diff = 0;

	QRect new_rect;
	new_rect.setX((r.x() - hor_diff) * scale);
	new_rect.setY((r.y() - vert_diff) * scale);
	new_rect.setWidth(r.width() * scale);
	new_rect.setHeight(r.height() * scale);

	QString text = page->text(new_rect);

	emit signal_item(act->text(), text);
}

void widget_document::add_category(QString cat)
{
	menu_fields->addAction(cat);
}

void widget_document::render_page()
{
	double dpi = 96;
	QImage image = page->renderToImage(dpi, dpi);

	if(image.isNull())
	{
		QMessageBox::critical(this, "Error", "Error rendering page " + QString::number(current_page) + " of file " + document_path);
		return;
	}

	QSize original_size = image.size();

	image = image.scaled(label_image->geometry().width(), label_image->geometry().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

	scale = (double)original_size.width() / (double)image.width();
	scale /= (dpi / 72.0);

	label_image->setPixmap(QPixmap::fromImage(image));
	label_image->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}
