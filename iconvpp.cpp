// iconvpp.cpp
// Implementation of following:
//   - iconvpp::Converter
#include "iconvpp.h"

#include <cstring>
#include <memory>
#include <utility>

namespace {
	const ::iconv_t ICONV_INVALID_HANDLE = reinterpret_cast< ::iconv_t >(-1);
}

// Open iconv handle
// If ::iconv_open() failed, all fields in Converter could not change.
void iconvpp::Converter::open(const std::string& dest_enc, const std::string& src_enc)
{
	::iconv_t obj = ::iconv_open(dest_enc.c_str(), src_enc.c_str());
	if (ICONV_INVALID_HANDLE == obj)
		throw iconvpp::Exception("Initialization failed.");

	if (ICONV_INVALID_HANDLE != iconv_)
		::iconv_close(iconv_);

	iconv_ = obj;
	dest_enc_ = dest_enc;
	src_enc_ = src_enc;
}

// c-tor
iconvpp::Converter::Converter(const std::string& dest_enc, const std::string& src_enc)
: iconv_(ICONV_INVALID_HANDLE)
, dest_enc_()
, src_enc_()
{
	open(dest_enc, src_enc);
}

// Copy c-tor
iconvpp::Converter::Converter(const Converter& src)
: iconv_(ICONV_INVALID_HANDLE)
, dest_enc_()
, src_enc_()
{
	open(src.dest_enc_, src.src_enc_);
}

// Move c-tor
iconvpp::Converter::Converter(Converter&& src)
: iconv_(src.iconv_)
, dest_enc_(std::move(src.dest_enc_))
, src_enc_(std::move(src.src_enc_))
{
	src.iconv_ = ICONV_INVALID_HANDLE;
}

// d-tor
iconvpp::Converter::~Converter()
{
	if (ICONV_INVALID_HANDLE != iconv_)
		::iconv_close(iconv_);
}

// Substitution
iconvpp::Converter& iconvpp::Converter::operator=(const Converter& src)
{
	if (this == &src)
		return *this;

	open(src.dest_enc_, src.src_enc_);

	return *this;
}

// Move substitution
iconvpp::Converter& iconvpp::Converter::operator=(Converter&& src)
{
	if (this == &src)
		return *this;

	iconv_ = src.iconv_;
	dest_enc_ = std::move(src.dest_enc_);
	src_enc_ = std::move(src.src_enc_);

	src.iconv_ = ICONV_INVALID_HANDLE;

	return *this;
}

// Converts string
std::string iconvpp::Converter::convert(const std::string& str)
{
	std::size_t from_size = str.size() + 1;

	// Max length of a character is 6 bytes in UTF-8.
	std::size_t to_size = 6 * from_size + 1;

	std::unique_ptr<char[]> from_buf(new char[from_size]);
	std::unique_ptr<char[]> to_buf(new char[to_size]);

	std::strcpy(from_buf.get(), str.c_str());

	// convert
	std::size_t from_size_temp = from_size - 1;
	std::size_t to_size_temp = to_size - 1;
	char* from_ptr = from_buf.get();
	char* to_ptr = to_buf.get();
	if (-1 == ::iconv(iconv_, &from_ptr, &from_size_temp, &to_ptr, &to_size_temp))
		throw iconvpp::Exception("Conversion failed.");
	
	*to_ptr = '\0';

	return std::string(to_buf.get());
}

