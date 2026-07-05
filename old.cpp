#include <stdint.h>
#include <stddef.h>


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



template<size_t N>
struct FormatStringConstExpr{
	char data[N];
	int placeholder_count = 0;
	consteval FormatStringConstExpr(const char(&str)[N]) {
		for(size_t i = 0; i < N; i++){
			data[i] = str[i];
			if (i < N-1 && str[i] == '{' && str[i+1] == '}'){
				placeholder_count++;
			}
		}
	}
};

/*
 * I wanted print("test {} {}", 1, 2); type formatting
 * but that requires throwing? 
 * a hack is to point to an undefined function instead of through
 * but it also requires to strip type deducation? 
 * would have to do usign type_identity_t = typename_identity<T>::type;
 */
void panic_func();
template<typename... Args>
struct FormatString{
	StringView view;
	template<size_t N>
		consteval FormatString(const char(&str)[N]): view(str) {
			int placeholder_count = 0;
			for(size_t i = 0; i < N-1; i++){
				if(str[i] == '{' && str[i+1] == '}')
					placeholder_count++;
			}
			if(placeholder_count != sizeof...(Args)){
				panic_func();
			}
		}
};


