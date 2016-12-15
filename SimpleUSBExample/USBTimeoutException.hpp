#pragma once
#include "USBException.hpp"
using namespace std;
namespace exceptions {

	class USBTimeOutException :USBException {
	public:
		USBTimeOutException(string p_errorMessage, string p_file, int p_line) :
			USBException(p_errorMessage, p_file, p_line) {

		}
		~USBTimeOutException() = default;
	};

} /* namespace exceptions */
#pragma once
