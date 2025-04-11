// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#pragma once

namespace Tmpl8
{

// helper macro for line clipping
#define OUTCODE(x,y) (((x)<xmin)?1:(((x)>xmax)?2:0))+(((y)<ymin)?4:(((y)>ymax)?8:0))

// pixel operations

// ScaleColor: change the intensity of red, green and blue using a single
// fixed-point scale in the range 0..256, where 256 is 100%.
inline uint ScaleColor( const uint c, const uint scale )
{
	const uint rb = (((c & 0xff00ff) * scale) >> 8) & 0x00ff00ff;
	const uint ag = (((c & 0xff00ff00) >> 8) * scale) & 0xff00ff00;
	return rb + ag;
}

// AddBlend: add together two colors, with clamping.
inline uint AddBlend( const uint c1, const uint c2 )
{
	const uint r1 = (c1 >> 16) & 255, r2 = (c2 >> 16) & 255;
	const uint g1 = (c1 >> 8) & 255, g2 = (c2 >> 8) & 255;
	const uint b1 = c1 & 255, b2 = c2 & 255;
	const uint r = min( 255u, r1 + r2 );
	const uint g = min( 255u, g1 + g2 );
	const uint b = min( 255u, b1 + b2 );
	return (r << 16) + (g << 8) + b;
}

// SubBlend: subtract a color from another color, with clamping.
inline uint SubBlend( uint a_Color1, uint a_Color2 )
{
	int red = (a_Color1 & 0xff0000) - (a_Color2 & 0xff0000);
	int green = (a_Color1 & 0x00ff00) - (a_Color2 & 0x00ff00);
	int blue = (a_Color1 & 0x0000ff) - (a_Color2 & 0x0000ff);
	if (red < 0) red = 0;
	if (green < 0) green = 0;
	if (blue < 0) blue = 0;
	return (uint)(red + green + blue);
}

// Calculate the difference between all color components of two colors:
inline uint32_t CalcColorDiff(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2)
{
	return abs(r1 - r2) + abs(g1 - g2) + abs(b1 - b2);   
}

// Calculate color components in range 0-255:
inline uint3 CalcColorComponents(uint32_t color)
{
	return { (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff };
}

// Calculate color from color components in range 0-255:
inline uint32_t CalcColor(uint8_t r, uint8_t g, uint8_t b)
{
	return r << 16 | g << 8 | b; 
}

// 32-bit surface container
class Surface
{
	enum { OWNER = 1 };
public:
	// constructor / destructor
	Surface() = default;
	Surface( int w, int h, uint* buffer );
	Surface( int w, int h );
	Surface( const char* file );
	~Surface();
	// operations
	void Render(Surface* screen, int x, int y);  
	void InitCharset();
	void SetChar( int c, const char* c1, const char* c2, const char* c3, const char* c4, const char* c5 );
	void Print( const char* t, int x1, int y1, uint c );
	void Clear( uint c );
	void Line( float x1, float y1, float x2, float y2, uint c );
	void Plot( int x, int y, uint c );
	void LoadFromFile( const char* file );
	void CopyTo( Surface* dst, int x, int y );
	void Box( int x1, int y1, int x2, int y2, uint color );
	void Bar( int x1, int y1, int x2, int y2, uint color );
	// attributes
	uint* pixels = 0;
	int width = 0, height = 0;
	bool ownBuffer = false;
	// static data for the hardcoded font
	static inline char font[51][5][6];
	static inline int transl[256];
	static inline bool fontInitialized = false;
};

#include "Image.h" 

#define FONT_SIZE 6

// 8-bit (paletized) surface container
class Surface8
{
public:
	// another 8-bit (paletized) surface container with a confined palette size
	class SubSurface8
	{
	public:
		class Palette8
		{
		public:
			static uint8_t constexpr COLOR_COUNT = 8; // just for convenience sake
			static uint8_t constexpr CYCLE_COUNT = 4; // first four colors will cycle 

		public:
			uint8_t indices[COLOR_COUNT];     

		public:
			static void Save(Image const* image, char const* path); 
			static Palette8* LoadFromFile(char const* path);  

		public:
			Palette8() = default; 
			Palette8(char const* path); 
			Palette8(uint8_t indices[COLOR_COUNT]); 
			uint8_t FindClosestIndex(uint32_t r, uint32_t g, uint32_t b) const;  
			void Cycle();
		}; 

#if BAKE_MODE
	public:
		inline static Palette8 savePalette; 
#endif

	public:
		uint8_t*		pixels;
		int				width;
		int				height;
		Palette8 const*	palette;  

	public:
		static void Save(Image const* image, char const* path);
		static SubSurface8* LoadFromFile(char const* path);

	public:
		SubSurface8() = default;
		SubSurface8(int const width, int const height); 
		SubSurface8(uint8_t* const buffer, int const width, int const height);  
		SubSurface8(char const* path); 
		~SubSurface8(); 
		void Render(Surface8* const screen, int const x, int const y) const;
	};

public:
	// application has a total of 256 colors to chose from:
	inline static uint32_t PAL[256]; 
	// static data for the hardcoded font:
	static inline char font[51][5][6]; 
	static inline int transl[256];  
	static inline bool fontInitialized = false; 

public:
	uint8_t*	pixels;
	int			width;
	int			height;

public:
	static void InitPalette(); 
	static void InitCharset(); 
	static void SetChar(int c, const char* c1, const char* c2, const char* c3, const char* c4, const char* c5); 
	static uint8_t FindClosestPaletteIndex(uint32_t r, uint32_t g, uint32_t b); 

public:
	Surface8() = default;  
	Surface8(uint8_t* const buffer, int const width, int const height);
	Surface8(int const width, int const height);
	~Surface8();  
	void Print(char const* s, int const x1, int const y1, uint8_t const c);
	void Print(char s, int const x, int const y, uint8_t const c);
	void Clear(uint8_t const c);
	void Line(float x1, float y1, float x2, float y2, uint8_t const c); 
	void Plot(int const x, int const y, uint8_t const c); 
	void CopyTo(Surface* dst); 
	void Box(int const x1, int const y1, int const x2, int const y2, uint8_t const c); 
	void Bar(int x1, int y1, int x2, int y2, uint8_t c);   
};
using SubSurface8	= Surface8::SubSurface8; 
using Sprite		= Surface8::SubSurface8;  
using ColorPalette8	= Surface8::SubSurface8::Palette8; 
}