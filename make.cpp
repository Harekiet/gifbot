#include "gif.h"
#include "vga.h"
#include "font.h"

//Active frame
//static uint8_t B[H][W];
//Active palette
//static uint8_t P[256][3];

Buffer B;
Palette P;

static void InitBuffers() {
	memset( B, 0, sizeof( B ) );
	for ( int i = 0; i < 256;i++ ) {
		P[i][0] = VGA[i][0]*4;
		P[i][1] = VGA[i][1]*4;;
		P[i][2] = VGA[i][2]*4;;
	}
}

void SaveFrame( bool clear ) {
    GifSaveFrame( B[0], P[0] );
    if ( clear ) {
	B.c();
    }
}

static void RunGif() {
    #include "/tmp/gif.cpp"
}

int main( int argc, char**argv ) {
    gifWriter.Init( "/tmp/gif.gif" );
    InitBuffers();

    RunGif();

//    printf( "Valid %d\n", gifWriter.Valid() );

    if ( gifWriter.Valid() ) {
        return 0;
    } else {
	return 3;
    }
}
