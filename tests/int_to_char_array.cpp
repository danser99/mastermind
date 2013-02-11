#include <iostream>
#include <inttypes.h>
using namespace std;

int main()
{
    
    
    while (true) {
	unsigned int v=0, r=0;
	uint8_t a[4] = {0,0,0,0};
	unsigned int ref = 0x000000FF;
	
	cin >>v;
	cout << hex << v << '\n';
	
	for (int i = 0; i < 4; i++) {
	    a[i] = (v & ref) >> (8 * i);
	    ref = ref << 8;
	    cout << '\n' << hex << ref;
	}   
    
    for (int i = 0; i < 4; i++)
        r += ((unsigned int) a[i]) << (8 * i);
    
    
    cout << hex << r;
    cout << '\n' << dec << r << "\n\n";

    }
    return 0;
}
