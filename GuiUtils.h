#pragma once

#include <QFileDialog>

namespace Ipt::Gui {
	class GuiUtils
	{
	public:

		static void initImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode);

	};
}

