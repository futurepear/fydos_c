#include "element.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "../renderer/text/textRenderer.h"

using namespace GUIStyles;

GUIElement::GUIElement() {
	width = new Pixel{ 0 };
	height = new Pixel{ 0 };
	top = new Pixel{ 0 };
	left = new Pixel{ 0 };
}

GUIElement::~GUIElement() {
	delete width;
	delete height;
	delete top;
	delete left;

	/*if (!id.empty())
		GUIElementIDcache.erase(id);*/

	//recurse thru children and delete them
	deleteChildren();
}

void GUIElement::setMeasurement(Measurement*& x, int a, char modeA, int b, char modeB) {
	char currentMode = x->mode;
	if (modeA == '0') modeA = currentMode;

	if (modeB != '0') { //calc
		delete x;
		Measurement* m1 = GUIStyles::createMeasurement(a, modeA);
		Measurement* m2 = GUIStyles::createMeasurement(b, modeB);
		x = new Calc(m1, m2);
		return;
	}

	if (modeA == currentMode) {
		x->value = a;
	}
	else {
		delete x;
		x = GUIStyles::createMeasurement(a, modeA);
	}
}

void GUIElement::setWidth(int x, char mode, int x2, char mode2) {
	setMeasurement(width, x, mode, x2, mode2);
	needsRecomputation = true;
}
void GUIElement::setHeight(int y, char mode, int y2, char mode2) {
	setMeasurement(height, y, mode, y2, mode2);
	needsRecomputation = true;
}
void GUIElement::move(int dX, int dY, int borderLeft, int borderTop, int borderRight, int borderBottom) {
	int x = std::clamp(left->value + dX, borderLeft, borderRight);
	int y = std::clamp(top->value + dY, borderTop, borderBottom);

	setLeft(x);
	setTop(y);
}

void GUIElement::setLeft(int x, char mode, int x2, char mode2) {
	setMeasurement(left, x, mode, x2, mode2);
	needsRecomputation = true;
}
void GUIElement::setTop(int y, char mode, int y2, char mode2) {
	setMeasurement(top, y, mode, y2, mode2);
	needsRecomputation = true;
}
void GUIElement::setDims(PositionArguments x) {
	setLeft(x.left);
	setTop(x.top);
	setWidth(x.width);
	setHeight(x.height);
	needsRecomputation = true;
}
void GUIElement::setColor(int r, int g, int b) {
	color.r = r;
	color.g = g;
	color.b = b;
}
void GUIElement::setText(std::string text) {
	innerText = text;
	needsRecomputation = true;
}
const std::string& GUIElement::getText() {
	return innerText;
}
void GUIElement::show() {
	visible = true;
}
void GUIElement::hide() {
	visible = false;
}
void GUIElement::toggleVisibility() {
	visible = !visible;
}

int GUIElement::computeWidth() {
	int parentWidth = 0;
	if (parent != nullptr) parentWidth = parent->offsetWidth;
	return width->compute(parentWidth) + spacedMargin * 2;
}
int GUIElement::textHeight() {
	int lineHeight = font.lineHeight;
	int height = Fonts::FiraSans.maxHeight(font.size);
	int width = 0;
	for (const char& c : innerText) {
		int w = Fonts::FiraSans.charWidth(c, 24);
		if (width + w > offsetWidth) {
			height += lineHeight;
			width = w;
		} else {
			width += w;
		}
	}
	return height;
}
int GUIElement::computeHeight() {
	int parentHeight = 0;
	if (parent != nullptr) parentHeight = parent->offsetHeight;
	return std::max(height->compute(parentHeight), textHeight()) + spacedMargin * 2;
}
int GUIElement::computeTop() {
	int parentTop = 0;
	int parentHeight = 0;
	if (parent != nullptr) {
		parentTop = parent->offsetTop;
		parentHeight = parent->offsetHeight;
	}
	if (position == Position::absolute) parentTop = 0;
	return top->compute(parentHeight) + parentTop;
}
int GUIElement::computeLeft() {
	int parentLeft = 0;
	int parentWidth = 0;
	if (parent != nullptr) {
		parentLeft = parent->offsetLeft;
		parentWidth = parent->offsetWidth;
	}
	if (position == Position::absolute) parentLeft = 0;
	return left->compute(parentWidth) + parentLeft;
}

void GUIElement::drag(int dX, int dY) {
	GUIElement* p = this->parent;
	move(dX, dY, 0, 0, p->offsetWidth - offsetWidth, p->offsetHeight - offsetHeight);
}

void GUIElement::update() {

}
void GUIElement::onclick(MouseEvent& e) {

}
void GUIElement::onscroll(MouseEvent& e) {

}
void GUIElement::onhover(MouseEvent& e) {

}

GUIElement* GUIElement::style() {
	return this;
}

void GUIElement::moveToTop() {
	std::vector<GUIElement*> newElemList{ parent->children.size() };

	int count = 0;
	for (int i = 0; i < parent->children.size(); i++) {
		if (parent->children[i] != this) {
			newElemList[count] = parent->children[i];
			count++;
		}
	}
	newElemList[count] = this;

	parent->children = newElemList;
}

void GUIElement::appendChild(GUIElement* GUIElement) {
	needsRecomputation = true;
	GUIElement->parent = this;
	children.push_back(GUIElement);
}
void GUIElement::deleteChildren() {
	//recursively deletes children;
	for (int i = 0; i < children.size(); i++) {
		delete children[i];
	}
	//now clear vector
	children.clear();
}