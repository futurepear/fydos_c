#pragma once

struct CharacterSymbol {
	int x;
	int y;
	int width;
	int height;
	int baseline;
	int pre = 0;
	int advance = 0;
};

namespace Fonts {
	struct Font {
		float baseSize;
		int padding;
		int _maxHeight;
		int charWidth(char c, int fontSize) const;
		float fontSizeToScale(int fontSize) const;
		int maxHeight(int fontSize) const;
	};

	constexpr Font FiraSans{ 48.0f, 12, 61 };
}

const CharacterSymbol& charToSymbol(char c);
