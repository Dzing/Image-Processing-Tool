#pragma once

#include <QString>
#include <QVariant>
#include <QList>

#include "Option.h"

namespace Ipt::Core {

	class PARAM_VIDGET_TYPE {
	public:
		static const int SLIDER = 0;
		static const int ROUNDBUTTON_GROUP = 1;
	};

	class Parameter
	{
	public:
		
		Parameter() {}
		Parameter(int id, QString name) : _id(id), _name(name) {}
		~Parameter() { 
			for (auto it = _options.begin(); it != _options.end(); it++) 
				delete it.value();
		}

		QString name() { return _name; }
		QVariant minValue() { return _minValue; }
		QVariant maxValue() { return _maxValue; }
		QVariant defaultValue() { return _defaultValue; }
		int widgetType() { return _widgetType; }
		QVariant step() { return _step; }
		int id() { return _id; }

		void setName(QString val) { this->_name = val; }
		void setMinValue(QVariant val) { this->_minValue = val; }
		void setMaxValue(QVariant val) { this->_maxValue = val; }
		void setDefaultValue(QVariant val) { this->_defaultValue = val; }
		void setWidgetType(int val) { this->_widgetType = val; }
		void setStep(QVariant val) { this->_step = val; }

		void regOption(int id, Option* option) { _options[id] = option; }
		Option* getOption(int id) {
			return _options.value(id);
		}

		QList<AlgorythmOptionInfo>* getOptionsInfo() {
			auto data = new QList<AlgorythmOptionInfo>;
			for (auto it = _options.begin(); it != _options.end(); it++) {
				data->append(AlgorythmOptionInfo(it.value()));
			}
			return data;
		}

	private:
		int _id;
		QString _name;
		QVariant _minValue;
		QVariant _maxValue;
		QVariant _defaultValue;
		QVariant _step;

		int _widgetType = 0;

		QMap<int, Option*> _options;
	};

	struct AlgorythmParamInfo {
		int id = 0;
		int widgetType = 0;
		QString name = "";
		QVariant minValue = 0;
		QVariant maxValue = 0;
		QVariant defaultValue = 0;
		QVariant step = 0;
		QList<AlgorythmOptionInfo> *options;

		AlgorythmParamInfo() {}
		~AlgorythmParamInfo() {}
		AlgorythmParamInfo(int id, Parameter* src) {
			this->id = src->id();
			this->widgetType = src->widgetType();
			this->name = src->name();
			this->options = src->getOptionsInfo();
			this->minValue = src->minValue();
			this->maxValue = src->maxValue();
			this->defaultValue = src->defaultValue();
			this->step = src->step();
		}
	};
}

