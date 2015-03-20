#include "Color.h"

SDL_Color Color::GetSDLColor() const
{
	return{ m_data.r, m_data.g, m_data.b, m_data.a };
}

uint32_t Color::GetInteger() const
{
	return m_data.i;
}