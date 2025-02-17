#include "textRenderer.h"

constexpr CharacterSymbol Serif[95]{
	{0, 11, 25, 51, 56},  //   
	{25, 11, 29, 51, 56},  // ! 
	{53, 11, 32, 32, 56},  // " 
	{86, 11, 37, 51, 56},  // # 
	{123, 11, 37, 55, 56},  // $ 
	{160, 11, 54, 51, 56},  // % 
	{213, 11, 51, 51, 56},  // & 
	{264, 11, 21, 32, 56},  // ' 
	{285, 11, 29, 62, 56},  // ( 
	{314, 11, 29, 62, 56},  // ) 
	{342, 11, 37, 36, 56},  // * 
	{379, 11, 40, 48, 56},  // + 
	{420, 11, 25, 59, 56},  // , 
	{444, 11, 29, 42, 56},  // - 
	{473, 11, 25, 51, 56},  // . 
	{0, 73, 26, 51, 118},  // / 
	{26, 73, 37, 51, 118},  // 0 
	{63, 73, 37, 51, 118},  // 1 
	{100, 73, 37, 51, 118},  // 2 
	{137, 73, 37, 51, 118},  // 3 
	{174, 73, 37, 51, 118},  // 4 
	{211, 73, 37, 51, 118},  // 5 
	{248, 73, 37, 51, 118},  // 6 
	{285, 73, 37, 51, 118},  // 7 
	{322, 73, 37, 51, 118},  // 8 
	{359, 73, 37, 51, 118},  // 9 
	{396, 73, 26, 51, 118},  // : 
	{422, 73, 26, 59, 118},  // ; 
	{448, 73, 40, 46, 118},  // < 
	{0, 135, 40, 39, 180},  // = 
	{40, 135, 40, 46, 180},  // > 
	{80, 135, 34, 51, 180},  // ? 
	{115, 135, 58, 62, 180},  // @ 
	{173, 135, 48, 51, 180},  // A 
	{221, 135, 45, 51, 180},  // B 
	{266, 135, 45, 51, 180},  // C 
	{311, 135, 48, 51, 180},  // D 
	{360, 135, 43, 51, 180},  // E 
	{402, 135, 40, 51, 180},  // F 
	{442, 135, 48, 51, 180},  // G 
	{0, 197, 48, 51, 242},  // H 
	{48, 197, 29, 51, 242},  // I 
	{77, 197, 31, 51, 242},  // J 
	{108, 197, 48, 51, 242},  // K 
	{156, 197, 43, 51, 242},  // L 
	{199, 197, 56, 51, 242},  // M 
	{255, 197, 48, 51, 242},  // N 
	{303, 197, 48, 51, 242},  // O 
	{352, 197, 40, 51, 242},  // P 
	{391, 197, 48, 60, 242},  // Q 
	{439, 197, 45, 51, 242},  // R 
	{0, 259, 40, 51, 304},  // S 
	{40, 259, 43, 51, 304},  // T 
	{82, 259, 48, 51, 304},  // U 
	{130, 259, 48, 51, 304},  // V 
	{179, 259, 59, 51, 304},  // W 
	{238, 259, 48, 51, 304},  // X 
	{286, 259, 48, 51, 304},  // Y 
	{334, 259, 43, 51, 304},  // Z 
	{377, 259, 29, 61, 304},  // [ 
	{405, 259, 26, 51, 304},  // \ 
	{431, 259, 29, 61, 304},  // ] 
	{460, 259, 35, 36, 304},  // ^ 
	{0, 321, 37, 62, 366},  // _ 
	{37, 321, 29, 25, 366},  // ` 
	{66, 321, 34, 51, 366},  // a 
	{100, 321, 37, 51, 366},  // b 
	{137, 321, 34, 51, 366},  // c 
	{171, 321, 37, 51, 366},  // d 
	{208, 321, 34, 51, 366},  // e 
	{242, 321, 29, 51, 366},  // f 
	{271, 321, 37, 62, 366},  // g 
	{308, 321, 37, 51, 366},  // h 
	{345, 321, 26, 51, 366},  // i 
	{371, 321, 26, 62, 366},  // j 
	{397, 321, 37, 51, 366},  // k 
	{434, 321, 26, 51, 366},  // l 
	{460, 321, 51, 51, 366},  // m 
	{0, 383, 37, 51, 428},  // n 
	{37, 383, 37, 51, 428},  // o 
	{74, 383, 37, 62, 428},  // p 
	{111, 383, 37, 62, 428},  // q 
	{148, 383, 29, 51, 428},  // r 
	{177, 383, 31, 51, 428},  // s 
	{208, 383, 26, 51, 428},  // t 
	{234, 383, 37, 51, 428},  // u 
	{271, 383, 37, 51, 428},  // v 
	{308, 383, 48, 51, 428},  // w 
	{356, 383, 37, 51, 428},  // x 
	{393, 383, 37, 62, 428},  // y 
	{430, 383, 34, 51, 428},  // z 
	{464, 383, 36, 62, 428},  // { 
	{0, 445, 22, 62, 490},  // | 
	{22, 445, 36, 62, 490},  // } 
	{58, 445, 39, 41, 490},  // ~ 
};

constexpr CharacterSymbol FiraSans[95]{
	{-4, 11, 25, 51, 56, 0, 0},  //   
	{23, 11, 24, 52, 56, -2, 10},  // ! 
	{51, 11, 31, 31, 56, -2, 17},  // " 
	{86, 11, 37, 51, 56, 0, 25},  // # 
	{125, 11, 37, 59, 56, 0, 24},  // $ 
	{164, 11, 52, 53, 56, -1, 38},  // % 
	{219, 11, 47, 52, 56, -3, 34},  // & 
	{271, 11, 23, 31, 56, -2, 8},  // ' 
	{298, 11, 28, 58, 56, -1, 14},  // ( 
	{329, 11, 28, 58, 56, -1, 14},  // ) 
	{360, 11, 33, 34, 56, 0, 21},  // * 
	{395, 11, 36, 44, 56, -2, 22},  // + 
	{435, 11, 24, 59, 56, -1, 10},  // , 
	{462, 11, 31, 38, 56, -2, 17},  // - 
	{0, 73, 24, 52, 118, -2, 10},  // . 
	{28, 73, 37, 56, 118, -4, 21},  // / 
	{71, 73, 39, 52, 118, -2, 25},  // 0 
	{114, 73, 33, 51, 118, -1, 16},  // 1 
	{150, 73, 36, 51, 118, 0, 22},  // 2 
	{188, 73, 36, 52, 118, 0, 22},  // 3 
	{226, 73, 38, 51, 118, -1, 25},  // 4 
	{267, 73, 36, 52, 118, 0, 23},  // 5 
	{305, 73, 38, 52, 118, -2, 24},  // 6 
	{347, 73, 33, 52, 118, 0, 21},  // 7 
	{382, 73, 38, 52, 118, -1, 25},  // 8 
	{423, 73, 37, 52, 118, -1, 24},  // 9 
	{463, 73, 24, 52, 118, -2, 10},  // : 
	{0, 135, 24, 59, 180, -1, 10},  // ; 
	{27, 135, 36, 46, 180, -2, 22},  // < 
	{67, 135, 36, 42, 180, -2, 22},  // = 
	{107, 135, 36, 46, 180, -2, 22},  // > 
	{147, 135, 34, 52, 180, -1, 21},  // ? 
	{184, 135, 61, 61, 180, -3, 46},  // @ 
	{250, 135, 40, 51, 180, 0, 28},  // A 
	{292, 135, 41, 51, 180, -4, 28},  // B 
	{339, 135, 39, 52, 180, -2, 27},  // C 
	{382, 135, 43, 51, 180, -4, 29},  // D 
	{431, 135, 38, 51, 180, -4, 24},  // E 
	{0, 197, 36, 51, 242, -4, 23},  // F 
	{42, 197, 42, 52, 242, -2, 28},  // G 
	{88, 197, 45, 51, 242, -4, 29},  // H 
	{139, 197, 26, 51, 242, -4, 10},  // I 
	{171, 197, 27, 58, 242, 1, 11},  // J 
	{199, 197, 40, 51, 242, -4, 29},  // K 
	{245, 197, 36, 51, 242, -4, 24},  // L 
	{287, 197, 49, 51, 242, -2, 35},  // M 
	{340, 197, 45, 51, 242, -4, 29},  // N 
	{391, 197, 45, 52, 242, -2, 31},  // O 
	{440, 197, 40, 51, 242, -4, 27},  // P 
	{0, 259, 45, 56, 304, -2, 33},  // Q 
	{49, 259, 41, 51, 304, -4, 29},  // R 
	{96, 259, 38, 52, 304, 0, 25},  // S 
	{136, 259, 37, 51, 304, 0, 25},  // T 
	{175, 259, 44, 52, 304, -4, 28},  // U 
	{225, 259, 39, 51, 304, 0, 27},  // V 
	{266, 259, 52, 51, 304, 0, 39},  // W 
	{320, 259, 38, 51, 304, 1, 26},  // X 
	{359, 259, 38, 51, 304, 1, 27},  // Y 
	{398, 259, 37, 51, 304, -1, 24},  // Z 
	{438, 259, 27, 57, 304, -2, 14},  // [ 
	{469, 259, 37, 56, 304, -4, 21},  // \ 
	{0, 321, 27, 57, 366, -2, 13},  // ] 
	{31, 321, 38, 26, 366, -1, 25},  // ^ 
	{72, 321, 37, 58, 366, 0, 25},  // _ 
	{111, 321, 26, 21, 366, -1, 14},  // ` 
	{140, 321, 38, 52, 366, -1, 24},  // a 
	{181, 321, 41, 52, 366, -4, 26},  // b 
	{228, 321, 35, 52, 366, -2, 22},  // c 
	{267, 321, 41, 52, 366, -2, 25},  // d 
	{312, 321, 38, 52, 366, -2, 24},  // e 
	{354, 321, 28, 51, 366, 0, 20},  // f 
	{384, 321, 37, 61, 366, 0, 26},  // g 
	{423, 321, 40, 51, 366, -4, 25},  // h 
	{469, 321, 26, 51, 366, -3, 11},  // i 
	{0, 383, 25, 62, 428, 1, 11},  // j 
	{26, 383, 37, 51, 428, -4, 25},  // k 
	{69, 383, 26, 52, 428, -4, 14},  // l 
	{101, 383, 53, 51, 428, -4, 38},  // m 
	{160, 383, 40, 51, 428, -4, 25},  // n 
	{206, 383, 40, 52, 428, -2, 26},  // o 
	{250, 383, 41, 62, 428, -4, 26},  // p 
	{297, 383, 41, 62, 428, -2, 25},  // q 
	{342, 383, 31, 51, 428, -4, 19},  // r 
	{379, 383, 34, 52, 428, 0, 21},  // s 
	{415, 383, 29, 52, 428, 0, 18},  // t 
	{446, 383, 40, 52, 428, -4, 24},  // u 
	{0, 445, 36, 51, 490, 0, 24},  // v 
	{38, 445, 46, 51, 490, 0, 34},  // w 
	{86, 445, 35, 51, 490, 1, 24},  // x 
	{122, 445, 36, 62, 490, 0, 24},  // y 
	{160, 445, 33, 51, 490, 0, 20},  // z 
	{195, 445, 27, 58, 490, 0, 14},  // { 
	{224, 445, 31, 56, 490, -7, 12},  // | 
	{264, 445, 27, 58, 490, -1, 15},  // } 
	{294, 445, 35, 39, 490, -1, 23},  // ~ 
};


int Fonts::Font::charWidth(char c, int fontSize) const {
	return (charToSymbol(c).width - padding) * fontSize / baseSize;
}
float Fonts::Font::fontSizeToScale(int fontSize) const {
	return fontSize / baseSize;
}
int Fonts::Font::maxHeight(int fontSize) const {
	return fontSize / baseSize * _maxHeight;
}

const CharacterSymbol& charToSymbol(char c) {
	int index = static_cast<int>(c) - 32;
	return FiraSans[index];
}