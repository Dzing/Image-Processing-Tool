#pragma once

#include <QString>
#include <QVariant>

namespace Ipt::Core {
	class Option
	{
	
	public:
		Option(int id, QString name, QVariant value) :_id(id), _name(name), _value(value) {}
		~Option() {}

		QString name() { return _name; }
		QVariant value() { return _value; }
		int id() { return _id; }

		void setName(QString name) { this->_name = name; }
		void setValue(QString value) { this->_value = value; }

	private:
		int _id;
		QString _name;
		QVariant _value;

	};

	struct AlgorythmOptionInfo {
		int id = 0;
		QString name;
		QVariant value;
		AlgorythmOptionInfo() {}
		AlgorythmOptionInfo(Option* src)
		{
			this->id = src->id();
			this->name = src->name();
			this->value = src->value();
		}
	};
}