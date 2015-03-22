#include "UIElement.h"

void UIElement::Draw(const UIRenderContext& acRenderContext)
{
	auto itor = m_childrenList.begin();
	auto end = m_childrenList.end();

	for (; itor != end; ++itor)
	{
		(*itor)->Draw(acRenderContext);
	}
}

void UIElement::AddChild(const UIElementPtr& acElement)
{
	m_childrenList.push_back(acElement);
}

void UIElement::RemoveChild(const UIElementPtr& acElement)
{
	m_childrenList.remove(acElement);
}