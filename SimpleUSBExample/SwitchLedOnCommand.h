#pragma once

#include "Command.h"
#include "USBConnector.h"
#include "IllegalArgumentException.hpp"

namespace business {
	class SwitchLedOnCommand : Command
	{
	private:
		usb::USBConnector* connector;
	public:
		SwitchLedOnCommand(usb::USBConnector* connector);
		~SwitchLedOnCommand() = default;
		void execute();
	};
}


