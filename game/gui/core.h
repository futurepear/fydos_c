#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <set>
#include "events.h"
#include "styling/measurements.h"
#include "styling/styles.h"
#include "element.h"
#include "../physics/physics.h"

class GUI {
private:
	static std::unordered_map<std::string, GUIElement*> elementIDcache;
	static GUIElement* draggingGUIElement;
public:

	//works with renderer to find when resolution changes
	static bool resolutionChange;
	static GUIElement root;
	static void setID(GUIElement* GUIElement, std::string id);
	static GUIElement* $(const std::string& id);
	static std::vector<GUIElement*> getElementsByClassName(std::string className);
	static GUIElement* createElement();
	static void compute(GUIElement* elem);
	static void deleteElement(GUIElement* elem);
	static bool findTargetGUIElement(MouseEvent& e, Vector<int> cursorPosition);
	static void handleMouseEvent(MouseEvent& e, GUIElement* target);

	template <typename F> static void recurse(GUIElement* elem, F callback) {
		auto& children = elem->children;
		for (int i = 0; i < children.size(); i++) {
			auto elem = children[i];
			bool go = callback(elem); //if true, move on, if false, dont continue
			if(go)
				recurse(elem, callback);
		}
	}
};