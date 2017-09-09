#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <cmath>
#include <climits>
#include <cctype>
#include <cfloat>
#include <vector>
#include <set>
# include <list>
//#include <array>
#include <string>
#include <algorithm>
#include <iostream>
#include <complex>

using namespace std;

void GifSaveFrame( const uint8_t* pixels, const uint8_t* palette );

enum {
	FRAMES = 64,
	WIDTH = 320,
	HEIGHT = 240,
	W = WIDTH,
	H = HEIGHT,
	S = W*H,
};

