#pragma once

enum DisplayType
{
	DISPLAYTYPE_NONE,
	DISPLAYTYPE_GDI,	  // Both 1 and 2 are called "Software" in stats.
	DISPLAYTYPE_SOFTWARE, // Both 1 and 2 are called "Software" in stats. This one was called "UNIV" in 1.00 stats.
	DISPLAYTYPE_DDRAW,
	DISPLAYTYPE_GLIDE,
	DISPLAYTYPE_OPENGL,
	DISPLAYTYPE_DIRECT3D,
	DISPLAYTYPE_RAVE,
	NUMBER_OF_DISPLAYTYPES
};
