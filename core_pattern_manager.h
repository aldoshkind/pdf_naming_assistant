#pragma once

#include <map>

#include <QObject>

class core_pattern_manager : public QObject
{
	Q_OBJECT

	typedef std::map<QString, QString>			patterns_t;
	patterns_t									patterns;

	QString							current_pattern;

	void							ensure_settings_exists				();
	void							fill_default_settings				();

	QString							get_settings_path					();

	void							deserialize							();
	void							serialize							();

public:
	/*constructor*/					core_pattern_manager				(QObject *parent = NULL);
	/*destructor*/					~core_pattern_manager				();

	QStringList						get_pattern_names					() const;

	bool							create_pattern						(QString name);
	void							remove_pattern						(QString name);

	void							set_pattern							(QString name, QString pattern);
	QString							get_pattern							(QString name);

	void							set_current_pattern					(QString name);
	QString							get_current_pattern					();

	QStringList						get_pattern_fields					(QString pattern);

signals:
	void							signal_pattern_set_changed			();
	void							signal_pattern_change				(QString name);
	void							signal_current_pattern_set			(QString name);
};
