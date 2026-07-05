
#include <cstddef>
#include <cstdint>



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
struct FormatString{
	char data[N];
	int placeholder_count = 0;
	consteval FormatString(const char(&str)[N]) {
		for(size_t i = 0; i < N; i++){
			data[i] = str[i];
			if (i < N-1 && str[i] == '{' && str[i+1] == '}'){
				placeholder_count++;
			}
		}
	}
};


enum Color {
	Black = 0,
	Blue,
	Green,
	Cyan,
	Red,
	Magenta,
	Brown,
	LightGray,
	DarkGray,
	LightBlue,
	LightGreen,
	LightCyan,
	LightRed,
	Pink,
	Yellow,
	White
};
class ColorCode{
private:
	uint8_t value;
public:
	constexpr ColorCode(Color fg, Color bg)
		: value((static_cast<uint8_t>(bg << 4) | static_cast<uint8_t>(fg))){}
	constexpr operator uint8_t() const {
		return value;
	}
};


class VGAWriter {
private:
	int row = 0;
	int col = 0;
	static constexpr int maxrow = 25;
	static constexpr int maxcol = 80;
	uint8_t color = 0x0F;
	volatile uint16_t* buffer = (uint16_t*)0xB8000;

	void bounds_check(){
		if(col >= maxcol){
			col = 0;
			row++;
		}
		if(row >= maxrow){
			// copy the current rows and move it up wone
			shift_up();
		}
	}
	void shift_up(){
		for(int i = 1; i < maxrow; i++){
			for(int j = 0; j < maxcol; j++){
				buffer[(i-1)*maxcol+j]
					= buffer[i*maxcol+j];
			}
		}
		for(int j = 0; j < maxcol; j++){
			buffer[(maxrow-1)*maxcol + j] =
				((uint16_t)' ' | (uint16_t)color << 8);
		}
		row = maxrow-1;
		col = 0;
	}
	template<typename T, typename... Args>
	void unroll_format(StringView str, const T& value, const Args&... args) {
		for(size_t i = 0; i < str.length(); i++){
			if(i < str.length() - 1 && str[i] == '{' && str[i+1] == '}') {
				print(StringView(str.data(),i));
				this->print(value);
				StringView remainder(str.data() + i + 2, str.length() - (i+2));
				if constexpr (sizeof...(args) > 0){
					unroll_format(remainder, args...);
				} else {
					print(remainder);
				}
				return;
			}
		}
	}
public:
	void write_char(char c, uint8_t color){
		if(c == '\n'){
			row++;
			col = 0;
		} else {
			buffer[row * maxcol + col] = ((uint16_t)c | (uint16_t) color << 8);
			col++;
		}
		bounds_check();
	}
	void write_char(char c){
		write_char(c, this->color);
	}
	template<size_t N>
    void print(const char (&str)[N]){
		print(StringView(str));
	}
	template<size_t N>
    void print(const char (&str)[N], uint8_t color){
		print(StringView(str), color);
	}
	void print(StringView str){
		print(str, this->color);
	}
	void print(StringView str, uint8_t color){
		for(size_t i = 0; i < str.length(); i++){
			write_char(str[i],color);
		}
	}
	void print(int32_t number){
		if(number == 0){
			write_char('0');
		}
		if(number < 0){
			write_char('-');
			number = -number;
		}
		char buf[16];
		int i = 0;
		while(number > 0){
			buf[i] = '0' + (number %10);
			number = number / 10;
			i++;
		}
		for(int j = i - 1; j >= 0; j--) {
			write_char(buf[j]);
		}
	}
	template<FormatString fmt, typename... Args>
	void print_fmt(const Args&... args) {
		static_assert(fmt.placeholder_count == sizeof...(Args),
				"Format Error: Number of {} does not match arguments!");
		StringView view(fmt.data, sizeof(fmt.data)-1);
		unroll_format(view,args...);
	}
	void clear(){
		for(int i = 0; i < maxrow; i++){
			for(int j = 0; j < maxcol; j++){
				buffer[i * maxcol + j] = ((uint16_t)' ' | (uint16_t) color << 8);
			}
		}
		row = 0;
		col = 0;
	}
};

// Multiboot header
// QEMU reads this and loads the program
struct [[gnu::packed]] MultibootHeader {
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
};

extern "C" [[gnu::section(".multiboot"), gnu::used]]
const MultibootHeader header = {
	0x1badb002,
	0x00000000,
	static_cast<uint32_t>(-(0x1badb002 + 0x00000000))
};

extern "C" [[noreturn]] void _start() {
	VGAWriter out;
	out.clear();
	out.print("[*] Booting OS...\n");
	out.print("[i] Testing...\n", ColorCode(Blue,Green));
	for(int i = 0; i < 3; i++){
		Color fg = static_cast<Color>((i%15)+2);
		Color bg = static_cast<Color>((i%15)+3);
		out.print("[i]    ");
		out.print("Spam!\n", ColorCode(fg,bg));
	}
	out.print_fmt<"[*] Testing: {} {}\n">(80,81);
	out.print_fmt<"[*] Testing: {} {}\n">(StringView("Test"),81);
	out.print_fmt<"[*] Testing: {} {}\n">(82, "test2");
	out.print_fmt<"[*] Size of {}: {}\n">("size_t", sizeof(size_t));
	out.print_fmt<"[*] Size of {}: {}\n">("int*", sizeof(int*));

	while(true){

	}
}
