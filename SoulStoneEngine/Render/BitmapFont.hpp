#ifndef BITMAPFONT_H
#define BITMAPFONT_H
#include "../Utilities/Vector2.hpp"
#include <vector>
#include "../Utilities/Texture.hpp"

struct BitmapFontGlyph
{
	Vector2 texCoordMins;
	Vector2 texCoordMax;
	float ttfA;
	float ttfB;
	float ttfC;
};

class BitmapFont
{
	public:
		std::vector<BitmapFontGlyph> m_bitmapFontGlypdDataList;
		Texture* m_fontTextureSheet;
		
	public:
		BitmapFont() {};
		BitmapFont( const std::string& bitmapFontDataName,const std::string& m_fontSheet );
		~BitmapFont();
		void RenderConsole();

	private:
		Vector2 ParseMinMaxString(const std::string& myStr);
};

#endif