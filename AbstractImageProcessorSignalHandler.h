#pragma once

#include <QObject>
#include <QUuid>

namespace Ipt::Core {

	class AbstractImageProcessorSignalHandler : public QObject 
	{

		//Q_OBJECT

	public:
		AbstractImageProcessorSignalHandler(QUuid contextId): _contextId(contextId) {}
		virtual ~AbstractImageProcessorSignalHandler() {}

	public slots:

		virtual void handle_srcImageIsUpdated(QUuid contextId) = 0;
		virtual void handle_prcImageIsUpdated(QUuid contextId) = 0;
		virtual void handle_algorythmIsUpdated(QUuid contextId) = 0;

	private:
		QUuid _contextId;
		
	};

}
