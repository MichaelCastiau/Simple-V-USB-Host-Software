#pragma once
#include <string>
#include <sstream>
using namespace std;
namespace exceptions {

	class DeviceNotConnectedException : runtime_error {
	private:
		string file;
		int line;
	public:
		DeviceNotConnectedException(string p_errorMessage, string p_file, int p_line) :
			runtime_error(p_errorMessage) {
			this->file = p_file;
			this->line = p_line;
		}
		virtual ~DeviceNotConnectedException() = default;
		string getMessage() {
			ostringstream stream;
			stream << "Error: " << this->what() << " in file " << this->file
				<< " on line " << this->line;
			return stream.str();
		}
	};

} /* namespace exceptions */