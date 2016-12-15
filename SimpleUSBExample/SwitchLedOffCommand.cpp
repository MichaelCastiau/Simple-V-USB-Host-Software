#include "stdafx.h"
#include "SwitchLedOffCommand.h"

namespace business {
	SwitchLedOffCommand::SwitchLedOffCommand(usb::USBConnector* connector)
	{
		if (!connector) {
			throw exceptions::InvalidArgumentException("Error setting usb connector in Switch led off command, connector cannot be undefined", __FILE__, __LINE__);
		}
		this->connector = connector;
	}
	void SwitchLedOffCommand::execute()
	{
		this->connector->sendData(USB_COMMAND_SWITCH_LED, 0, 0);
	}
}

