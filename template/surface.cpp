// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"

#include "Files.h" 
#include "Image.h" 
#include "../lib/stb_image.h"

using namespace Tmpl8;

// Surface class implementation

Surface::Surface( int w, int h, uint* b ) : pixels( b ), width( w ), height( h ) {}

Surface::Surface( int w, int h ) : width( w ), height( h )
{
	pixels = (uint*)MALLOC64( w * h * sizeof( uint ) );
	ownBuffer = true; // needs to be deleted in destructor
}
Surface::Surface( const char* file ) : pixels( 0 ), width( 0 ), height( 0 )
{
	// check if file exists; show an error if there is a problem
	FILE* f = fopen( file, "rb" );
	if (!f) FatalError( "File not found: %s", file );
	fclose( f );
	// load the file
	Surface::LoadFromFile( file );
}

void Surface::LoadFromFile( const char* file )
{
	// use stb_image to load the image file
	int n;
	unsigned char* data = stbi_load( file, &width, &height, &n, 0 );
	if (!data) return; // load failed
	pixels = (uint*)MALLOC64( width * height * sizeof( uint ) );
	ownBuffer = true; // needs to be deleted in destructor
	const int s = width * height;
	if (n == 1) /* greyscale */ for (int i = 0; i < s; i++)
	{
		const unsigned char p = data[i];
		pixels[i] = p + (p << 8) + (p << 16);
	}
	else
	{
		for (int i = 0; i < s; i++) pixels[i] = (data[i * n + 0] << 16) + (data[i * n + 1] << 8) + data[i * n + 2];
	}
	// free stb_image data
	stbi_image_free( data );
}

Surface::~Surface()
{
	if (ownBuffer) FREE64( pixels ); // free only if we allocated the buffer ourselves
}

void Surface::Clear( uint c )
{
	// WARNING: not the fastest way to do this.
	const int s = width * height;
	for (int i = 0; i < s; i++) pixels[i] = c;
}

void Surface::Plot( int x, int y, uint c )
{
	if (x < 0 || y < 0 || x >= width || y >= height) return;
	pixels[x + y * width] = c;
}

void Surface::Box( int x1, int y1, int x2, int y2, uint c )
{
	Line( (float)x1, (float)y1, (float)x2, (float)y1, c );
	Line( (float)x2, (float)y1, (float)x2, (float)y2, c );
	Line( (float)x1, (float)y2, (float)x2, (float)y2, c );
	Line( (float)x1, (float)y1, (float)x1, (float)y2, c );
}

void Surface::Bar( int x1, int y1, int x2, int y2, uint c )
{
	// clipping
	if (x1 < 0) x1 = 0;
	if (x2 >= width) x2 = width - 1;
	if (y1 < 0) y1 = 0;
	if (y2 >= height) y2 = width - 1;
	// draw clipped bar
	uint* a = x1 + y1 * width + pixels;
	for (int y = y1; y <= y2; y++)
	{
		for (int x = 0; x <= (x2 - x1); x++) a[x] = c;
		a += width;
	}
}

// Surface::Print: Print some text with the hard-coded mini-font.
void Surface::Print( const char* s, int x1, int y1, uint c )
{
	if (!fontInitialized)
	{
		// we will initialize the font on first use
		InitCharset();
		fontInitialized = true;
	}
	uint* t = pixels + x1 + y1 * width;
	for (int i = 0; i < (int)(strlen( s )); i++, t += 6)
	{
		int pos = 0;
		if ((s[i] >= 'A') && (s[i] <= 'Z')) pos = transl[(unsigned short)(s[i] - ('A' - 'a'))];
		else pos = transl[(unsigned short)s[i]];
		uint* a = t;
		const char* u = (const char*)font[pos];
		for (int v = 0; v < 5; v++, u++, a += width)
			for (int h = 0; h < 5; h++) if (*u++ == 'o') *(a + h) = c, * (a + h + width) = 0;
	}
}

// Surface::Line: Draw a line between the specified screen coordinates.
// Uses clipping for lines that are partially off-screen. Not efficient.
void Surface::Line( float x1, float y1, float x2, float y2, uint c )
{
	// clip (Cohen-Sutherland, https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm)
	const float xmin = 0, ymin = 0, xmax = (float)width - 1, ymax = (float)height - 1;
	int c0 = OUTCODE( x1, y1 ), c1 = OUTCODE( x2, y2 );
	bool accept = false;
	while (1)
	{
		if (!(c0 | c1)) { accept = true; break; }
		else if (c0 & c1) break; else
		{
			float x = 0, y = 0;
			const int co = c0 ? c0 : c1;
			if (co & 8) x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1), y = ymax;
			else if (co & 4) x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1), y = ymin;
			else if (co & 2) y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1), x = xmax;
			else if (co & 1) y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1), x = xmin;
			if (co == c0) x1 = x, y1 = y, c0 = OUTCODE( x1, y1 );
			else x2 = x, y2 = y, c1 = OUTCODE( x2, y2 );
		}
	}
	if (!accept) return;
	float b = x2 - x1, h = y2 - y1, l = fabsf( b );
	if (fabsf( h ) > l) l = fabsf( h );
	int il = (int)l;
	float dx = b / (float)l, dy = h / (float)l;
	for (int i = 0; i <= il; i++, x1 += dx, y1 += dy)
		*(pixels + (int)x1 + (int)y1 * width) = c;
}

// Surface::CopyTo: Copy the contents of one Surface to another, at the specified
// location. With clipping.
void Surface::CopyTo( Surface* d, int x, int y )
{
	uint* dst = d->pixels;
	uint* src = pixels;
	if ((src) && (dst))
	{
		int srcwidth = width;
		int srcheight = height;
		int dstwidth = d->width;
		int dstheight = d->height;
		if ((srcwidth + x) > dstwidth) srcwidth = dstwidth - x;
		if ((srcheight + y) > dstheight) srcheight = dstheight - y;
		if (x < 0) src -= x, srcwidth += x, x = 0;
		if (y < 0) src -= y * width, srcheight += y, y = 0;
		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += x + dstwidth * y;
			for (int i = 0; i < srcheight; i++)
			{
				memcpy( dst, src, srcwidth * 4 );
				dst += dstwidth, src += width;
			}
		}
	}
}

void Surface::SetChar( int c, const char* c1, const char* c2, const char* c3, const char* c4, const char* c5 )
{
	strcpy( font[c][0], c1 );
	strcpy( font[c][1], c2 );
	strcpy( font[c][2], c3 );
	strcpy( font[c][3], c4 );
	strcpy( font[c][4], c5 );
}

void Surface::Render(Surface* screen, int x, int y)
{
	for (int xx = 0; xx < width; xx++) 
	{
		for (int yy = 0; yy < height; yy++) 
		{
			// if a pixel is perfectly black it doesn't get drawn
			const uint c = pixels[xx + yy * width]; 
			if (c & 0xffffff) screen->Plot(x + xx, y + yy, c); 
		}
	}
}

void Surface::InitCharset()
{
	SetChar( 0, ":ooo:", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 1, "oooo:", "o:::o", "oooo:", "o:::o", "oooo:" );
	SetChar( 2, ":oooo", "o::::", "o::::", "o::::", ":oooo" );
	SetChar( 3, "oooo:", "o:::o", "o:::o", "o:::o", "oooo:" );
	SetChar( 4, "ooooo", "o::::", "oooo:", "o::::", "ooooo" );
	SetChar( 5, "ooooo", "o::::", "ooo::", "o::::", "o::::" );
	SetChar( 6, ":oooo", "o::::", "o:ooo", "o:::o", ":ooo:" );
	SetChar( 7, "o:::o", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 8, "::o::", "::o::", "::o::", "::o::", "::o::" );
	SetChar( 9, ":::o:", ":::o:", ":::o:", ":::o:", "ooo::" );
	SetChar( 10, "o::o:", "o:o::", "oo:::", "o:o::", "o::o:" );
	SetChar( 11, "o::::", "o::::", "o::::", "o::::", "ooooo" );
	SetChar( 12, "oo:o:", "o:o:o", "o:o:o", "o:::o", "o:::o" );
	SetChar( 13, "o:::o", "oo::o", "o:o:o", "o::oo", "o:::o" );
	SetChar( 14, ":ooo:", "o:::o", "o:::o", "o:::o", ":ooo:" );
	SetChar( 15, "oooo:", "o:::o", "oooo:", "o::::", "o::::" );
	SetChar( 16, ":ooo:", "o:::o", "o:::o", "o::oo", ":oooo" );
	SetChar( 17, "oooo:", "o:::o", "oooo:", "o:o::", "o::o:" );
	SetChar( 18, ":oooo", "o::::", ":ooo:", "::::o", "oooo:" );
	SetChar( 19, "ooooo", "::o::", "::o::", "::o::", "::o::" );
	SetChar( 20, "o:::o", "o:::o", "o:::o", "o:::o", ":oooo" );
	SetChar( 21, "o:::o", "o:::o", ":o:o:", ":o:o:", "::o::" );
	SetChar( 22, "o:::o", "o:::o", "o:o:o", "o:o:o", ":o:o:" );
	SetChar( 23, "o:::o", ":o:o:", "::o::", ":o:o:", "o:::o" );
	SetChar( 24, "o:::o", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar( 25, "ooooo", ":::o:", "::o::", ":o:::", "ooooo" );
	SetChar( 26, ":ooo:", "o::oo", "o:o:o", "oo::o", ":ooo:" );
	SetChar( 27, "::o::", ":oo::", "::o::", "::o::", ":ooo:" );
	SetChar( 28, ":ooo:", "o:::o", "::oo:", ":o:::", "ooooo" );
	SetChar( 29, "oooo:", "::::o", "::oo:", "::::o", "oooo:" );
	SetChar( 30, "o::::", "o::o:", "ooooo", ":::o:", ":::o:" );
	SetChar( 31, "ooooo", "o::::", "oooo:", "::::o", "oooo:" );
	SetChar( 32, ":oooo", "o::::", "oooo:", "o:::o", ":ooo:" );
	SetChar( 33, "ooooo", "::::o", ":::o:", "::o::", "::o::" );
	SetChar( 34, ":ooo:", "o:::o", ":ooo:", "o:::o", ":ooo:" );
	SetChar( 35, ":ooo:", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar( 36, "::o::", "::o::", "::o::", ":::::", "::o::" );
	SetChar( 37, ":ooo:", "::::o", ":::o:", ":::::", "::o::" );
	SetChar( 38, ":::::", ":::::", "::o::", ":::::", "::o::" );
	SetChar( 39, ":::::", ":::::", ":ooo:", ":::::", ":ooo:" );
	SetChar( 40, ":::::", ":::::", ":::::", ":::o:", "::o::" );
	SetChar( 41, ":::::", ":::::", ":::::", ":::::", "::o::" );
	SetChar( 42, ":::::", ":::::", ":ooo:", ":::::", ":::::" );
	SetChar( 43, ":::o:", "::o::", "::o::", "::o::", ":::o:" );
	SetChar( 44, "::o::", ":::o:", ":::o:", ":::o:", "::o::" );
	SetChar( 45, ":::::", ":::::", ":::::", ":::::", ":::::" );
	SetChar( 46, "ooooo", "ooooo", "ooooo", "ooooo", "ooooo" );
	SetChar( 47, "::o::", "::o::", ":::::", ":::::", ":::::" ); // Tnx Ferry
	SetChar( 48, "o:o:o", ":ooo:", "ooooo", ":ooo:", "o:o:o" );
	SetChar( 49, "::::o", ":::o:", "::o::", ":o:::", "o::::" );
	char c[] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/";
	int i;
	for (i = 0; i < 256; i++) transl[i] = 45;
	for (i = 0; i < 50; i++) transl[(unsigned char)c[i]] = i;
}

void Surface8::InitPalette() 
{
	// first setting all colors to black (transparent):
	memset(PAL, 0x000000, 256 * sizeof(uint32_t));  

	// index 0 is always black (transparent):
	uint8_t idx = 1;   
	// index 1 is almost black (as alternative to black):
	PAL[idx++] = (int)(1.0f) << 16 | (int)(1.0f) << 8 | (int)(1.0f); 
	// 6 different modes per channel means 216 + 2 modes total (leaves the last 38 modus untouched): 
	float incr = 255.0f / 5.0f; 
	for (int r = 0; r < 6; r++) for (int g = 0; g < 6; g++) for (int b = 1; b < 6; b++, idx++)  
	{
		float fr = r * incr;     
		float fg = g * incr;  
		float fb = b * incr;  
		PAL[idx] = (int)fr << 16 | (int)fg << 8 | (int)fb; 
	}

#if BAKE_MODE 
	SubSurface8::savePalette = ColorPalette8("assets/color_palettes/save.cpalette");   
#endif 
}

void Surface8::InitCharset()
{
	SetChar(0, ":ooo:", "o:::o", "ooooo", "o:::o", "o:::o");
	SetChar(1, "oooo:", "o:::o", "oooo:", "o:::o", "oooo:");
	SetChar(2, ":oooo", "o::::", "o::::", "o::::", ":oooo");
	SetChar(3, "oooo:", "o:::o", "o:::o", "o:::o", "oooo:");
	SetChar(4, "ooooo", "o::::", "oooo:", "o::::", "ooooo");
	SetChar(5, "ooooo", "o::::", "ooo::", "o::::", "o::::");
	SetChar(6, ":oooo", "o::::", "o:ooo", "o:::o", ":ooo:");
	SetChar(7, "o:::o", "o:::o", "ooooo", "o:::o", "o:::o");
	SetChar(8, "::o::", "::o::", "::o::", "::o::", "::o::");
	SetChar(9, ":::o:", ":::o:", ":::o:", ":::o:", "ooo::");
	SetChar(10, "o::o:", "o:o::", "oo:::", "o:o::", "o::o:");
	SetChar(11, "o::::", "o::::", "o::::", "o::::", "ooooo");
	SetChar(12, "oo:o:", "o:o:o", "o:o:o", "o:::o", "o:::o");
	SetChar(13, "o:::o", "oo::o", "o:o:o", "o::oo", "o:::o");
	SetChar(14, ":ooo:", "o:::o", "o:::o", "o:::o", ":ooo:");
	SetChar(15, "oooo:", "o:::o", "oooo:", "o::::", "o::::");
	SetChar(16, ":ooo:", "o:::o", "o:::o", "o::oo", ":oooo");
	SetChar(17, "oooo:", "o:::o", "oooo:", "o:o::", "o::o:");
	SetChar(18, ":oooo", "o::::", ":ooo:", "::::o", "oooo:");
	SetChar(19, "ooooo", "::o::", "::o::", "::o::", "::o::");
	SetChar(20, "o:::o", "o:::o", "o:::o", "o:::o", ":oooo");
	SetChar(21, "o:::o", "o:::o", ":o:o:", ":o:o:", "::o::");
	SetChar(22, "o:::o", "o:::o", "o:o:o", "o:o:o", ":o:o:");
	SetChar(23, "o:::o", ":o:o:", "::o::", ":o:o:", "o:::o");
	SetChar(24, "o:::o", "o:::o", ":oooo", "::::o", ":ooo:");
	SetChar(25, "ooooo", ":::o:", "::o::", ":o:::", "ooooo");
	SetChar(26, ":ooo:", "o::oo", "o:o:o", "oo::o", ":ooo:");
	SetChar(27, "::o::", ":oo::", "::o::", "::o::", ":ooo:");
	SetChar(28, ":ooo:", "o:::o", "::oo:", ":o:::", "ooooo");
	SetChar(29, "oooo:", "::::o", "::oo:", "::::o", "oooo:"); 
	SetChar(30, "o::::", "o::o:", "ooooo", ":::o:", ":::o:"); 
	SetChar(31, "ooooo", "o::::", "oooo:", "::::o", "oooo:"); 
	SetChar(32, ":oooo", "o::::", "oooo:", "o:::o", ":ooo:"); 
	SetChar(33, "ooooo", "::::o", ":::o:", "::o::", "::o::"); 
	SetChar(34, ":ooo:", "o:::o", ":ooo:", "o:::o", ":ooo:"); 
	SetChar(35, ":ooo:", "o:::o", ":oooo", "::::o", ":ooo:"); 
	SetChar(36, "::o::", "::o::", "::o::", ":::::", "::o::"); 
	SetChar(37, ":ooo:", "::::o", ":::o:", ":::::", "::o::"); 
	SetChar(38, ":::::", ":::::", "::o::", ":::::", "::o::"); 
	SetChar(39, ":::::", ":::::", ":ooo:", ":::::", ":ooo:"); 
	SetChar(40, ":::::", ":::::", ":::::", ":::o:", "::o::"); 
	SetChar(41, ":::::", ":::::", ":::::", ":::::", "::o::"); 
	SetChar(42, ":::::", ":::::", ":ooo:", ":::::", ":::::"); 
	SetChar(43, ":::o:", "::o::", "::o::", "::o::", ":::o:"); 
	SetChar(44, "::o::", ":::o:", ":::o:", ":::o:", "::o::"); 
	SetChar(45, ":::::", ":::::", ":::::", ":::::", ":::::"); 
	SetChar(46, "ooooo", "ooooo", "ooooo", "ooooo", "ooooo"); 
	SetChar(47, "::o::", "::o::", ":::::", ":::::", ":::::"); // Tnx Ferry 
	SetChar(48, "o:o:o", ":ooo:", "ooooo", ":ooo:", "o:o:o");  
	SetChar(49, "::::o", ":::o:", "::o::", ":o:::", "o::::"); 
	char c[] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/"; 
	int i; 
	for (i = 0; i < 256; i++) transl[i] = 45;  
	for (i = 0; i < 50; i++) transl[(unsigned char)c[i]] = i;  
}

void Surface8::SetChar(int c, const char* c1, const char* c2, const char* c3, const char* c4, const char* c5) 
{
	strcpy(font[c][0], c1); 
	strcpy(font[c][1], c2); 
	strcpy(font[c][2], c3); 
	strcpy(font[c][3], c4); 
	strcpy(font[c][4], c5); 
}

Surface8::Surface8(uint8_t* const buffer, int const width, int const height) :
	pixels(buffer), 
	width(width), 
	height(height) 
{}
 
Surface8::Surface8(int const width, int const height) : 
	width(width), 
	height(height)  
{
	pixels = (uint8_t*)MALLOC64(width * height * sizeof(uint8_t));
	memset(pixels, 0, width * height * sizeof(uint8_t)); 
}  

Surface8::~Surface8()
{
	FREE64(pixels); 
}

void Surface8::Print(char const* s, int const x1, int const y1, uint8_t const c) 
{
	if (!fontInitialized) 
	{
		// we will initialize the font on first use
		InitCharset(); 
		fontInitialized = true; 
	}
	uint8_t* t = pixels + x1 + y1 * width;  
	for (int i = 0; i < (int)(strlen(s)); i++, t += 6)  
	{
		int pos = 0; 
		if ((s[i] >= 'A') && (s[i] <= 'Z')) pos = transl[(unsigned short)(s[i] - ('A' - 'a'))]; 
		else pos = transl[(unsigned short)s[i]]; 
		uint8_t* a = t; 
		const char* u = (const char*)font[pos]; 
		for (int v = 0; v < 5; v++, u++, a += width) 
			for (int h = 0; h < 5; h++) if (*u++ == 'o') *(a + h) = c, *(a + h + width) = 0; 
	}
}

void Surface8::Print(char s, int const x, int const y, uint8_t const c)
{
	if (!fontInitialized)
	{
		// we will initialize the font on first use
		InitCharset();
		fontInitialized = true;
	}

	uint8_t* t = pixels + x + y * width;

	int pos = 0;
	if ((s >= 'A') && (s <= 'Z')) pos = transl[(unsigned short)(s - ('A' - 'a'))];
	else pos = transl[(unsigned short)s];
	const char* u = (const char*)font[pos];
	for (int v = 0; v < 5; v++, u++, t += width)
		for (int h = 0; h < 5; h++) if (*u++ == 'o') *(t + h) = c, * (t + h + width) = 0;
}

void Surface8::Clear(uint8_t const c)
{
	memset(pixels, c, width * height * sizeof(uint8_t));   
}

// Uses clipping for lines that are partially off-screen. Not efficient.
// TODO try to remove this^ when project is done and working:
void Surface8::Line(float x1, float y1, float x2, float y2, uint8_t const c)  
{
	// clip (Cohen-Sutherland, https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm) 
	const float xmin = 0, ymin = 0, xmax = (float)width - 1, ymax = (float)height - 1; 
	int c0 = OUTCODE(x1, y1), c1 = OUTCODE(x2, y2); 
	bool accept = false; 
	while (1) 
	{
		if (!(c0 | c1)) { accept = true; break; } 
		else if (c0 & c1) break; else 
		{
			float x = 0, y = 0; 
			const int co = c0 ? c0 : c1; 
			if (co & 8) x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1), y = ymax; 
			else if (co & 4) x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1), y = ymin; 
			else if (co & 2) y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1), x = xmax; 
			else if (co & 1) y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1), x = xmin; 
			if (co == c0) x1 = x, y1 = y, c0 = OUTCODE(x1, y1);
			else x2 = x, y2 = y, c1 = OUTCODE(x2, y2);
		}
	}
	if (!accept) return; 
	float b = x2 - x1, h = y2 - y1, l = fabsf(b); 
	if (fabsf(h) > l) l = fabsf(h); 
	int il = (int)l; 
	float dx = b / (float)l, dy = h / (float)l; 
	for (int i = 0; i <= il; i++, x1 += dx, y1 += dy) 
		*(pixels + (int)x1 + (int)y1 * width) = c; 
}

// TODO remove this if-statement if project is over and works:
void Surface8::Plot(int const x, int const y, uint8_t const c)
{
	if (x < 0 || y < 0 || x >= width || y >= height) return; 
	pixels[x + y * width] = c; 
}

void Surface8::Box(int const x1, int const y1, int const x2, int const y2, uint8_t const c) 
{
	Line((float)x1, (float)y1, (float)x2, (float)y1, c);
	Line((float)x2, (float)y1, (float)x2, (float)y2, c);
	Line((float)x1, (float)y2, (float)x2, (float)y2, c);
	Line((float)x1, (float)y1, (float)x1, (float)y2, c);
}
 
void Surface8::Bar(int x1, int y1, int x2, int y2, uint8_t const c)
{
	// clipping
	if (x1 < 0) x1 = 0; 
	if (x2 >= width) x2 = width - 1; 
	if (y1 < 0) y1 = 0;
	if (y2 >= height) y2 = width - 1; 
	// draw clipped bar
	uint8_t* a = x1 + y1 * width + pixels; 
	for (int y = y1; y <= y2; y++)
	{
		for (int x = 0; x <= (x2 - x1); x++) a[x] = c;
		a += width;
	}
}

// assumes the two surfaces are equal in size:
void Surface8::CopyTo(Surface* dst)
{   
	int idx = 0; 
	for (int x = 0; x < width; x++) for (int y = 0; y < height; y++, idx++)
	{
		dst->pixels[idx] = PAL[pixels[idx]];  
	}
}

uint8_t Surface8::FindClosestPaletteIndex(uint32_t r1, uint32_t g1, uint32_t b1)  
{
	uint8_t closestIndex = 0; 
	uint32_t smallestDiff = 765; // 255 x 3    

	for (int i = 0; i < 256; i++)
	{
		uint3 color2 = CalcColorComponents(Surface8::PAL[i]); 
		uint32_t diff = CalcColorDiff(r1, g1, b1, color2.r, color2.g, color2.b);
		if (diff < smallestDiff) 
		{
			smallestDiff = diff; 
			closestIndex = i; 
		}
	}
	return closestIndex; 
}

uint8_t ColorPalette8::FindClosestIndex(uint32_t r1, uint32_t g1, uint32_t b1) const
{
	uint8_t closestIndex = 0; 
	uint32_t smallestDiff = 765; // 255 x 3   

	for (int i = 0; i < COLOR_COUNT; i++) 
	{
		uint3 color2 = CalcColorComponents(Surface8::PAL[indices[i]]); 
		uint32_t diff = CalcColorDiff(r1, g1, b1, color2.r, color2.g, color2.b);   
		if (diff < smallestDiff)  
		{
			smallestDiff = diff;  
			closestIndex = i;  
		} 
	}
	return closestIndex;  
}    

void ColorPalette8::Cycle()
{
	uint8_t tmp = indices[0];  
	for (s32 i = 1; i < CYCLE_COUNT; i++)  
	{ 
		indices[i - 1] = indices[i]; 
	}
	indices[CYCLE_COUNT - 1] = tmp;  
} 

// save to disk:
void SubSurface8::Save(Image const* image, char const* path)  
{
#if BAKE_MODE 
	Files::PrintSaving(path);

	FILE* dst = fopen(path, "w"); if (!dst) FatalError("Couldn't open file: %s", path); 

	fprintf(dst, "%d %d\n", image->width, image->height);
	
	int idx = 0; 
	uint colorIndex;  
	for (int yy = 0; yy < image->height; yy++)
	{
		for (int xx = 0; xx < image->width; xx++, idx++)
		{
			uint32_t c = image->data[idx];
			uint3 color = CalcColorComponents(c);  
			colorIndex = savePalette.FindClosestIndex(color.r, color.g, color.b);         
			fprintf(dst, "%d ", colorIndex);
		}
		fprintf(dst, "\n"); 
	}
	fclose(dst);

	Files::PrintSuccess(path); 
#endif
}

SubSurface8* SubSurface8::LoadFromFile(char const* path) 
{
	Files::PrintLoading(path);  

	FILE* file = fopen(path, "r"); if (!file) FatalError("Couldn't open file: %s", path);

	// get size:
	int width, height; 
	if (!fscanf(file, "%d %d\n", &width, &height))
	{
		Files::PrintFailure(path); 
		return NULL;
	}
	uint8_t* buffer = new uint8_t[width * height]; 

	int idx = 0;
	for (int yy = 0; yy < height; yy++)
	{
		for (int xx = 0; xx < width; xx++, idx++)
		{
			if (!fscanf(file, "%d", &buffer[idx]))
			{
				delete[] buffer;
				Files::PrintFailure(path); 
 				return NULL; 
			}
		}
		fscanf(file, "\n"); 
	}
	fclose(file);

	SubSurface8* surf = new SubSurface8(buffer, width, height);
	Files::PrintSuccess(path);  
	return surf;
}

Surface8::SubSurface8::SubSurface8(int const width, int const height) :
	width(width),
	height(height)
{
	pixels = (uint8_t*)MALLOC64(width * height * sizeof(uint8_t));
	memset(pixels, 0, width * height * sizeof(uint8_t));
}


SubSurface8::SubSurface8(uint8_t* const buffer, int const width, int const height) :
	pixels(buffer), 
	width(width), 
	height(height), 
	palette(nullptr) 
{}

SubSurface8::~SubSurface8() 
{ 
	//delete[] pixels;  
} 

SubSurface8::SubSurface8(char const* path) 
{
	Files::PrintLoading(path);
	FILE* file = Files::ReadFile(path); 
	if (!fscanf(file, "%d %d\n", &width, &height))  
	{
		Files::PrintFailure(path);   
		return; 
	}
	pixels = new uint8_t[width * height];  

	int idx = 0;  
	for (int yy = 0; yy < height; yy++)
	{
		for (int xx = 0; xx < width; xx++, idx++) 
		{
			if (!fscanf(file, "%d", &pixels[idx])) 
			{
				delete[] pixels; pixels = nullptr; 
				Files::PrintFailure(path);  
				return; 
			}
		}
		fscanf(file, "\n"); 
	}
	fclose(file); 
	Files::PrintSuccess(path);
}

void SubSurface8::Render(Surface8* const screen, int const x, int const y) const
{
	// do not render if sprite does not have a color:
	if (!palette) return; 

	// check if the sprite is within bounds of the screen:
	if (y < -height || y >(screen->height)) return;  
	if (x < -width || x >(screen->width)) return;  

	int x1 = x, x2 = x + width; 
	int y1 = y, y2 = y + height; 
	uint8_t* src = pixels; 
	if (x1 < 0) src += -x1, x1 = 0;
	if (x2 > screen->width) x2 = screen->width;
	if (y1 < 0) src += -y1 * width, y1 = 0;
	if (y2 > screen->height) y2 = screen->height;
	uint8_t* dst = screen->pixels;  

	// check if sprite is not flat:
	if (x2 <= x1 && y2 <= y1) return;

	uint32_t dstAddr = x1 + y1 * screen->width;
	uint32_t w = x2 - x1; 
	uint32_t h = y2 - y1;
	for (int yy = 0; yy < h; yy++)
	{
		for (int xx = 0; xx < w; xx++)
		{
			// all pixels that are black (index 0) are getting discarded:
			uint8_t c = palette->indices[src[xx]];    
			if (c & 0xffffff)
			{
				dst[dstAddr + xx] = c;
			}
		}
		dstAddr += screen->width;
		src += width;
	}

#if SPRITE_MODE
	screen->Box(x, y, x + width, y + height, 80);   
#endif
}

// save to disk:
void ColorPalette8::Save(Image const* image, char const* path)
{ 
#if BAKE_MODE
	Files::PrintSaving(path); 

	if (image->width != COLOR_COUNT || image->height != 1)
	{
		Files::PrintFailure(path); 
		return;
	}

	FILE* dst = fopen(path, "w"); if (!dst) FatalError("Couldn't open file: %s", path);

	uint8_t colorIdx; 
	for (int i = 0; i < COLOR_COUNT; i++) 
	{
		uint32_t c = image->data[i]; 
		uint3 color = CalcColorComponents(c);
		colorIdx = Surface8::FindClosestPaletteIndex(color.r, color.g, color.b);    
		fprintf(dst, "%d ", colorIdx); 
	}
	fclose(dst);

	Files::PrintSuccess(path);  
#endif
} 

ColorPalette8* ColorPalette8::LoadFromFile(char const* path)
{
	Files::PrintLoading(path); 

	FILE* file = fopen(path, "r"); if (!file) FatalError("Couldn't open file: %s", path); 

	uint8_t* buffer = new uint8_t[COLOR_COUNT];  

	for (int i = 0; i < COLOR_COUNT; i++)  
	{
		if (!fscanf(file, "%d", &buffer[i]))
		{
			Files::PrintFailure(path);   
			return NULL;
		}
	}
	fclose(file); 

	ColorPalette8* palette = new ColorPalette8(buffer); 
	Files::PrintSuccess(path); 
	return palette; 
}

ColorPalette8::Palette8(uint8_t indices[COLOR_COUNT])
{
	memcpy(this->indices, indices, COLOR_COUNT * sizeof(uint8_t));   
}

ColorPalette8::Palette8(char const* path) 
{
	FILE* file = fopen(path, "r"); if (!file) FatalError("Couldn't open file: %s", path);

	for (int i = 0; i < COLOR_COUNT; i++)
	{
		if (!fscanf(file, "%d", &indices[i])) return; 
	}
	fclose(file);
}
