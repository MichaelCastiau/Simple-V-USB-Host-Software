#pragma once
#include <string>
#include <sstream>
using namespace std;
namespace exceptions {

	class USBException : runtime_error {
	private:
		string file;
		int line;
	public:
		USBException(string p_errorMessage, string p_file, int p_line) :
			runtime_error(p_errorMessage) {
			this->file = p_file;
			this->line = p_line;
		}
		virtual ~USBException() = default;
		string getMessage() {
			ostringstream stream;
			stream << "Error: " << this->what() << " in file " << this->file
				<< " on line " << this->line;
			return stream.str();
		}
	};
}