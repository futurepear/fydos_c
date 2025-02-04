#include "core.h"
#include <vector>
#include <iostream>

using namespace GUIStyles;

bool GUI::resolutionChange = false;
GUIElement GUI::root{};
std::unordered_map<std::string, GUIElement*> GUI::elementIDcache{};
GUIElement* GUI::draggingGUIElement = nullptr;

void GUI::setID(GUIElement* GUIElement, std::string id) {
	GUIElement->id = id;
	GUI::elementIDcache[id] = GUIElement;
}

GUIElement* GUI::$(const std::string& id) {
	if (GUI::elementIDcache.contains(id)) {
		return GUI::elementIDcache[id];
	} else {
		return nullptr;
	}
}

std::vector<GUIElement*> GUI::getElementsByClassName(std::string className) {
	std::vector<GUIElement*> elems{};

	GUI::recurse(&GUI::root, [&className, &elems](GUIElement* elem) {
		if (elem->classList.contains(className)) {
			elems.push_back(elem);
		}
		return true;
	});
	return elems;
}

GUIElement* GUI::createElement() {
	return new GUIElement{};
}

//TODO: make it check ids
void GUI::deleteElement(GUIElement* elem) {
	//remove all references
	auto parent = elem->parent;

	if (elem == draggingGUIElement) {
		draggingGUIElement = nullptr;
	}

	//remove all IDs
	GUI::elementIDcache.erase(elem->id);
	GUI::recurse(elem, [](GUIElement* elem) {
		GUI::elementIDcache.erase(elem->id);
		return true;
	});

	for (int i = 0; i < parent->children.size(); i++) {
		if (parent->children[i] == elem) {
			parent->children.erase(parent->children.begin() + i);
			break;
		}
	}
	delete elem;
}

//click, hover, scroll events
bool GUI::findTargetGUIElement(MouseEvent& e, Vector<int> cursorPosition) {
	//find exactly what GUIElement was cliekd fast using DFS
	GUIElement* target = nullptr;
	GUIElement* scrollTarget = nullptr;

	GUI::recurse(&GUI::root, [&cursorPosition, &target, &scrollTarget](GUIElement* elem) {
		bool collides = collisions::intersectRect({ elem->offsetLeft, elem->offsetTop }, { elem->offsetWidth, elem->offsetHeight }, cursorPosition);
		if (!elem->visible) return false;
		if (!elem->pointerEvents) return true;
		if (collides) {
			target = elem;
			if (elem->scroll == GUIStyles::Scroll::scroll) {
				scrollTarget = elem;
			}
			return true;
		} else {
			return false; //terminate search
		}
	});

	if (scrollTarget != nullptr && (e.scrollX != 0 || e.scrollY != 0)) {
		scrollTarget->onscroll(e);
	}

	if (target != nullptr && target != &root) {
		target->onhover(e);
		
		GUI::handleMouseEvent(e, target);
		if (e.click) {
			target->onclick(e);
		}
		return true;
	}
	GUI::handleMouseEvent(e, &GUI::root);
	return false; //bruh return if not equal to root 
}

void GUI::handleMouseEvent(MouseEvent& e, GUIElement* target) {
	//dragging
	if (draggingGUIElement == nullptr && e.mousedown && target->classList.contains("draggable")) {
		draggingGUIElement = target;
		target->moveToTop();
	}
	if (e.hold && draggingGUIElement != nullptr) {
		draggingGUIElement->drag(e.dX, e.dY);
	}
	if (e.mouseRelease) {
		draggingGUIElement = nullptr;
	}
}

//this monster computes all styles and positions
void GUI::compute(GUIElement* parent) {
	parent->needsRecomputation = false;
	std::vector<GUIElement*>& children = parent->children;
	int L = children.size();

	int childrenWidth = 0;
	int childrenHeight = 0;
	for (int i = 0; i < L; i++) {
		if (children[i]->position == Position::absolute) continue;
		childrenWidth += children[i]->computeWidth();
		childrenHeight += children[i]->computeHeight();
	}
	parent->scrollWidth = childrenWidth;
	

	int pW = parent->offsetWidth;
	int pH = parent->offsetHeight;

	int top = 0;// parent->offsetTop;
	int left = 0;// parent->offsetLeft;

	if (parent->direction == Flex::row && (parent->align == Flex::all || parent->align == Flex::column)) 
		left += (pW - childrenWidth) / 2;
	if (parent->direction == Flex::column && (parent->align == Flex::all || parent->align == Flex::row))
		top += (pH - childrenHeight) / 2;

	int height2 = 0;
	int maxRowHeight = 0;

	for (int i = 0; i < L; i++) {
		auto elem = children[i];
		elem->offsetWidth = elem->computeWidth() - elem->margin*2 - elem->spacedMargin * 2;
		elem->offsetHeight = elem->computeHeight() - elem->margin*2 - elem->spacedMargin * 2;
		int dTop = elem->margin + elem->spacedMargin;
		int dLeft = elem->margin + elem->spacedMargin;

		//if(elem->computeHeight() )

		height2 += elem->offsetHeight;

		if (parent->direction == Flex::row &&(parent->align == Flex::all || parent->align == Flex::row)) 
			dTop = pH / 2 - elem->offsetHeight / 2;
		if (parent->direction == Flex::column && (parent->align == Flex::all || parent->align == Flex::column))
			dLeft = pW / 2 - elem->offsetWidth / 2;

		if (elem->position == Position::absolute) {
			dTop = 0;
			dLeft = 0;
			top = 0;
			left = 0;
		}

		int computedHeight = elem->offsetHeight + dTop + elem->margin + elem->spacedMargin;
		int computedWidth = elem->offsetWidth + dLeft + elem->margin + elem->spacedMargin;
		
		if (parent->wrap == Flex::wrap && left+computedWidth > pW) {
			left = 0;
			top += maxRowHeight;
			maxRowHeight = 0;
		}

		if (maxRowHeight < computedHeight) maxRowHeight = computedHeight;

		elem->offsetTop = elem->computeTop() + dTop + top + parent->scrollTop;
		elem->offsetLeft = elem->computeLeft() + dLeft + left;
		
		if (parent->direction == GUIStyles::Flex::column && elem->position == Position::relative)
			top += computedHeight;
		if (parent->direction == GUIStyles::Flex::row && elem->position == Position::relative)
			left += computedWidth;
	}

	parent->scrollHeight = height2;

	for (int i = 0; i < L; i++) {
		auto elem = children[i];
		compute(elem);
	}
}
