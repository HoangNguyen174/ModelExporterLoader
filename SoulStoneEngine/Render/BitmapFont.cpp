#include "BitmapFont.hpp"
#include "../Utilities/tinyxml2.h"
#include "../Utilities/GameCommon.hpp"

BitmapFont::BitmapFont( const std::string& bitmapFontName,const std::string& m_fontSheet )
{
	tinyxml2::XMLDocument dataFile;
	m_fontTextureSheet = new Texture();
	m_fontTextureSheet = Texture::CreateOrGetTexture(m_fontSheet);

	dataFile.LoadFile( bitmapFontName.c_str() );

	if(dataFile.ErrorID() != 0)
	{
		DebuggerPrintf( "Cannot Load Bitmap Font data file.\n" );
		exit(0);
	}

	tinyxml2::XMLElement* root =  dataFile.FirstChildElement();
	if(root == nullptr)
	{
		DebuggerPrintf( "Failed to load XML font file: No root element.\n" );
		exit(0);
	}

	for(tinyxml2::XMLElement* ele =  dataFile.FirstChildElement(); ele != nullptr; ele = ele->FirstChildElement())
	{
		for(tinyxml2::XMLElement* subele =  ele->FirstChildElement("Glyph"); subele != nullptr; subele = subele->NextSiblingElement())
		{
			std::string eleName = subele->Value();

			if(eleName == "Glyph")
			{
				const char* minTexCoord;
				const char* maxTexCoord;
				Vector2 min;
				Vector2 max;
				BitmapFontGlyph glyph;

				minTexCoord = subele->Attribute("texCoordMins");
				maxTexCoord = subele->Attribute("texCoordMaxs");

				glyph.ttfA = subele->FloatAttribute("ttfA");
				glyph.ttfB = subele->FloatAttribute("ttfB");
				glyph.ttfC = subele->FloatAttribute("ttfC");

				glyph.texCoordMins = ParseMinMaxString(minTexCoord);
				glyph.texCoordMax = ParseMinMaxString(maxTexCoord);

				m_bitmapFontGlypdDataList.push_back(glyph);
			}
		}
	}
}

Vector2 BitmapFont::ParseMinMaxString(const std::string& myStr)
{
	int comma = myStr.find(",");
	Vector2 temp;
	temp.x = (float)atof(myStr.substr(0,comma).c_str());
	temp.y = (float)atof(myStr.substr(comma + 1,myStr.size()).c_str());
	return temp;
}

BitmapFont::~BitmapFont()
{
	//delete m_fontTextureSheet;
}