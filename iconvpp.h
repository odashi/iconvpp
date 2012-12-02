// iconvpp.h
// Simple wrapper of libiconv
//
#pragma once

#include <stdexcept>
#include <string>
#include <iconv.h>

namespace iconvpp
{
	class Exception : public std::domain_error {
	public:
		Exception(const std::string& cause)
			: std::domain_error("iconv: " + cause) {}
	};

	class Converter {
		::iconv_t iconv_;
		std::string dest_enc_;
		std::string src_enc_;

		void open(const std::string& dest_enc, const std::string& src_enc);

	public:
		Converter(const std::string& dest_enc, const std::string& src_enc);
		Converter(const Converter& src);
		Converter(Converter&& src);
		~Converter();

		Converter& operator=(const Converter& src);
		Converter& operator=(Converter&& src);

		std::string convert(const std::string& str);

		std::string DestEncoding() const { return dest_enc_; }
		std::string SrcEncoding() const { return src_enc_; }
	};
}

