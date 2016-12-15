#pragma once
#include <string>
#include <sstream>
using namespace std;
namespace exceptions {

	class DeviceAlreadyOpenException :runtime_error {
	private:
		string file;
		int line;
	public:
		DeviceAlreadyOpenException(string p_errorMessage, string p_file, int p_line) :
			runtime_error(p_errorMessage) {
			this->file = p_file;
			this->line = p_line;
		}
		virtual ~DeviceAlreadyOpenException() = default;
		string getMessage() {
			ostringstream stream;
			stream << "Error: " << this->what() << " in file " << this->file << " on line " << this->line;
			return stream.str();
		}
	};

} /* namespace exceptions */