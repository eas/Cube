#pragma once

#define COLOR_DEFINE(name, r, g, b) \
	const DWORD name = D3DCOLOR_XRGB( (r), (g), (b) );

COLOR_DEFINE( Black,	000, 000, 000 )
COLOR_DEFINE( Red,		255, 000, 000 )
COLOR_DEFINE( Green,	000, 255, 000 )
COLOR_DEFINE( Blue,		000, 000, 255 )
COLOR_DEFINE( Yellow,	255, 255, 000 )
COLOR_DEFINE( Cyan,		000, 255, 255 )
COLOR_DEFINE( Magenta,	255, 000, 255 )
COLOR_DEFINE( White,	255, 255, 255 )

COLOR_DEFINE( Gray,		128, 128, 128 )
#undef COLOR_DEFINE