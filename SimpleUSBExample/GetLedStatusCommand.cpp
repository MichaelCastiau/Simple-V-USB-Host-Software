#include "stdafx.h"
#include "GetLedStatusCommand.h"

namespace business {
	GetLedStatusCommand::GetLedStatusCommand(usb::USBConnector* connector)
	{
		if (!connector) {
			throw exceptions::InvalidArgumentException("Error setting usb connector in get string command, connector cannot be undefined", __FILE__, __LINE__);
		}
		this->connector = connector;
	}

	void business::GetLedStatusCommand::execute()
	{
		unsigned char* status = this->connector->getData(USB_GET_STRING, 0, 0);
		if (status[0] == 1) {
			printf("The led is currently switched on\n");
		}
		else {
			printf("The led is currently switched off\n");
		}
	}
}
