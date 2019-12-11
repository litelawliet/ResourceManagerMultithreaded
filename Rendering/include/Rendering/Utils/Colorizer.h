/**
 * Copyright © 2019, ISART Digital Montreal
 * Made by Thibaut PONCHON, GP2022
 */
#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Rendering::Utils
{
	/**
	 * @brief Color enum for colors in the terminal
	 */
	enum EPencilColor : int
	{
		BLACK = 0,
		BLUE = 1,
		GREEN = 2,
		CYAN = 3,
		RED = 4,
		MAGENTA = 5,
		BROWN = 6,
		LIGHTGREY = 7,
		DARKGREY = 8,
		LIGHTBLUE = 9,
		LIGHTGREEN = 10,
		LIGHTCYAN = 11,
		LIGHTRED = 12,
		LIGHTMAGENTA = 13,
		YELLOW = 14,
		WHITE = 15,
		BLINK = 128
	};

	struct Pencil
	{
		/**
		 * @brief Allow to write anything in the console with a text color and a background color
		 * @param p_fontColor Color of the text
		 * @param p_backgroundColor Color of the background
		 */
		static void TextColor(const unsigned int p_fontColor,
		                      const unsigned int p_backgroundColor = BLACK)
		{
			static HANDLE m_screen = GetStdHandle(STD_OUTPUT_HANDLE);

			unsigned int colorAttribute = p_backgroundColor;
			colorAttribute = _rotl(colorAttribute, 4) | p_fontColor;
			SetConsoleTextAttribute(m_screen, colorAttribute);
		}
	};
}
