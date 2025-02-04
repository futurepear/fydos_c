#include "components.h"
#include "core.h"
#include <cmath>
#include <format>

class CloseButton : public GUIElement {
private:
	GUIElement* target;
public:
	CloseButton(GUIElement* elem) : target{ elem } {
		setColor(150, 0, 0);
	}
	void onclick(MouseEvent& e) {
		//GUI::deleteElement(target);
	}
};

class InventorySlot : public GUIElement {
private:
	ItemBuffer* linkedInventory; //the inventory object its linked to
	int itemID;
	int position;  //the literal physical position of the slot where its rendering
public:
	InventorySlot(ItemBuffer* inventory, int id, int i) : linkedInventory{ inventory }, itemID{ id }, position{ i } {};

	//Clicking on item slot in inventory
	void onclick(MouseEvent& e) {
		const Item item = linkedInventory->at(position);
		
		e.input.storageInteraction = true;
		e.input.cursor.storage = linkedInventory;
		e.input.cursor.itemID = item.id();
		e.input.cursor.position = position;
	}
};

class InventoryElement : public GUIElement {
private:
	ItemBuffer* linkedInventory;
	int stride = 5;
	int slots = 0;
	int emptySlots = 0;
	int a = 0;
	std::string title;
	bool hasCloseButton;
public:
	InventoryElement(ItemBuffer* inventory, std::string _title = "Storage", bool closeButton = true) : linkedInventory{ inventory }, title{ _title }, hasCloseButton{ closeButton } {
		this->wrap = GUIStyles::Flex::wrap;
		this->classList.insert("storage");
		update();
	}
	void update() {
		int L = linkedInventory->displaySize(stride);
		if (!linkedInventory->changed && (L == slots + emptySlots && linkedInventory->size() == slots)) return; //only update when needed
		linkedInventory->changed = false;
		deleteChildren();
		needsRecomputation = true;

		int closeButtonSpace = 30;
		if (!hasCloseButton) closeButtonSpace = 0;

		std::string weight = std::format("{:.2f}", linkedInventory->totalWeight());
		std::string maxWeight = std::format("{:.2f}", linkedInventory->maxWeight());

		GUIElement* infoBar = GUI::createElement();
		infoBar->setWidth(100, '%', -closeButtonSpace, 'p');
		infoBar->setHeight(30, 'p');
		infoBar->font.size = 24;
		infoBar->setText(title + " - " + weight + "/" + maxWeight);
		infoBar->font.size = 20;
		infoBar->pointerEvents = false;
		this->appendChild(infoBar);

		GUIElement* closeButton = new CloseButton(this);
		closeButton->setDims({ 0, 0, 30, 30 });
		if(hasCloseButton)
			this->appendChild(closeButton);


		slots = 0;
		emptySlots = 0;
		for (int i = 0; i < L; i++) {
			const Item& item = linkedInventory->at(i);
			GUIElement* slot = new InventorySlot{ linkedInventory, item.id(), i };
			slot->setWidth(100 / stride, '%');
			slot->setHeight(50);
			slot->margin = 3;
			if(!item.empty())
				slot->setText(item.getName() + std::string(": ") + std::to_string(item.getQuantity()));
			slot->font.size = 18;
			slot->font.lineHeight = 20;
			

			if (item.empty()) {
				slot->setColor(100, 100, 100);
				emptySlots++;
			} else {
				slots++;
			}
			this->appendChild(slot);
		}
	}
	void onclick(MouseEvent& e) {
	}
	const ItemBuffer* storage() {
		return linkedInventory;
	}
};

class HotbarSlot : public GUIElement {
private:
	int slot;
public:
	HotbarSlot(int x) : slot{ x } {}
	void onclick(MouseEvent& e) {
		if (slot == 9) return; //empty slot

		CursorCommand& cmd = e.inventory.cursorCommand;
		cmd.update();
		if (cmd.bound()) {
			e.inventory.setHotbarItem(slot, cmd.id);
		}

		e.inventory.switchSlot(slot);
	}
};

class Hotbar : public GUIElement {
private:
public:
	Hotbar(int size) {
		for (int i = 0; i < 10; i++) {
			GUIElement* slot = new HotbarSlot{ i };
			slot->setWidth(size);
			slot->setHeight(size);
			slot->margin = 3;
			slot->align = GUIStyles::Flex::all;
			GUI::setID(slot, "slot" + std::to_string(i));
			this->appendChild(slot);
		}
	}
};

class BasicCrafter : public GUIElement {
private:
	GUIElement* progressBar;
public:
	Crafter* crafter; //move this to private
	BasicCrafter(Crafter* ref, GUIElement* _progressBar) : crafter{ ref }, progressBar{ _progressBar } {
	}
	void update() {
		progressBar->setWidth(static_cast<int>(crafter->progress() * 100), '%');
		progressBar->needsRecomputation = true;
	}
};

class RecipeButton : public GUIElement {
private:
	Crafter* linkedCrafter;
	int recipeID;
public:
	RecipeButton(int id, Crafter* crafter) : recipeID{ id }, linkedCrafter{ crafter } {

	}
	void onclick(MouseEvent& e) {
		std::cout << "clicked recipe: " << recipeID << "\n";
		e.input.crafts.push_back({ linkedCrafter, recipeID });
	}
};

Components::RecipeBrowser::RecipeBrowser(Crafter* ref) : crafter{ ref } {
	GUIElement* title = GUI::createElement();
	title->setText("Recipes");
	title->setHeight(30, 'p');
	title->setWidth(100, '%');
	title->font.size = 24;
	title->pointerEvents = false;
	this->classList.insert("recipeBrowser");

	recipeList = Components::scrollingContainer();
	recipeList->style()->wrap = GUIStyles::Flex::wrap;
	recipeList->setHeight(100, '%', -30, 'p');
	recipeList->setWidth(100, '%');

	this->direction = GUIStyles::Flex::column;
	this->appendChild(title);
	this->appendChild(recipeList);
}

void Components::RecipeBrowser::updateRecipeList(std::vector<const ItemBuffer*> recipeStorageInputs) {
	recipeList->deleteChildren();

	std::vector<int> recipes = crafter->findRecipes(recipeStorageInputs);

	for (int i = 0; i < recipes.size(); i++) {
		const auto& recipe = Dictionary::recipes[recipes[i]];

		GUIElement* item = new RecipeButton(recipe.id, crafter);
		item->setWidth(90, '%');
		item->setText(recipe.processName);

		recipeList->appendChild(item);
	}
}



//Parent must ALWAYS be a scrolling container
class Scrollbar : public GUIElement {
private:
	GUIElement* scroller;
	int m_scrollerHeightRatio = 0;
	float scrollPercentage = 0;
public:
	Scrollbar() {
		GUIElement* scroll = GUI::createElement();
		scroll->setWidth(100, '%');
		scroll->setHeight(10, '%');
		scroll->classList.insert("draggable");
		this->appendChild(scroll);
		scroller = scroll;
	}
	void scrollDown(int amount) {
		scroller->drag(0, amount);
	}
	void update() {
		int pScrollHeight = parent->style()->scrollHeight - parent->style()->offsetHeight;

		int heightRatio = 100;
		if (parent->style()->scrollHeight > parent->style()->offsetHeight) {
			heightRatio = 100 * parent->style()->offsetHeight / parent->style()->scrollHeight;
		}
		
		if (heightRatio > 100) heightRatio = 100;
		if (heightRatio != m_scrollerHeightRatio) {
			m_scrollerHeightRatio = heightRatio;
			scroller->setHeight(heightRatio, '%');
			scroller->needsRecomputation = true;
		}
		float maxY = this->offsetHeight - scroller->offsetHeight;
		float newScrollPercentage = 100 * (scroller->offsetTop - this->offsetTop ) / maxY;
		if (std::abs(newScrollPercentage - scrollPercentage) > 0.01) {
			scrollPercentage = newScrollPercentage;
			parent->style()->scrollTop = -pScrollHeight * newScrollPercentage / 100;
			needsRecomputation = true;
		}
	}
};

//width must stay constant..
class ScrollingContainer : public GUIElement {
private:
	GUIElement* innerContainer;
	Scrollbar* m_scrollbar;
public:
	ScrollingContainer() {
		int scrollWidth = 20;
		GUIElement* container = GUI::createElement();
		container->setWidth(100, '%', -scrollWidth, 'p');
		container->setHeight(100, '%');
		container->pointerEvents = false;
		GUIElement* scrollbar = new Scrollbar();
		scrollbar->setWidth(scrollWidth);
		scrollbar->setHeight(100, '%');
		scrollbar->setColor(50, 50, 50);
		GUIElement::appendChild(container);
		GUIElement::appendChild(scrollbar);

		innerContainer = container;
		m_scrollbar = reinterpret_cast<Scrollbar*>(scrollbar);

		this->scroll = GUIStyles::Scroll::scroll;
	}
	GUIElement* style() {
		return innerContainer;
	}
	void appendChild(GUIElement* GUIElement) {
		innerContainer->appendChild(GUIElement);
	}
	void deleteChildren() {
		innerContainer->deleteChildren();
	}
	void onscroll(MouseEvent& e) {
		m_scrollbar->scrollDown(10 * e.scrollY);
	}
};

GUIElement* Components::scrollingContainer() {
	return new ScrollingContainer{};
}

const ItemBuffer* Components::getContainerStorage(GUIElement* elem) {
	return reinterpret_cast<InventoryElement*>(elem)->storage();
}

GUIElement* Components::hotbar() {
	int size = 75;
	GUIElement* hotbar = new Hotbar(size);
	hotbar->setWidth(size * 10 + 6, 'p');
	hotbar->setHeight(size + 6, 'p');
	hotbar->setLeft(50, '%', -(size * 10 + 6) / 2, 'p');
	hotbar->setTop(100, '%', -size - 6, 'p');
	hotbar->align = GUIStyles::Flex::all;
	hotbar->position = GUIStyles::Position::absolute;
	GUI::setID(hotbar, "hotbar");
	return hotbar;
}

GUIElement* Components::inventory(ItemBuffer* inventoryObject) {
	GUIElement* inventory = new InventoryElement(inventoryObject);
	inventory->setWidth(25, '%');
	inventory->setHeight(50, '%');
	inventory->position = GUIStyles::Position::absolute;
	inventory->classList.insert("draggable");
	return inventory;
}

GUIElement* Components::basicCraftingMenu(ItemBuffer* input, ItemBuffer* output, Crafter* crafter) {
	GUIElement* progress = GUI::createElement();

	int h = 600;
	int w = 500;
	int rw = 500;

	GUIElement* outerContainer = GUI::createElement();
	outerContainer->setDims({ 0, 0, w + rw, h });
	outerContainer->position = GUIStyles::Position::absolute;
	outerContainer->classList.insert("draggable");

	GUIElement* container = new BasicCrafter(crafter, progress);
	container->direction = GUIStyles::Flex::column;
	container->setDims({ 0, 0, w, h });
	container->pointerEvents = false;

	GUIElement* bar = GUI::createElement();
	bar->setWidth(100, '%');
	bar->setHeight(30, 'p');
	bar->pointerEvents = false;
	bar->setText("Crafter");
	bar->pointerEvents = false;

	GUIElement* innerContainer = GUI::createElement();
	innerContainer->setWidth(100, '%');
	innerContainer->setHeight(h - 40 - 30, 'p');

	GUIElement* inputContainer = new InventoryElement(input, "Input", false);
	GUIElement* outputContainer = new InventoryElement(output, "Output", false);
	inputContainer->setWidth(50, '%');
	inputContainer->setHeight(100, '%');
	outputContainer->setWidth(50, '%');
	outputContainer->setHeight(100, '%');

	GUIElement* bottomContainer = GUI::createElement();
	bottomContainer->setDims({ 0, 0, w, 40 });
	bottomContainer->align = GUIStyles::Flex::all;

	GUIElement* progressOuter = GUI::createElement();
	progressOuter->setWidth(90, '%');
	progressOuter->setHeight(90, '%');
	progressOuter->setColor(50, 50, 50);
	progress->setWidth(60, '%');
	progress->setHeight(100, '%');
	progress->setColor(50, 200, 50);

	GUIElement* recipes = new RecipeBrowser(crafter);
	recipes->setDims({ 0, 0, rw, h });
	recipes->pointerEvents = false;

	bottomContainer->appendChild(progressOuter);
	progressOuter->appendChild(progress);
	innerContainer->appendChild(inputContainer);
	innerContainer->appendChild(outputContainer);

	container->appendChild(bar);
	container->appendChild(innerContainer);
	container->appendChild(bottomContainer);
	outerContainer->appendChild(container);
	outerContainer->appendChild(recipes);

	return outerContainer;
}