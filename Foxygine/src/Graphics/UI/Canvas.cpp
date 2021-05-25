#include "Canvas.h"
#include "../Graphics.h"



void Canvas::OnAttach()
{
	Graphics::RegisterCanvas(this);
	canvas = this;
	UIElement::SetActive(true);
	latestElementID = -1;
}


void Canvas::OnDetach()
{
	Graphics::UnregisterCanvas(this);
}


UIElement* Canvas::FindElement(std::string _name)
{
	auto index = elementNameLookup[_name];
	if (index >= 0)
		return elementMap[index];
}


UIElement* Canvas::FindElement(long id)
{
	return elementMap[id];
}


void Canvas::RegisterElement(UIElement* element)
{
	elementMap[++latestElementID] = element;
	elementNameLookup[element->name] = latestElementID;
	element->SetCanvas(this, latestElementID);
}


void Canvas::UnregisterElement(UIElement* element)
{
	long id = element->ClearCanvas(this);
	if (id != -1) {
		elementMap.erase(id);
		elementNameLookup.erase(element->name);
	}
}
