#include <iostream>
#include "iconvpp.h"

int main(void)
{
	try {
		// str_in: UTF-8 string
		std::string str_in = "日本語テストメッセージ";

		// Converter object: UTF-8 into CP932(Shift_JIS)
		iconvpp::Converter cv("CP932", "UTF-8");
	
		// str_out: CP932 converted string 
		auto str_out = cv.convert(str_in);

		std::cout << str_out << std::endl;

	} catch (std::exception& ex) {
		std::cerr << "ERROR ... " << ex.what() << std::endl;
	}

	return 0;
}


