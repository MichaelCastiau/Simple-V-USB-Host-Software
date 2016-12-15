#pragma once
using namespace std;
#include <string>
#include <sstream>
namespace exceptions {
	class InvalidArgumentException : runtime_error {
	private:
		string file;
		int line;
	public:
		InvalidArgumentException(string p_errorMessage, string p_file, int p_line) :
			runtime_error(p_errorMessage) {
			this->file = p_file;
			this->line = p_line;
		}
		virtual ~InvalidArgumentException() = default;
		string getMessage() {
			ostringstream stream;
			stream << "Error: " << this->what() << " in file " << this->file
				<< " on line " << this->line;
			return stream.str();
		}
	};
}