#include "header.h"
#include <gif_lib.h>

class GIFOutput {
	GifFileType* gf;
    uint32_t count;
	enum {
		MAX = FRAMES,
	};
	
public:
	void Init( const char* fileName ) {
		gf = EGifOpenFileName( fileName, false );
		gf->SWidth = WIDTH;
		gf->SHeight = HEIGHT;
		gf->SColorResolution = 256;
		//Seems I'm required to make a global map
		gf->SColorMap = MakeMapObject( 256, 0 );

		//Write the description already
		EGifPutScreenDesc( gf, WIDTH, HEIGHT, 256, 0, gf->SColorMap );

		 char nsle[12] = "NETSCAPE2.0";
		 char subblock[3];
		 EGifPutExtensionFirst(gf, APPLICATION_EXT_FUNC_CODE, 11, nsle);
	         subblock[0] = 1;
	         subblock[2] = 255;
	         subblock[1] = 255;
	         EGifPutExtensionLast(gf, APPLICATION_EXT_FUNC_CODE, 3, subblock);
	}

	void AddFrame( const uint8_t* pixels, const uint8_t* palette ) {
		//Easy exit here
		if ( count >= MAX )
        		return;
		++count;

		//Save the color map
		ColorMapObject map;
		map.ColorCount = 256;
		map.BitsPerPixel = 8;
		map.Colors = (GifColorType*)palette;
		//Put the color map in there		
		EGifPutImageDesc( gf, 0, 0, WIDTH, HEIGHT, 0, &map );
		//Put the raster data in there
		for ( int y = 0; y < HEIGHT; y++, pixels += WIDTH ) {
		    EGifPutLine( gf, (GifPixelType*)pixels, WIDTH );
		}	    
	}
	
	GIFOutput() {
		count = 0;
		gf = 0;
	}

	~GIFOutput() {
	    EGifCloseFile(gf);
	}

	bool Valid() {
	    return count > 0;
	}
};


static GIFOutput gifWriter;

//Save the current frame from B using the palette in P
void GifSaveFrame( const uint8_t* pixels, const uint8_t* palette ) {
    gifWriter.AddFrame( pixels, palette );
}
