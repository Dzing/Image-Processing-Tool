#pragma once

#include <QtWidgets>
#include <QMainWindow>
#include <qgraphicsview>

#include "ImageProcessor.h"
#include "AbstractImageProcessorSignalHandler.h"

namespace Ipt::Gui {

	class MainWindow : public QMainWindow
	{

		Q_OBJECT
		friend class MainWindowProcessorSignalHandler;

	public:
		MainWindow(QWidget* parent = nullptr);
		~MainWindow();

		QUuid contextId() { return _contextId;  }

		void rebuildOptionsArea();
		void showSrcImage();
		void showPrcImage();

	signals:
		void ready();
		void requestImageProcessing(QPixmap*);
		void requestAlgorythmMetdataList();

	private slots:
		
		void handle_actionLoadFile();
		void handle_eventExecuteButtonClicked();				// СЛОТ-обработчик нажатия кнопки выполнения обработки
		void handle_eventResetButtonClicked();					// СЛОТ-обработчик нажатия кнопки сброса обрабатываемого изображения к базовому
		void handle_actionChangeAlgorythm();					// СЛОТ-обработчик изменения значения в комбо-боксе списка алгоритмов

		void handle_eventGenericSliderValueChanged(int val);
		void handle_eventGenericButtongroupValueChanged();
		void handle_ready();

	private:

		Ipt::Core::ImageProcessor* _imageProcessor;
		QUuid _contextId;

		QGraphicsScene* _graphSceneSrc;
		QGraphicsScene* _graphScenePrc;
		QGraphicsView* _wSrcGraphicsView;
		QGraphicsView* _wPrcGraphicsView;

		QComboBox* _algorythmComboBox;
		QWidget* _optionsParent;

		//

		void initUi();
		void initImageProcessor();

		QWidget* buildImageViewArea();
		QWidget* buildControlsArea();

		QWidget* buildParameterWidget(Ipt::Core::AlgorythmParamInfo);
		QWidget* buildParameterSliderWidget(Ipt::Core::AlgorythmParamInfo);
		QWidget* buildParameterRounbuttonGroupWidget(Ipt::Core::AlgorythmParamInfo);

		void initAlgorythmSelector();
		void createMainMenu();
		
		bool loadFile(const QString& fileName);
		
		
		//
		void resizeEvent(QResizeEvent* event);
		void showEvent(QShowEvent* event);

		QMap<QObject*, int> _genericControlsParameterMapping;
	};

	class MainWindowProcessorSignalHandler : public Ipt::Core::AbstractImageProcessorSignalHandler
	{
		Q_OBJECT

	public:
		MainWindowProcessorSignalHandler(QUuid contextId, MainWindow* owner) : Ipt::Core::AbstractImageProcessorSignalHandler(contextId), _owner(owner) {}
		~MainWindowProcessorSignalHandler() {}

	public slots:

		void handle_prcImageIsUpdated(QUuid contextId) { if (_owner->contextId() == contextId) _owner->showPrcImage(); }
		void handle_srcImageIsUpdated(QUuid contextId) { if (_owner->contextId() == contextId) _owner->showSrcImage(); }
		void handle_algorythmIsUpdated(QUuid contextId) { if (_owner->contextId() == contextId) _owner->rebuildOptionsArea(); }

	private:
		MainWindow* _owner;

	};

	

}

