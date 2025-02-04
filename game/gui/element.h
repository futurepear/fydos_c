#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <set>
#include "events.h"
#include "styling/measurements.h"
#include "styling/styles.h"

class GUIElement {
private:
	GUIStyles::Measurement* width;
	GUIStyles::Measurement* height;
	GUIStyles::Measurement* top;
	GUIStyles::Measurement* left;
	void setMeasurement(GUIStyles::Measurement*& x, int a, char modeA = '0', int b = 0, char modeB = '0');

	std::string innerText = "";
	int textHeight();
public:
	bool needsRecomputation = false;

	int offsetTop = 0;
	int offsetLeft = 0;
	//how much space it fills in, in pixels
	int offsetWidth = 0;
	int offsetHeight = 0;
	int scrollWidth = 0;
	int scrollHeight = 0;
	int scrollTop = 0;

	int padding = 0;
	int spacedPadding = 0;
	int margin = 0;
	int spacedMargin = 0;

	bool visible = true;
	bool pointerEvents = true;
	int zIndex = 0;
	bool render = true;

	std::set<std::string> classList{};

	//styles
	GUIStyles::Position position = GUIStyles::Position::relative;
	GUIStyles::Flex direction = GUIStyles::Flex::row;
	GUIStyles::Flex grow = GUIStyles::Flex::none;
	GUIStyles::Flex align = GUIStyles::Flex::none;
	GUIStyles::Flex wrap = GUIStyles::Flex::none;
	GUIStyles::Color color{};
	GUIStyles::Scroll scroll = GUIStyles::Scroll::hidden;
	GUIStyles::FontStyle font{};

	std::string id = "";
	std::vector<GUIElement*> children{};
	GUIElement* parent = nullptr;

	GUIElement();
	~GUIElement();

	void setWidth(int x, char mode = '0', int x2 = 0, char mode2 = '0');
	void setHeight(int y, char mode = '0', int y2 = 0, char mode2 = '0');
	void setDims(GUIStyles::PositionArguments x);
	void setColor(int r, int g, int b);
	void setLeft(int x, char mode = '0', int x2 = 0, char mode2 = '0');
	void setTop(int y, char mode = '0', int y2 = 0, char mode2 = '0');
	void setText(std::string text);
	const std::string& getText();

	void move(int dX, int dY, int borderLeft = 0, int borderTop = 0, int borderRight = 2000, int borderBottom = 2000);
	//move with automatic border checking
	void drag(int dX, int dY);
	
	void show();
	void hide();
	void toggleVisibility();

	//how much space it takes up in pixels
	int computeWidth();
	int computeHeight();

	//gets GUIElement to apply styles to
	virtual GUIElement* style();

	//update display
	virtual void update();

	virtual void onclick(MouseEvent& e);
	virtual void onscroll(MouseEvent& e);
	virtual void onhover(MouseEvent& e);

	virtual void appendChild(GUIElement* GUIElement);
	virtual void deleteChildren();
	void moveToTop();

	int computeTop();
	int computeLeft();
};