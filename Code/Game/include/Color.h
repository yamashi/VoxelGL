#pragma once

class Color
{
public:

	union Data
	{
		uint32_t i;
		struct
		{
			char r;
			char g;
			char b;
			char a;
		};
	};

	Color(){ m_data.i = 0; }
	~Color(){}

	uint32_t GetInteger() const;
	SDL_Color GetSDLColor() const;

	Data& Get() { return m_data; }

private:

	
	Data m_data;
};