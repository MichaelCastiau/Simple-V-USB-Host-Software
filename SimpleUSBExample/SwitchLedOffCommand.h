#pragma once

#include "Command.h"
#include "USBConnector.h"
#include "IllegalArgumentException.hpp"
#include <iostream>

namespace business {
	class SwitchLedOffCommand
	{
	private:
		usb::USBConnector* connector;
	public:
		SwitchLedOffCommand(usb::USBConnector* connector);
		~SwitchLedOffCommand() = default;
		void execute();
	};
}


