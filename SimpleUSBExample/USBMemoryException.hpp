#pragma once
#include "USBException.hpp"
using namespace std;
namespace exceptions {

	class USBMemoryException :USBException {
	public:
		USBMemoryException(string p_errorMessage, string p_file, int p_line) :
			USBException(p_errorMessage, p_file, p_line) {

		}
		~USBMemoryException() = default;
	};

} /* namespace exceptions */
