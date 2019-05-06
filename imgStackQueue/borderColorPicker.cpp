#include "borderColorPicker.h"

borderColorPicker::borderColorPicker(HSLAPixel fillColor, PNG & img, double tolerance,HSLAPixel center)
{
	color = fillColor;
	im = img;
	tol = tolerance;
	ctr = center;
/* Your code here! */
}

HSLAPixel borderColorPicker::operator()(int x, int y)
{
	for(int i = x-3; i<= x+3; i++){
		for(int j = y-3; j<=y+3;j++){
			if((x-i)*(x-i)+(y-j)*(y-j)<=9){
				
				if(i < 0 || i >= (int)im.width() || j<0 || j>= (int)im.height()){
					
					return color;
				}else if(ctr.dist(*(im.getPixel(i,j))) > tol && *(im.getPixel(i,j)) != color){
					
					return color;
				}
					
				
			}
		}
		
	}
	
	return *(im.getPixel(x,y));
	
/* Your code here! */
    
}
