#include "MainWindow.h"
#include "GuiUtils.h"

#include <QApplication>
#include <QScreen>


using namespace Ipt::Gui;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), 
    _graphSceneSrc(new QGraphicsScene()),
    _graphScenePrc(new QGraphicsScene())
{

    initImageProcessor();

    initUi();
    resize(QGuiApplication::primaryScreen()->availableSize()*4/5);
    initAlgorythmSelector();

    connect(
        this, SIGNAL(ready()), this, SLOT(handle_ready())
    );
  
}

MainWindow::~MainWindow()
{

    _imageProcessor->freeInstance();

}

void MainWindow::handle_actionLoadFile()
{
    QFileDialog dialog(this, "Open File");
    GuiUtils::initImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}

void MainWindow::handle_eventExecuteButtonClicked()
{
    _imageProcessor->requestProcessing(_contextId);
}

void MainWindow::handle_eventResetButtonClicked()
{
    _imageProcessor->resetProcessedImage(_contextId);
}

void MainWindow::handle_actionChangeAlgorythm()
{
    
    int idAlgorythm = _algorythmComboBox->currentData().toInt();
    _imageProcessor->selectAlgorythm(_contextId, idAlgorythm);

}

void MainWindow::handle_eventGenericSliderValueChanged(int val)
{
    
    QSlider* obj = (QSlider *)sender();
    int paramId = _genericControlsParameterMapping[obj];
    
    _imageProcessor->setAlgorythmParameterValue(_contextId, paramId, val);
  
}

void MainWindow::handle_eventGenericButtongroupValueChanged()
{
    QButtonGroup* obj = (QButtonGroup*)sender();
    int paramId = _genericControlsParameterMapping[obj];
    _imageProcessor->pickAlgorythmOptionValue(_contextId, paramId, obj->checkedId());
   
}

void Ipt::Gui::MainWindow::handle_ready()
{
    _imageProcessor->setNewSrcImage(_contextId, new QImage(":/img/res/src.png"));
}

QWidget* MainWindow::buildImageViewArea() {

    auto graphicContent = new QWidget();

    auto graphicLayout = new QHBoxLayout;
    graphicLayout->setAlignment(Qt::AlignLeft);
    graphicContent->setLayout(graphicLayout);

    auto srcImageCardcontent = new QWidget();
    graphicLayout->addWidget(srcImageCardcontent);

    auto srcImageCardLayout = new QVBoxLayout;
    srcImageCardLayout->setAlignment(Qt::AlignTop);
    srcImageCardcontent->setLayout(srcImageCardLayout);

    auto srcImgLabel = new QLabel("Source image");
    srcImgLabel->setMaximumHeight(24);
    srcImgLabel->setFrameStyle(1);
    srcImgLabel->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
    srcImageCardLayout->addWidget(srcImgLabel);

    this->_wSrcGraphicsView = new QGraphicsView();
    this->_wSrcGraphicsView->setScene(this->_graphSceneSrc);
    this->_wSrcGraphicsView->setAlignment(Qt::AlignTop);
    this->_wSrcGraphicsView->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
    srcImageCardLayout->addWidget(this->_wSrcGraphicsView);

    auto prcImageCardcontent = new QWidget();
    graphicLayout->addWidget(prcImageCardcontent);

    auto prcImageCardLayout = new QVBoxLayout;
    prcImageCardLayout->setAlignment(Qt::AlignTop);
    prcImageCardcontent->setLayout(prcImageCardLayout);

    auto prcImgLabel = new QLabel("Processed image");
    prcImgLabel->setMaximumHeight(24);
    prcImgLabel->setFrameStyle(1);
    prcImgLabel->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
    prcImageCardLayout->addWidget(prcImgLabel);

    this->_wPrcGraphicsView = new QGraphicsView();
    this->_wPrcGraphicsView->setScene(this->_graphScenePrc);
    this->_wPrcGraphicsView->setAlignment(Qt::AlignTop);
    this->_wPrcGraphicsView->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
    prcImageCardLayout->addWidget(this->_wPrcGraphicsView);

    return (QWidget*)graphicContent;
}

QWidget* MainWindow::buildControlsArea()
{
    auto controlsContent = new QGroupBox("Settings");
    controlsContent->setMinimumHeight(100);
    controlsContent->setLayout(new QHBoxLayout());

        auto settings = new QWidget();
        controlsContent->layout()->addWidget(settings);
        settings->setMinimumHeight(60);
        settings->setLayout(new QHBoxLayout());
        settings->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);

            auto algorythmLine = new QWidget();
            settings->layout()->addWidget(algorythmLine);
            algorythmLine->setLayout(new QHBoxLayout());
            algorythmLine->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

                auto algorythmlabel = new QLabel("Algorythm:");
                algorythmLine->layout()->addWidget(algorythmlabel);
                algorythmlabel->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
                
                this->_algorythmComboBox = new QComboBox();
                algorythmLine->layout()->addWidget(_algorythmComboBox);
                _algorythmComboBox->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
                _algorythmComboBox->setFixedHeight(24);
                _algorythmComboBox->setFixedWidth(200);
            
                connect(
                    _algorythmComboBox, SIGNAL(textActivated(const QString &)), this, SLOT(handle_actionChangeAlgorythm())
                );

            this->_optionsParent = new QWidget();
            settings->layout()->addWidget(_optionsParent);
            settings->setMinimumHeight(20);
            _optionsParent->setLayout(new QHBoxLayout());
            _optionsParent->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);

        auto actions = new QWidget();
        controlsContent->layout()->addWidget(actions);
        actions->setMinimumHeight(24);
        actions->setLayout(new QVBoxLayout());
        actions->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Preferred);

            auto btExecute = new QPushButton("Execute");
            actions->layout()->addWidget(btExecute);
            btExecute->setMaximumWidth(100);
            btExecute->setMinimumHeight(40);
            btExecute->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
            connect(
                btExecute, SIGNAL(released()), this, SLOT(handle_eventExecuteButtonClicked())
            );


            auto btReset = new QPushButton("Reset");
            actions->layout()->addWidget(btReset);
            btReset->setMaximumWidth(100);
            btReset->setMinimumHeight(40);
            btReset->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
            connect(
                btReset, SIGNAL(released()), this, SLOT(handle_eventResetButtonClicked())
            );

    return (QWidget*)controlsContent;
}

QWidget* MainWindow::buildParameterWidget(Ipt::Core::AlgorythmParamInfo info)
{
    int vidgetType = info.widgetType;

    switch (vidgetType) {
    case (int)Ipt::Core::PARAM_VIDGET_TYPE::ROUNDBUTTON_GROUP:
        return buildParameterRounbuttonGroupWidget(info);
    case (int)Ipt::Core::PARAM_VIDGET_TYPE::SLIDER:
        return buildParameterSliderWidget(info);
    default:
        return new QWidget();
    }
}

QWidget* MainWindow::buildParameterSliderWidget(Ipt::Core::AlgorythmParamInfo info)
{

    auto base = new QGroupBox(info.name);
    base->setLayout(new QHBoxLayout());
    
    auto slider = new QSlider();
    _genericControlsParameterMapping.insert(slider, info.id);
    connect(
        slider, SIGNAL(valueChanged(int)), this, SLOT(handle_eventGenericSliderValueChanged(int))
    );
    base->layout()->addWidget(slider);
    slider->setOrientation(Qt::Orientation::Horizontal);
    slider->setMinimum(info.minValue.toInt());
    slider->setMaximum(info.maxValue.toInt());
    slider->setSingleStep(info.step.toInt());
    slider->setValue(info.defaultValue.toInt());

    return base;
}

QWidget* MainWindow::buildParameterRounbuttonGroupWidget(Ipt::Core::AlgorythmParamInfo info)
{
    auto base = new QGroupBox(info.name);
    base->setLayout(new QHBoxLayout());

    auto bGroup = new QButtonGroup();
    _genericControlsParameterMapping.insert(bGroup, info.id);
    connect(
        bGroup, SIGNAL(buttonReleased(QAbstractButton*)), this, SLOT(handle_eventGenericButtongroupValueChanged())
    );
   

    for (auto it = info.options->begin(); it != info.options->end(); it++) {
                
        auto btn = new QRadioButton();
        btn->setText(it->name);
        bGroup->addButton(btn, it->value.toInt());
        base->layout()->addWidget(btn);
    }

    int defaultId = info.defaultValue.toInt();

    auto btnById = bGroup->button(defaultId);
    btnById->setChecked(true);


    return base;
}

void MainWindow::initAlgorythmSelector()
{
    _algorythmComboBox->clear();

    auto info = *(_imageProcessor->getAlgorythmsInfo());

    int size = info.size();
    for (auto it = info.begin(); it != info.end(); it++) {
        QString nm = it->name;
        QVariant id = it->id;
        _algorythmComboBox->addItem(nm, id);
    }

    rebuildOptionsArea();
}

void MainWindow::createMainMenu()
{

    auto* fileMenu = menuBar()->addMenu("File");
    auto* loadAction = fileMenu->addAction("Open file ...");
    connect(
        loadAction, SIGNAL(triggered()), this, SLOT(handle_actionLoadFile())
    );

}

void MainWindow::rebuildOptionsArea()
{

    _genericControlsParameterMapping.clear();

    // чистим область
    auto items = _optionsParent->findChildren<QWidget*>();
    for (auto it = items.begin(); it != items.end(); it++) {
        (*it)->disconnect();
        (*it)->deleteLater();
    }
   
    // получаем список параметров
    auto info = *(_imageProcessor->getAlgorythmParamInfo(_contextId));

    for (auto it = info.begin(); it != info.end(); it++) {
        _optionsParent->layout()->addWidget(buildParameterWidget((*it)));
    }

}

void MainWindow::initUi()
{
    
    setWindowTitle("Image Processing Tool");

    auto windowContent = new QWidget(); 
    setCentralWidget(windowContent);

    windowContent->setLayout(new QVBoxLayout());

    windowContent->layout()->addWidget(buildImageViewArea());
    windowContent->layout()->addWidget(buildControlsArea());

    createMainMenu();

}

void MainWindow::initImageProcessor()
{

    _imageProcessor = Ipt::Core::ImageProcessor::getInstance();

    _contextId = _imageProcessor->createContext();
    _imageProcessor->regSignalHandler(_contextId, new MainWindowProcessorSignalHandler(_contextId, this));

}

void MainWindow::showSrcImage()
{
    QImage* img = _imageProcessor->getSourceImage(_contextId);
    _graphSceneSrc->clear();
    _graphSceneSrc->addPixmap(QPixmap::fromImage(*img));
    _graphSceneSrc->setSceneRect(0, 0, img->width(), img->height());

    _wSrcGraphicsView->fitInView(_graphSceneSrc->sceneRect(), Qt::KeepAspectRatio);
 
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    _wSrcGraphicsView->fitInView(_graphSceneSrc->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);
    _wPrcGraphicsView->fitInView(_graphSceneSrc->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);

}

void Ipt::Gui::MainWindow::showEvent(QShowEvent* event)
{

    emit ready();
}

void MainWindow::showPrcImage()
{
    QImage* img = _imageProcessor->getProcessedImage(_contextId);
    _graphScenePrc->clear();
    _graphScenePrc->addPixmap(QPixmap::fromImage(*img));
    _graphScenePrc->setSceneRect(0, 0, img->width(), img->height());

    //_wPrcGraphicsView->repaint();
    _wPrcGraphicsView->fitInView(_graphScenePrc->sceneRect(), Qt::KeepAspectRatio);
   
}

bool MainWindow::loadFile(const QString& fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    QImage* newImage = new QImage(reader.read());
    
    if (newImage->isNull()) return false;

    _imageProcessor->setNewSrcImage(_contextId, newImage);
    return true;
}

