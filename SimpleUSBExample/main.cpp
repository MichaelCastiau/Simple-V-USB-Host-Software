// SimpleUSBExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "USBConnector.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include "SwitchLedOffCommand.h"
#include "SwitchLedOnCommand.h"
#include "GetLedStatusCommand.h"

void print(std::string value) {
	std::cout << value << std::endl;
}
void error(std::string error) {
	std::cerr << error << std::endl;
}
int main(int argc, char** argv) {
	try {
		print("Initializing connector...");
		usb::USBConnector connector(0x16c0, 0x05dc);
		print("DONE");
		print("Connecting to usb device...");
		connector.connect();
		print("DONE");
		string message;
		while (message != "exit") {
			print("enter 'on' to switch led on, 'check' to get led status or 'off' to switch led off: (type 'exit' to exit)");
			std::cin >> message;
			if (message == "on") {
				business::SwitchLedOnCommand ledOn(&connector);
				ledOn.execute();
			}
			else if (message == "off") {
				business::SwitchLedOffCommand ledOff(&connector);
				ledOff.execute();
			}
			else if (message == "check") {
				business::GetLedStatusCommand getString(&connector);
				getString.execute();
			}
			else if (message != "exit") {
				std::cout << "command '" << message << "' unknown" << std::endl;
			}
		}
		//If we got out of the loop, we'll shut down the application
		connector.closeConnection();
		return 0;
	}
	catch (exceptions::DeviceAlreadyOpenException& e) {
		error("The device was already opened");
	}
	catch (exceptions::DeviceNotConnectedException& e) {
		error(e.getMessage());
	}
	catch (exceptions::USBAccessException& e) {
		error("USB Access error");
	}
	catch (exceptions::USBBusException& e) {
		error("The bus is busy");
	}
	catch (exceptions::USBInputOutputException& e) {
		error("Input/output error");
	}
	catch (exceptions::USBInvalidParameterException& e) {
		error("Invalid parameter exception");
	}
	catch (exceptions::USBMemoryException& e) {
		error("Ran out of memory");
	}
	catch (exceptions::USBTimeOutException& e) {
		error("Connection timed out");
	}
	catch (exceptions::USBException& e) {
		error("undefined usb error");
	}
	catch (runtime_error& e) {
		error(e.what());
	}
}
