#include "stdafx.h"
#include "SwitchLedOnCommand.h"
#include <iostream>

namespace business {
	SwitchLedOnCommand::SwitchLedOnCommand(usb::USBConnector* connector)
	{
		if (!connector) {
			throw exceptions::InvalidArgumentException("Error setting usb connector in Switch led on command, connector cannot be undefined", __FILE__, __LINE__);
		}
		this->connector = connector;
	}
	void SwitchLedOnCommand::execute()
	{
		this->connector->sendData(USB_COMMAND_SWITCH_LED, 1, 0, 0);
	}
}


