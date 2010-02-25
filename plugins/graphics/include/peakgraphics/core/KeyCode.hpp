/*
Copyright (c) 2010, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKGRAPHICS_KEYCODE_GRAPHICS_HPP_
#define _PEAKGRAPHICS_KEYCODE_GRAPHICS_HPP_

namespace peak
{
	namespace graphics
	{
		enum KeyCode
		{
			EK_MinChar = 0,
			EK_MaxChar = 0x100,
			EK_None = 0x100,
			EK_Return,
			EK_Escape,
			EK_Space,
			EK_Backspace,
			EK_Insert,
			EK_Delete,
			EK_Home,
			EK_End,
			EK_PageUp,
			EK_PageDown,
			EK_Control,
			EK_Shift,
			EK_Alt,
			EK_AltGr,
			EK_Menu,
			EK_Up,
			EK_Down,
			EK_Left,
			EK_Right,
			EK_Tab,
			EK_F1,
			EK_F2,
			EK_F3,
			EK_F4,
			EK_F5,
			EK_F6,
			EK_F7,
			EK_F8,
			EK_F9,
			EK_F10,
			EK_F11,
			EK_F12
			// TODO: Numpad
		};
	}
}

#endif
