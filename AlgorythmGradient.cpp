#include "AlgorythmGradient.h"

using namespace Ipt::Logic;

#include <QImage>

void AlgorythmGradient::init()
{

	setName("Gradient (kernel 3*3)");

	Ipt::Core::Parameter* pColourness = new Ipt::Core::Parameter(GP_OPERATOR, "Operator");
	pColourness->setWidgetType(Ipt::Core::PARAM_VIDGET_TYPE::ROUNDBUTTON_GROUP);
	pColourness->setDefaultValue(0);
	pColourness->regOption(GP_OPERATOR_VAL_SOBEL, new Ipt::Core::Option(GP_OPERATOR_VAL_SOBEL, "Sobel", GP_OPERATOR_VAL_SOBEL));
	pColourness->regOption(GP_OPERATOR_VAL_SCHARR, new Ipt::Core::Option(GP_OPERATOR_VAL_SCHARR, "Scharr", GP_OPERATOR_VAL_SCHARR));
	pColourness->regOption(GP_OPERATOR_VAL_PREWITT, new Ipt::Core::Option(GP_OPERATOR_VAL_PREWITT, "Prewitt", GP_OPERATOR_VAL_PREWITT));
	regParameter(GP_OPERATOR, pColourness);

}

int AlgorythmGradient::applyOperator(float (&data)[3][3])
{
	
	int result = 0;

	int resultH = 0;
	int resultV = 0;

	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {

			resultH += kernelH[x][y] * data[x][y];
			resultV += kernelV[x][y] * data[x][y];

		}
	}

	return(int)(sqrt(pow(resultH,2)+ pow(resultV, 2)));

}

void AlgorythmGradient::initKernels(int operatorId)
{

	switch (operatorId) {
	case GP_OPERATOR_VAL_SOBEL:
		kernelH[0][0] = -1;		kernelH[1][0] = -2;		kernelH[2][0] = -1;
		kernelH[0][1] = 0;		kernelH[1][1] = 0;		kernelH[2][1] = 0;
		kernelH[0][2] = 1;		kernelH[1][2] = 2;		kernelH[2][2] = 1;
		
		kernelV[0][0] = -1;		kernelV[1][0] = 0;		kernelV[2][0] = 1;
		kernelV[0][1] = -2;		kernelV[1][1] = 0;		kernelV[2][1] = 2;
		kernelV[0][2] = -1;		kernelV[1][2] = 0;		kernelV[2][2] = 1;
		break;
	case GP_OPERATOR_VAL_SCHARR:

		kernelH[0][0] = 3;		kernelH[1][0] = 10;		kernelH[2][0] = 3;
		kernelH[0][1] = 0;		kernelH[1][1] = 0;		kernelH[2][1] = 0;
		kernelH[0][2] = -3;		kernelH[1][2] = -10;	kernelH[2][2] = -3;

		kernelV[0][0] = -3;		kernelV[1][0] = 0;		kernelV[2][0] = 3;
		kernelV[0][1] = -10;	kernelV[1][1] = 0;		kernelV[2][1] = 10;
		kernelV[0][2] = -3;		kernelV[1][2] = 0;		kernelV[2][2] = 3;

		break;
	case GP_OPERATOR_VAL_PREWITT:

		kernelH[0][0] = -1;		kernelH[1][0] = -1;		kernelH[2][0] = -1;
		kernelH[0][1] = 0;		kernelH[1][1] = 0;		kernelH[2][1] = 0;
		kernelH[0][2] = 1;		kernelH[1][2] = 1;		kernelH[2][2] = 1;

		kernelV[0][0] = -1;		kernelV[1][0] = 0;		kernelV[2][0] = 1;
		kernelV[0][1] = -1;		kernelV[1][1] = 0;		kernelV[2][1] = 1;
		kernelV[0][2] = -1;		kernelV[1][2] = 0;		kernelV[2][2] = 1;

		break;
	}
}

QImage* AlgorythmGradient::execute(QImage* src, QMap<int, QVariant>* params)
{
	
	auto operatorId = params->value(GP_OPERATOR).toInt();
	initKernels(operatorId);

	QImage* copy = new QImage(*src);

	int w = src->width();
	int h = src->height();

	// предварительно построим карту яркостей каждого пикселя
	//src->width
	float** brMap = new float* [w];
	for (int it = 0; it < w; it++) {
		brMap[it] = new float[h];
	}

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {

			auto pix = src->pixelColor(x, y);
			brMap[x][y] = (pix.red() * 0.299 + pix.green() * 0.587 + pix.blue() * 0.114);
		}
	}

	// для каждого пикселя получаем значения яркостей в окрестности
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {

			int x_minus_1 = (x - 1 < 0) ? x : x - 1;
			int x_plus_1 = (x + 1 >= w) ? x : x + 1;

			int y_minus_1 = (y - 1 < 0) ? y : y - 1;
			int y_plus_1 = (y + 1 >= h) ? y : y + 1;

			float brNearby[3][3];			
			
			brNearby[0][0] = brMap[x_minus_1][y_minus_1];
			brNearby[0][1] = brMap[x_minus_1][y];
			brNearby[0][2] = brMap[x_minus_1][y_plus_1];

			brNearby[1][0] = brMap[x][y_minus_1];
			brNearby[1][1] = brMap[x][y];
			brNearby[1][2] = brMap[x][y_plus_1];

			brNearby[2][0] = brMap[x_plus_1][y_minus_1];
			brNearby[2][1] = brMap[x_plus_1][y];
			brNearby[2][2] = brMap[x_plus_1][y_plus_1];

			// и в зависимости от оператора применяя набор фильтров вычисляем показатель
			int value = applyOperator(brNearby);

			if (value < 0) value = 0;
			if (value > 255) value = 255;

			copy->setPixelColor(x,y, QColor(value, value, value));

		}
	}

	return copy;
}
