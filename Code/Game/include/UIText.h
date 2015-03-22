#pragma once

#include "Font.h"
#include "UIElement.h"
#include "Shader.h"
#include "Texture.h"

class UIText : public UIElement
{
public:

	UIText(UIFont& aFont);
	UIText(const UIText& aRhs);
	virtual ~UIText();

	UIText& operator=(const UIText& aRhs);

	void SetFont(UIFont& acFont);
	void SetText(const std::string& acText);

	std::string GetText() const;

	virtual void Draw(const UIRenderContext& acRenderContext);

	template<class... Args>
	static std::shared_ptr<UIText> Create(Args&&... args)
	{
		return std::make_shared<UIText>(std::forward<Args>(args)...);
	}

protected:

	void GenerateTexture();

private:

	void DoConstruct();

	GLuint m_vboId{ 0 };
	GLuint m_vaoId{ 0 };

	UIFont& m_font;
	std::string m_text;
	Texture m_texture;
};

using UITextPtr = std::shared_ptr < UIText > ;
using UITextWeakPtr = std::weak_ptr < UIText > ;