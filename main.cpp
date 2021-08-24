

#include "MainWindow.h"
#include "ImageProcessor.h"

int main(int argc, char* argv[])
{

	QApplication app(argc, argv);

	Ipt::Gui::MainWindow mw;
	mw.show();

	return app.exec();
}