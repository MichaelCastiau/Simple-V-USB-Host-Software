#pragma once

#ifndef USB_COMMAND_SWITCH_LED
#define USB_COMMAND_SWITCH_LED 0
#endif // !USB_COMMAND_SWITCH_LED

#ifndef USB_GET_STRING
#define USB_GET_STRING 1
#endif // !USB_GET_STRING




namespace business {
	class Command {
		//Abstract class
		virtual void execute() = 0;
	};
}