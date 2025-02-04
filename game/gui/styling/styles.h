#pragma once

namespace GUIStyles {
	struct Color {
		int r = 128;
		int g = 128;
		int b = 128;
	};

	enum class Flex {
		row,
		column,
		all,
		grow,
		none,
		center,
		front,
		back,
		wrap
	};

	enum class Position {
		relative,
		absolute
	};

	enum class Scroll {
		hidden,
		scroll
	};

	struct FontStyle {
		int size = 24;
		int lineHeight = 30;
		Color color{};
		Color shadowColor{};
		int fontWeight = 400;
	};
}