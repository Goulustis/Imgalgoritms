#ifndef CUSTOMCOLORPICKER_H
#define CUSTOMCOLORPICKER_H

#include "colorPicker.h"

class customColorPicker : public colorPicker
{
  public:
  	customColorPicker(HSLAPixel gridColor1);
	
	
  	virtual HSLAPixel operator()(int x, int y);

  private:
    HSLAPixel color1; 

};

#endif