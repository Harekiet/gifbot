/*
	!gif header 
*/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
//#include <cmath>
#include <climits>
#include <cctype>
#include <cfloat>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include <complex>

//#define GLM_FORCE_SWIZZLE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/random.hpp"

using namespace glm;
//using namespace std;

enum {
        FRAMES = 64,
        WIDTH = 320,
        HEIGHT = 240,
        W = WIDTH,
        H = HEIGHT,
        S = W*H,
};

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef double D;
typedef int I;
typedef unsigned int UI;

//Character fonts
extern const U8 f8[256*8];


/*
	Main buffer object, this gets saved to a gif!
*/

class Buffer {
	uint8_t data[S];
	uint8_t outside;
	int index;
public:
	Buffer() {
		index = 0;
		c();
	}

	void operator= (int input ) {
		data[ index++ % sizeof(data) ] = input;
	}	
	void operator* (int i ) {
		i = (i >= 0 ) ? ( (i <= 255 ) ? i : ( 255 )) : 0;
		data[ index++ % sizeof(data) ] = i;
	}	
	
	//Clear the buffer
	void c( int v = 0) {
		memset( data, v, sizeof( data ) );
	}

	//Allow direct access
	uint8_t* operator[] ( int index ) {
		return data + index * WIDTH;
	}
	
	uint8_t& operator() (int y, int x) {
		if ( y>=0 && y<H && x>=0 && x<W) {
			return data[y*WIDTH+x];
		} else {
		    return outside;
		}
	}

	uint8_t& i(int y, int x) {
		if ( y>=0 && y<H && x>=0 && x<W) {
			return data[y*WIDTH+x];
		} else {
		    return outside;
		}
	}

	//Acccess through -1,1 ranges
	uint8_t& d( D y, D x) {
	    	return i((y+1)*H*0.5+0.5,((x*H)+W)*0.5+0.5);
	}	

	uint8_t& operator() (int o) {
		return data[o];
	}	

	uint8_t* Get() {
		return data;
	}	
	
	operator uint8_t*() {
		return data;
	}
};

/*
	Palette objbect, defaults to vga table somewhere else
*/

class Palette {
	uint8_t data[768];
	int index;
public:
	Palette() {
		index = 0;
		G();
	}
	
	//Make a grey scale
	void G() {
		for( int i = 0;i<256;i++ ) {
			data[i*3+0]=i;
			data[i*3+1]=i;
			data[i*3+2]=i;
		}
	}
	
	//Allow a single palette entry to be set and forward the index
	int operator= (int input ) {
		data[ index++ % 768 ] = input;
		return input;
	}

	//Allow floats from 0-1 as well...
	double operator*(double input ) {
		data[ index++ % 768 ] = (255*input + 0.5);
		return input;
	}

	uint8_t* operator[] (int index) {
		return data + (index &0xff) * 3;
	}

	//Access to save it
	uint8_t* Get() {
		return data;
	}
};

extern Buffer B;
extern Palette P;

//Save a frame, more than FRAMES will exit();
extern void SaveFrame(bool clear);

#define F SaveFrame(false)
#define C SaveFrame(true)
#define PG P.G()
#define X(_Y_,_X_) B.d(_Y_,_X_)

template<typename T>
void ROT( T&x, T&y, double a) {
    T ca = cos(a); T sa = sin(a);
    T u = x * ca - y * sa;
    T v = x * sa + y * ca;
    x = u; y = v;
}

#define L2 for(D y=-1;y<1;y+=(2.0/H))for(D x=-1/0.75;x<1/0.75;x+=(2/0.75)/W)

#define L3 for(I y=0;y<H;y++)for(I x=0;x<W;x++)

//y=2*iy/W;
#define L D x=-1,y=-1/0.75;for(I iy=0;iy<H;iy++,y=-1+2.0*iy/H)for(I ix=0;ix<W;ix++,x=-1/0.75+2.0*ix/H)

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

#ifndef PI
#    define PI 3.14159265358979323846
#endif