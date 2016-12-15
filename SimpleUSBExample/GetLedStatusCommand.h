#pragma once

#include "Command.h"
#include "USBConnector.h"

namespace business {
	class GetLedStatusCommand : Command
	{
	private:
		usb::USBConnector* connector;
	public:
		GetLedStatusCommand(usb::USBConnector* connector);
		~GetLedStatusCommand() = default;

		void execute();
	};
}


