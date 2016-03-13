#pragma once

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QRubberBand>
#include <QComboBox>

#include <poppler/qt5/poppler-qt5.h>

#include "core_pattern_manager.h"

class widget_document : public QWidget
{
	Q_OBJECT

	core_pattern_manager		*cpm;

	QVBoxLayout					*layout_main;

	QHBoxLayout					*layout_control;

	QComboBox					*combo_current_pattern;

	QLabel						*label_image;
	QPushButton					*button_prev;
	QLabel						*label_current;
	QPushButton					*button_next;

	QRubberBand					*rubber;

	QPoint						rect_origin;

	QMenu						*menu_fields;
	QMenu						*menu_pats;
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

	void						add_category					(QString cat);
	void						clear_categories				();

public:
	/*constructor*/				widget_document					(core_pattern_manager *cpm, QWidget *parent = NULL);
	/*destructor*/				~widget_document				();

public slots:
	void						slot_file_select				(QString path);
	void						slot_menu_hide					();
	void						slot_next_page					();
	void						slot_prev_page					();
	void						slot_field_filled				(QAction *);
	void						slot_update_pats				();
	void						slot_pattern_selected			(QString name);
	void						slot_pattern_selected			(QAction *);

signals:
	void						signal_item						(QString cat, QString value);
};
