#pragma once

#include <QVariant>
#include <QImage>

#include "Parameter.h"

namespace Ipt::Core {

	class AbstractAlgorythm {

	public:
		AbstractAlgorythm() {}
		AbstractAlgorythm(QString name) : _name(name) {}

		virtual QImage* execute(QImage* src, QMap<int, QVariant>* params) = 0;


		QString name() { return _name; }

		void setName(QString name) { this->_name = name; }
		void regParameter(int id, Parameter* param) { _parameters[id] = param; }

		
		Parameter* findParameter(int id) { return _parameters[id]; }

		QList<AlgorythmParamInfo>* getParamInfo() {
			auto data = new QList<AlgorythmParamInfo>();
			for (auto it = _parameters.begin(); it != _parameters.end(); it++) {
				data->append(AlgorythmParamInfo(it.key(), it.value()));
			}
			return data;
		}

	protected:
		QString _name;

	private:

		QMap<int, Parameter*> _parameters;

	};
}
