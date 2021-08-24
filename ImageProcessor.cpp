#include "ImageProcessor.h"

#include "AlgorythmContrast.h"
#include "AlgorythmGradient.h"

#include <QtConcurrent>

using namespace Ipt::Core;

ImageProcessor* ImageProcessor::_instance = nullptr;

void ImageProcessor::regSignalHandler(QUuid contextId, AbstractImageProcessorSignalHandler* signalHandler)
{
	_context[contextId]->signalHandler = signalHandler;

	connect(this, SIGNAL(srcImageIsUpdated(QUuid)), signalHandler, SLOT(handle_srcImageIsUpdated(QUuid)));
	connect(this, SIGNAL(prcImageIsUpdated(QUuid)), signalHandler, SLOT(handle_prcImageIsUpdated(QUuid)));
	connect(this, SIGNAL(algorythmIsUpdated(QUuid)), signalHandler, SLOT(handle_algorythmIsUpdated(QUuid)));
}

ImageProcessor::ImageProcessor()
{
	regAlgorythm((AbstractAlgorythm*)(new Ipt::Logic::AlgorythmContrast()));
	regAlgorythm((AbstractAlgorythm*)(new Ipt::Logic::AlgorythmGradient()));
}

ImageProcessor::~ImageProcessor()
{
}

ImageProcessor* ImageProcessor::getInstance()
{
	if (!_instance)
		_instance = new ImageProcessor();
	return _instance;
}

QList<AlgorythmInfo>* ImageProcessor::getAlgorythmsInfo()
{
	QList<AlgorythmInfo> *result = new QList<AlgorythmInfo>();

	 for (auto it = _algorythms.begin(); it != _algorythms.end(); it++) {
		 AlgorythmInfo info = { it.key(), it.value()->name() };
		 result->append(info);
	 }

	 return result;
}

QList<AlgorythmParamInfo>* ImageProcessor::getAlgorythmParamInfo(QUuid contextId)
{
	return _algorythms[_context[contextId]->algorythmId]->getParamInfo();
}


// context

QUuid ImageProcessor::createContext()
{
	QUuid newId = QUuid::createUuid();
	ProcessorContext* newContext = new ProcessorContext();
	_context[newId] = newContext;
	newContext->watcherSignalHandler = new WatcherSignalHandler(newId);

	connect(
		&(newContext->watcher), SIGNAL(finished()), newContext->watcherSignalHandler, SLOT(handle_finished())
	);

	connect(
		newContext->watcherSignalHandler, SIGNAL(processingFinished(QUuid)), this, SLOT(handle_processingFinished(QUuid))
	);

	return newId;
}

void ImageProcessor::setNewSrcImage(QUuid contextId, QImage* image)
{
	_context[contextId]->srcImage = image;
	_context[contextId]->prcImage = new QImage(*image);

	emit srcImageIsUpdated(contextId);
	emit prcImageIsUpdated(contextId);
}

QImage* ImageProcessor::getProcessedImage(QUuid contextId)
{
	return _context[contextId]->prcImage;
}

void ImageProcessor::requestProcessing(QUuid contextId)
{

	ProcessorContext* ctx = _context[contextId];

	if (ctx->isProcessingLaunched) return;

	ctx->isProcessingLaunched = true;

	auto job = [=](ProcessorContext* ctx) {
		QImage* newImage = _algorythms[ctx->algorythmId]->execute(ctx->prcImage, ctx->parameters);
		delete ctx->prcImage;
		ctx->prcImage = newImage;
	};
	// это в другой поток!
	/*QImage * newImage =_algorythms[ctx->algorythmId]->execute(ctx->prcImage, ctx->parameters);
	delete _context[contextId]->prcImage;
	_context[contextId]->prcImage = newImage;*/
	// это в другой поток!


	QFuture<void> future = QtConcurrent::run(job, ctx);
	ctx->watcher.setFuture(future);
	
}



void ImageProcessor::handle_processingFinished(QUuid contextId)
{
	emit prcImageIsUpdated(contextId);
	_context[contextId]->isProcessingLaunched = false;
}

void ImageProcessor::resetProcessedImage(QUuid contextId)
{
	delete _context[contextId]->prcImage;
	_context[contextId]->prcImage = new QImage(*(_context[contextId]->srcImage));

	emit prcImageIsUpdated(contextId);
}

QImage* ImageProcessor::getSourceImage(QUuid contextId)
{
	return _context[contextId]->srcImage;
}

void ImageProcessor::selectAlgorythm(QUuid contextId, int algorythmId)
{
	auto ctx = _context[contextId];
	ctx->algorythmId = algorythmId;
	ctx->parameters->clear();

	emit algorythmIsUpdated(contextId);
}

void ImageProcessor::setAlgorythmParameterValue(QUuid contextId, int paramId, QVariant value)
{
	_context[contextId]->parameters->insert(paramId, value);
}

void ImageProcessor::pickAlgorythmOptionValue(QUuid contextId, int paramId, int variantId)
{
	auto ctx = _context[contextId];
	
	auto algInfo = _algorythms[ctx->algorythmId];
	auto algParam = algInfo->findParameter(paramId);
	auto option = algParam->getOption(variantId);
	ctx->parameters->insert(paramId, option->value());

}
