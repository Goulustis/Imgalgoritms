#include "customColorPicker.h"

customColorPicker::customColorPicker(HSLAPixel gridColor1)
{
    color1 = gridColor1;

}

HSLAPixel customColorPicker::operator()(int x, int y)
{
    if (x % 2 == 0 || y % 2 == 0) {
		
        return color1;
		
    }
    else {
		 
	     HSLAPixel color2(300., 0.5, 1.0);
         return  color2;
     }
	
	
}