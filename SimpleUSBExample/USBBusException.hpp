#pragma once

#include "USBException.hpp"
using namespace std;
namespace exceptions {

	class USBBusException :USBException {
	public:
		USBBusException(string p_errorMessage, string p_file, int p_line) :
			USBException(p_errorMessage, p_file, p_line) {

		}
		~USBBusException() = default;
	};

} /* namespace exceptions */
