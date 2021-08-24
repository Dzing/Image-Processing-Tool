#pragma once

#include "AbstractAlgorythm.h"

namespace Ipt::Logic {

	class AlgorythmGradient : public Ipt::Core::AbstractAlgorythm
	{
	public:
		AlgorythmGradient() { init(); }
		~AlgorythmGradient() {}

		QImage* execute(QImage* src, QMap<int, QVariant>* params);

		static const int GP_OPERATOR = 0;

		static const int GP_OPERATOR_VAL_SOBEL = 0;
		static const int GP_OPERATOR_VAL_SCHARR = 1;
		static const int GP_OPERATOR_VAL_PREWITT = 2;
	private:
		void init();
		int applyOperator(float(&data)[3][3]);
		void initKernels(int operatorId);

		int kernelH[3][3];
		int kernelV[3][3];

	};

}

