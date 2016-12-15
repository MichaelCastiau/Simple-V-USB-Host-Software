#pragma once
#include "USBException.hpp"
namespace exceptions {

	class USBAccessException :USBException {
	public:
		USBAccessException(string p_errorMessage, string p_file, int p_line) :
			USBException(p_errorMessage, p_file, p_line) {

		}
		~USBAccessException() = default;
	};

} /* namespace exceptions */
