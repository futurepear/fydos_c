#include "renderer.h"
#include "../glHelper.h"
#include "../../gui/core.h"
#include <vector>
#include <string>
#include <cmath>

#include "../text/textRenderer.h"

//custom logic for rendering GUI GUIElements related to the game
void updateGUI(Game& game, RenderStateObject& RenderState) {

	//hotbar
	for (int i = 0; i < 10; i++) {
		GUIElement* elem = GUI::$("slot" + std::to_string(i));
		elem->setColor(128, 128, 128);
		auto& inv = game.players["me"]->inventory;
		if (inv.activeItem().id() == inv.getHotbarItem(i).id())
			elem->setColor(128, 128, 50);
		if (inv.getHotbarItem(i).empty())
			elem->setColor(100, 100, 100);
	}
}

void renderGUI(Game& game, RenderStateObject& RenderState) {
	//update all elements - this may delete or add more elements 
	GUI::recurse(&GUI::root, [](GUIElement* elem) {
		elem->update();
		return (elem->visible);
	});

	//add all elements to this vector 
	std::vector<GUIElement*> elements{};
	GUI::recurse(&GUI::root, [&elements](GUIElement* elem) {
		if (elem->visible) {
			if(elem->render)
				elements.push_back(elem);
			return true;
		} else {
			return false;
		}
	});

	//recompute GUIElements that changed
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->needsRecomputation)
			GUI::compute(elements[i]->parent);
	}
	//sort elems

	updateGUI(game, RenderState);

	RenderState.shader("gui");
	unsigned int colorLoc = glGetUniformLocation(RenderState.programs["gui"], "color");
	unsigned int cutoffLoc = glGetUniformLocation(RenderState.programs["gui"], "cutoff");
	glBindVertexArray(RenderState.geometry["box2"].position);

	for (int i = 0; i < elements.size(); i++) {
		GUIElement* elem = elements[i];
		glm::mat3 elementMatrix{};

		elementMatrix.set(elem->offsetLeft, elem->offsetTop, 0, elem->offsetWidth, elem->offsetHeight);
		
		RenderState.setTransform(elementMatrix);
		GUIStyles::Color& c = elem->color;
		float color[]{ c.r/255.0f, c.g/255.0f, c.b/255.0f, 1.0 };
		glUniform4fv(colorLoc, 1, color);

		auto parent = elem->parent;
		float cutoff[]{ parent->offsetTop, parent->offsetTop + parent->offsetHeight };
		glUniform2fv(cutoffLoc, 1, cutoff);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, RenderState.geometry["box2"].size);
	}

	//text
	RenderState.shader("text");
	RenderState.setTexture("font-serif");
	unsigned int guiColorLoc = glGetUniformLocation(RenderState.programs["text"], "color");
	unsigned int guiCutoffLoc = glGetUniformLocation(RenderState.programs["text"], "cutoff");
	unsigned int characterLoc = glGetUniformLocation(RenderState.programs["text"], "character");
	unsigned int stylesLoc = glGetUniformLocation(RenderState.programs["text"], "styles");

	

	for (int i = 0; i < elements.size(); i++) {
		GUIElement* elem = elements[i];
		const std::string& text = elem->getText();

		float offset = elem->offsetLeft;
		float line = elem->offsetTop;
		float scale = Fonts::FiraSans.fontSizeToScale(elem->font.size);
		float color[]{ 1.0f, 1.0f, 1.0f, 1.0f };
		float cutoff[]{ elem->parent->offsetTop, elem->parent->offsetTop + elem->parent->offsetHeight };
		float textStyle[]{ scale, 0.9f, 1.0f, 0.0f };

		glUniform4fv(guiColorLoc, 1, color);
		glUniform4fv(stylesLoc, 1, textStyle);
		glUniform2fv(guiCutoffLoc, 1, cutoff);

		for (const char& c : text) {
			auto& character = charToSymbol(c);
			int renderWidth = Fonts::FiraSans.charWidth(c, elem->font.size);
			if (offset-elem->offsetLeft + renderWidth > elem->offsetWidth) {
				offset = elem->offsetLeft;
				line += elem->font.lineHeight;
			}
			
			glm::mat3 textMatrix{};
			textMatrix.set(offset, line, 0, character.width * scale, character.height * scale);
			RenderState.setTransform(textMatrix);
			float charDims[]{ character.x, character.y, character.width, character.height };
			glUniform4fv(characterLoc, 1, charDims);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, RenderState.geometry["box2"].size);
			offset += renderWidth;
		}
	}
	
}