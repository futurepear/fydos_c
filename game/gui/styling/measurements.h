#pragma once

namespace GUIStyles {

	struct PositionArguments {
		int top;
		int left;
		int width;
		int height;
	};

	class Measurement {
	public:
		int value = 0;
		const char mode = '0';
		//edge is size of measurement of the parent
		virtual int compute(int edge = 0) = 0;
	};

	class Pixel : public Measurement {
	public:
		const char mode = 'p';
		Pixel(int x);
		int compute(int edge = 0);
	};

	class Percentage : public Measurement {
	public:
		const char mode = '%';
		Percentage(int x);
		int compute(int edge = 0);
	};

	class Calc : public Measurement {
	private:
		Measurement* m_a;
		Measurement* m_b;
	public:
		const char mode = 'c';
		Calc(Measurement* a, Measurement* b);
		~Calc();
		int compute(int edge = 0);
	};

	Measurement* createMeasurement(int x, int mode);
}