#include "stripeColorPicker.h"

stripeColorPicker::stripeColorPicker(HSLAPixel fillColor, int stripeSpacing)
{
	color = fillColor;
	spacing = stripeSpacing;
/* your code here! */

}

HSLAPixel stripeColorPicker::operator()(int x, int y)
{
	if (x%spacing == 0){
		return color;
	}else{
		HSLAPixel targ;
		return targ;
	}
		
/* your code here! */
}
