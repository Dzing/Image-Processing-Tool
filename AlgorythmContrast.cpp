#include "AlgorythmContrast.h"

#include <QImage>

using namespace Ipt::Logic;

void AlgorythmContrast::init()
{

	this->_name = "Contrast";

	Ipt::Core::Parameter * pCoefficient = new Ipt::Core::Parameter(CP_COEFFICIENT, "Coefficient");
	pCoefficient->setWidgetType(Ipt::Core::PARAM_VIDGET_TYPE::SLIDER);
	pCoefficient->setMinValue(-100);
	pCoefficient->setMaxValue(100);
	pCoefficient->setDefaultValue(0);
	pCoefficient->setStep(1);
	regParameter(CP_COEFFICIENT, pCoefficient);

	Ipt::Core::Parameter* pColourness = new Ipt::Core::Parameter(CP_COLOURNESS, "Colourness");
	pColourness->setWidgetType(Ipt::Core::PARAM_VIDGET_TYPE::ROUNDBUTTON_GROUP);
	pColourness->setDefaultValue(0);
	pColourness->regOption(CP_COLOURNESS_VAL_CHROMATIC, new Ipt::Core::Option(CP_COLOURNESS_VAL_CHROMATIC, "Chromatic", CP_COLOURNESS_VAL_CHROMATIC));
	pColourness->regOption(CP_COLOURNESS_VAL_GRAYSCALE, new Ipt::Core::Option(CP_COLOURNESS_VAL_GRAYSCALE, "Grayscale", CP_COLOURNESS_VAL_GRAYSCALE));
	regParameter(CP_COLOURNESS, pColourness);
}

QImage* AlgorythmContrast::execute(QImage* src, QMap<int, QVariant>* params)
{
	QImage* copy = new QImage(*src);

	int w = src->width();
	int h = src->height();

	// ищем среднюю €ркость изображени€
	float avrBrightness = calcAvBrightness(src);

	// ќЁ‘‘»÷»≈Ќ“
	int KOEFF = params->value(CP_COEFFICIENT).toInt(); // задавать параметром

	double k = 1.0 + KOEFF/100.0;

	//дл€ цветного варианта каждую составл€ющую мы будем умножать на коэффициент.
	// так как цвета задаютс€ известным диапазоном , можно предрассчитать результат применени€ коэффициента заранее дл€ каждого значени€ цвета
	// и потом пользоватьс€ этой кратой, а не рассчитывать всЄ это по 3 раза дл€ каждого пиксел€

	int MAX_COLOR_VALUE = pow(2,8) -1;			// дл€ 8-битного цвета
	uchar* map = new uchar[MAX_COLOR_VALUE+1];				
	for (int it = 0; it < MAX_COLOR_VALUE+1; it++) {
		
		int delta = (int)(it - avrBrightness);

		int tmp = (int)(avrBrightness + k * delta);

		if (tmp < 0) { tmp = 0; }
		if (tmp > MAX_COLOR_VALUE - 1) { tmp = MAX_COLOR_VALUE - 1; }

		map[it] = tmp;
	}

	auto f = copy->format();
	auto pf = copy->pixelFormat();

	int paramColourness = params->value(CP_COLOURNESS).toInt();

	// примен€ем рассчеты к каждому пикселю
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {

			QColor pix = copy->pixelColor(x, y);

			QColor newpix;
			switch (paramColourness) {
			case (int)CP_COLOURNESS_VAL_CHROMATIC:
				newpix = QColor(map[pix.red()], map[pix.green()], map[pix.blue()]);
				//copy->setPixelColor(x, y, newpix);
				break;
			case (int)CP_COLOURNESS_VAL_GRAYSCALE:
				// переведем в значение серого по весам
				int greyValue = (int)(pix.red()*0.3+ pix.green()*0.6+ pix.blue()*0.1);
				int mappedGreyValue = map[greyValue];
				newpix = QColor(mappedGreyValue, mappedGreyValue, mappedGreyValue);
				//copy->setPixelColor(x, y, newpix);
				break;
			}

			copy->setPixelColor(x, y, newpix);
		}
	}

	delete [] map;
	// 
	
	//copy->invertPixels();

	return copy;
}

float AlgorythmContrast::calcAvBrightness(QImage* src)
{

	int size_w = src->width();
	int size_h = src->height();

	int incBrightness = 0;

	//суммарна€ €ркость всех пикселей
	for (int x = 0; x < size_w; x++) {
		for (int y = 0; y < size_h; y++) {
			QColor pix = src->pixelColor(x, y);
			
			incBrightness += (int)(pix.red() * 0.299 + pix.green() * 0.587 + pix.blue() * 0.114);

		}
	}
	// делим на общее количество пикселей
	return incBrightness / (size_w* size_h);

}




