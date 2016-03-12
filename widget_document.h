#pragma once

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QRubberBand>

#include <poppler/qt5/poppler-qt5.h>

class widget_document : public QWidget
{
	Q_OBJECT

	QVBoxLayout					*layout_main;

	QHBoxLayout					*layout_control;

	QLabel						*label_image;
	QPushButton					*button_prev;
	QLabel						*label_current;
	QPushButton					*button_next;

	QRubberBand					*rubber;

	QPoint						rect_origin;

	QMenu						*menu_fields;
	typedef std::map<QString, QAction *>	actions_by_name_t;
	actions_by_name_t						actions_by_name;

	Poppler::Document			*document;
	Poppler::Page				*page;

	QString						document_path;
	unsigned int				current_page;

	double						scale;

	bool						eventFilter						(QObject *, QEvent *ev);
	void						load_document					(QString path);
	void						show_page						(unsigned int page_num);

	void						render_page						();

public:
	/*constructor*/				widget_document					(QWidget *parent = NULL);
	/*destructor*/				~widget_document				();

	void						add_category					(QString cat);

public slots:
	void						slot_file_select				(QString path);
	void						slot_menu_hide					();
	void						slot_next_page					();
	void						slot_prev_page					();
	void						slot_action_triggered			(QAction *);

signals:
	void						signal_item						(QString cat, QString value);
};
