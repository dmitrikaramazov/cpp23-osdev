#pragma once
#include <stddef.h>
#include <stdint.h>

template<typename T> struct type_identity { using type = T; };
template<typename T> using type_identity_t = typename type_identity<T>::type;

template<typename T>
concept OutputDriver = requires(T driver, char c){
	driver.write_char(c);
};

class StringView {
private:
	const char* data_;
	size_t length_;
public:
	template<size_t N>
    constexpr StringView(const char(&str)[N]) : data_(str), length_(N-1) {}
	constexpr StringView(const char* str, size_t len) : data_(str), length_(len) {}
	char operator[](size_t index) const {
		if(index >= length_) return ' ';
		return data_[index];
	}
	size_t length() const { return length_; }
	const char* data() const { return data_; }
};


void jank_throw();
template<typename... Args>
struct FormatString {
	StringView view;
	template<size_t N>
	consteval FormatString(const char(&str)[N]) : view(str, N-1) {
		int placeholder_count = 0;
		for(size_t i = 0; i < N -1; i++){
			if(str[i] == '{' && str[i+1] == '}'){
				placeholder_count++;
			}
		}
		if(placeholder_count != sizeof...(Args)){
			// only ever evaluated at compile time i think
			jank_throw();
		}
	}
};

namespace Fmt {

	template<OutputDriver Driver>
	void print(Driver& driver, StringView str){
		for(size_t i = 0; i < str.length(); i++){
			driver.write_char(str[i]);
		}
	}

	template<OutputDriver Driver>
	void print(Driver& driver, int32_t number) {
		if(number == 0){
			driver.write_char('0');
			return;
		}
		uint32_t u_num = static_cast<uint32_t>(number);
		if(number < 0){
			driver.write_char('-');
			u_num = -u_num;
		}
		char buf[16];
		int i = 0;
		while(u_num > 0){
			buf[i] = '0' + (u_num%10);
			u_num /= 10;
			i++;
		}
		for(int j = i-1; j >= 0; j--){
			driver.write_char(buf[j]);
		}
	}

	template<typename Driver, typename T, typename... Args>
	void unroll_format(Driver& driver, StringView str, const T& value, const Args&... args) {
		for(size_t i = 0; i < str.length(); i++){
			if(i < str.length() - 1 && str[i] == '{' && str[i+1] == '}'){
				Fmt::print(driver, StringView(str.data(),i));
				Fmt::print(driver, value);
				StringView remainder(str.data() + i + 2, str.length() - (i+2));
				if constexpr (sizeof...(args) > 0){
					unroll_format(driver,remainder,args...);
				} else {
					Fmt::print(driver, remainder);
				}
				return;
			}
		}
	}
	
	template<OutputDriver Driver,  typename... Args>
	void print_fmt(Driver& driver,FormatString<Args...> fmt, const Args&... args){
		if constexpr(sizeof...(Args) > 0){
			unroll_format(driver,fmt.view,args...);
		} else {
			print(driver, fmt.view);
		}
	}
};
