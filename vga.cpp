#include "vga.hpp"



/*
 * VGA Writer specific structures
 */


void VGAWriter::bounds_check(){
	if(col >= maxcol){
		col = 0;
		row++;
	}
	if(row >= maxrow){
		// copy the current rows and move it up wone
		shift_up();
	}
}
void VGAWriter::shift_up(){
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
void VGAWriter::write_char(char c, uint8_t color){
	if(c == '\n'){
		row++;
		col = 0;
	} else {
		buffer[row * maxcol + col] = ((uint16_t)c | (uint16_t) color << 8);
		col++;
	}
	bounds_check();
}
void VGAWriter::write_char(char c){
	write_char(c, this->color);
}
void VGAWriter::print(StringView str){
	print(str, this->color);
}
void VGAWriter::print(StringView str, uint8_t color){
	for(size_t i = 0; i < str.length(); i++){
		write_char(str[i],color);
	}
}


void VGAWriter::print(int32_t number){
	if(number == 0){
		write_char('0');
		return;
	}
	uint32_t u_num = static_cast<uint32_t>(number);
	if(number < 0){
		write_char('-');
		// use uint instead of int to prevent int32_min overflow edge case
		u_num = -u_num;
	}
	char buf[16];
	int i = 0;
	while(u_num > 0){
		buf[i] = '0' + (u_num %10);
		u_num = u_num / 10;
		i++;
	}
	for(int j = i - 1; j >= 0; j--) {
		write_char(buf[j]);
	}
}
void VGAWriter::clear(){
	for(int i = 0; i < maxrow; i++){
		for(int j = 0; j < maxcol; j++){
			buffer[i * maxcol + j] = ((uint16_t)' ' | (uint16_t) color << 8);
		}
	}
	row = 0;
	col = 0;
}

void VGAWriter::set_color(uint8_t new_color){
	this->color = new_color;
}
uint8_t VGAWriter::get_color(){
	return this->color;
}

namespace VGA{
	Spinlock vga_lock;
	VGAWriter vga;

	void print(StringView str){
		vga_lock.lock();
		vga.print(str);
		vga_lock.unlock();
	}
	void print(StringView str, uint8_t color){
		vga_lock.lock();
		vga.print(str, color);
		vga_lock.unlock();
	}
	void println(StringView str){
		vga_lock.lock();
		vga.print(str);
		vga.print("\n");
		vga_lock.unlock();
	}
	void println(StringView str, uint8_t color){
		vga_lock.lock();
		vga.print(str, color);
		vga.print("\n");
		vga_lock.unlock();
	}
	void clear(){
		vga_lock.lock();
		vga.clear();
		vga_lock.unlock();
	}
}


