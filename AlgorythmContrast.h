#pragma once

#include "AbstractAlgorythm.h"

namespace Ipt::Logic {

	class AlgorythmContrast : public Ipt::Core::AbstractAlgorythm
	{

	public:

		AlgorythmContrast() { init(); }
		~AlgorythmContrast() {}

		QImage* execute(QImage* src, QMap<int, QVariant>* params);

		static const int CP_COEFFICIENT = 0;
		static const int CP_COLOURNESS = 1;

		static const int CP_COLOURNESS_VAL_CHROMATIC = 0;
		static const int CP_COLOURNESS_VAL_GRAYSCALE = 1;

		
	private:
		
		void init();
		float calcAvBrightness(QImage*);

	};

}
