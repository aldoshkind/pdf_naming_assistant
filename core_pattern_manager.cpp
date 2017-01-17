#include "core_pattern_manager.h"

#include <QStringList>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>

#include <QDomDocument>

#include <QDebug>

/*constructor*/ core_pattern_manager::core_pattern_manager(QObject *parent) : QObject(parent)
{
	ensure_settings_exists();

	deserialize();
	emit signal_pattern_set_changed();
}

/*destructor*/ core_pattern_manager::~core_pattern_manager()
{
	//
}

QStringList core_pattern_manager::get_pattern_names() const
{
	QStringList res;

	for(patterns_t::const_iterator it = patterns.begin() ; it != patterns.end() ; ++it)
	{
		res.append(it->first);
	}

	return res;
}

QString core_pattern_manager::get_pattern(QString name)
{
	if(patterns.find(name) != patterns.end())
	{
		return patterns[name];
	}

	return "";
}

bool core_pattern_manager::create_pattern(QString name)
{
	if(patterns.find(name) != patterns.end())
	{
		return false;
	}

	patterns[name] = "";

	serialize();

	emit signal_pattern_set_changed();
	return true;
}

void core_pattern_manager::remove_pattern(QString name)
{
	patterns.erase(name);
	serialize();
	emit signal_pattern_set_changed();
}

void core_pattern_manager::set_pattern(QString name, QString pattern)
{
	patterns[name] = pattern;
	serialize();
	emit signal_pattern_change(name);
}

void core_pattern_manager::deserialize()
{
	QString path = get_settings_path();

	QFile file(path);
	QDomDocument doc;
	if(file.open(QIODevice::ReadOnly))
	{
		doc.setContent(file.readAll());
		file.close();
	}
	else
	{
		return;
	}
	patterns.clear();

	QDomElement el = doc.firstChildElement("pattern_info");
	if(el.isNull() == true)
	{
		return;
	}

	QDomElement el_patterns = el.firstChildElement("patterns");
	if(el_patterns.isNull() == true)
	{
		return;
	}

	QDomNodeList children = el_patterns.childNodes();
	for(int i = 0 ; i < children.size() ; i += 1)
	{
		QDomElement p = children.at(i).toElement();
		patterns[p.attribute("name")] = p.attribute("pattern");
	}

	QDomElement el_current = el.firstChildElement("current_pattern");
	if(el_current.isNull() == true)
	{
		return;
	}
	set_current_pattern(el_current.attribute("name"));
}

void core_pattern_manager::serialize()
{
	QString path = get_settings_path();

	QDomDocument doc;
	QDomElement root = doc.createElement("pattern_info");
	doc.appendChild(root);
	QDomElement el_patterns = doc.createElement("patterns");
	root.appendChild(el_patterns);

	for(patterns_t::iterator it = patterns.begin() ; it != patterns.end() ; ++it)
	{
		QDomElement p = doc.createElement("pattern");
		p.setAttribute("name", it->first);
		p.setAttribute("pattern", it->second);
		el_patterns.appendChild(p);
	}

	QDomElement el_current = doc.createElement("current_pattern");
	root.appendChild(el_current);
	el_current.setAttribute("name", current_pattern);

	QDir().mkpath(QFileInfo(path).dir().absolutePath());
	QFile file(path);
	if(file.open(QIODevice::WriteOnly))
	{
		QTextStream out(&file);
		doc.save(out, 2);
		file.close();
	}
}

QString core_pattern_manager::get_settings_path()
{
	return QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + "/pdf_naming_assistant/patterns.xml";
}

void core_pattern_manager::ensure_settings_exists()
{
	QString path = get_settings_path();

	if(QFileInfo(path).exists() == false)
	{
		fill_default_settings();
		serialize();
	}
}

void core_pattern_manager::fill_default_settings()
{
	patterns["статья"] = "{author}# - #{article name}@ (@{year}#)#.%suffix%";
	patterns["книга"] = "{book name}@ (@{year}#)#.%suffix%";

	set_current_pattern(patterns.begin()->first);
}

QStringList core_pattern_manager::get_pattern_fields(QString pattern)
{
	QStringList fields;

	QRegularExpression re;
	re.setPattern("\\{(.*?)\\}");

	QRegularExpressionMatchIterator it = re.globalMatch(pattern);

	while(it.hasNext())
	{
		QRegularExpressionMatch match = it.next();
		fields.push_back(match.captured(1));
	}

	return fields;
}

void core_pattern_manager::set_current_pattern(QString name)
{
	if((patterns.find(name) == patterns.end()) && (patterns.size() != 0))
	{
		return;
	}
	if(patterns.size() == 0)
	{
		name = "";
	}
	current_pattern = name;
	serialize();
	emit signal_current_pattern_set(name);
}

QString core_pattern_manager::get_current_pattern()
{
	return current_pattern;
}
