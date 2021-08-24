#pragma once

#include "AbstractAlgorythm.h"
#include "AbstractImageProcessorSignalHandler.h"

#include <QObject>
#include <QMap>
#include <QList>
#include <QUuid>
#include <QImage>
#include <QVariant>
#include <QFutureWatcher>

// менеджер модели данных приложения

namespace Ipt::Core {

	struct AlgorythmInfo;
	struct ProcessorContext;
	class WatcherSignalHandler;

	class ImageProcessor : public QObject
	{
		
		Q_OBJECT

	private:
		ImageProcessor();
		~ImageProcessor();

	public:

		static ImageProcessor* getInstance();
		static void freeInstance() { delete _instance;  _instance = nullptr; }

		QUuid createContext();

		void requestProcessing(QUuid contextId);

		QList<AlgorythmInfo>* getAlgorythmsInfo();
		QList<AlgorythmParamInfo>* getAlgorythmParamInfo(QUuid contextId);

		// context setters
		void regSignalHandler(QUuid contextId, AbstractImageProcessorSignalHandler* signalHandler);
		void setNewSrcImage(QUuid contextId, QImage* pixmap);
		void selectAlgorythm(QUuid contextId, int algorythmId);
		void setAlgorythmParameterValue(QUuid contextId, int paramId, QVariant value);
		void pickAlgorythmOptionValue(QUuid contextId, int paramId, int variantId);	
		// context getters
		QImage* getSourceImage(QUuid contextId);
		QImage* getProcessedImage(QUuid contextId);
		//
		
		void resetProcessedImage(QUuid contextId);

	public slots:
		void handle_processingFinished(QUuid contextId);

	signals:
		void srcImageIsUpdated(QUuid contextId);
		void prcImageIsUpdated(QUuid contextId);
		void algorythmIsUpdated(QUuid contextId);



	private:											
	
		void regAlgorythm(AbstractAlgorythm* alg) {	_algorythms[_algorythmCount++] = alg; }

		//
		
		static ImageProcessor* _instance;

		QMap <int, AbstractAlgorythm*> _algorythms;
		QMap <QUuid, ProcessorContext*> _context;

		int _algorythmCount = 0;

	};

	struct ProcessorContext {
		AbstractImageProcessorSignalHandler* signalHandler;
		int algorythmId = 0;
		QImage* srcImage;
		QImage* prcImage;
		QMap<int, QVariant>* parameters = new QMap<int, QVariant>();			// ид параметра алгоритма - значение параметра
		QFutureWatcher<void> watcher;
		WatcherSignalHandler * watcherSignalHandler;
		bool isProcessingLaunched = false;
		ProcessorContext() {}
		~ProcessorContext() { delete parameters; parameters = 0; }
	};

	struct AlgorythmInfo {
		int id;
		QString name;
	};

	class WatcherSignalHandler : public QObject
	{
		Q_OBJECT

	public:

		WatcherSignalHandler(QUuid contextId) : _contextId(contextId) {}
		~WatcherSignalHandler() {}

	public slots:
		void handle_finished() { emit  processingFinished(_contextId); }

	signals:
		void processingFinished(QUuid);


	private:
		QUuid _contextId;

	};

	

	

}

