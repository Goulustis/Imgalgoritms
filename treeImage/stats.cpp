
#include "stats.h"


stats::stats(PNG & im){
	unsigned int w = im.width();
	unsigned int h = im.height();
	//create vectors to put stuff in
	for(unsigned int i = 0; i < w; i++){
		vector<double> temphX(h);
		vector<double> temphY(h);
		vector<double> tempSat(h);
		vector<double> tempLum(h);
		vector<vector<int>> tempvechis(h);
		
		for(unsigned int j = 0 ; j < h; j++){
			vector<int> tempbin(36);
			tempvechis[j] = tempbin;
		}
		
		hist.push_back(tempvechis);
		sumHueX.push_back(temphX);
		sumHueY.push_back(temphY);
		sumSat.push_back(tempSat);
		sumLum.push_back(tempLum);
	}
	
	//initializing hist, hue, sat, lum vectors base base case
	hist[0][0][(int)(im.getPixel(0,0)->h)/10] = 1;
	
	sumHueX[0][0] =  cos((im.getPixel(0,0)->h)*(PI/180));
	
	sumHueY[0][0] =  sin((im.getPixel(0,0)->h)*(PI/180));
	
	sumSat[0][0] = im.getPixel(0,0)->s;
	
	sumLum[0][0] = im.getPixel(0,0)->l;
	
	//initialization for base case
	for(unsigned int j = 1 ; j < h; j++){
		sumHueX[0][j] =  sumHueX[0][j-1] + cos((im.getPixel(0,j)->h)*(PI/180));
		sumHueY[0][j] =  sumHueY[0][j-1] + sin((im.getPixel(0,j)->h)*(PI/180));
		sumSat[0][j] = sumSat[0][j-1] + im.getPixel(0,j)->s;
		sumLum[0][j] = sumLum[0][j-1] + im.getPixel(0,j)->l;
		
		//hist init
		hist[0][j][(int)(im.getPixel(0,j)->h)/10] = 1; 
		for( int k = 0; k<36; k++){
			hist[0][j][k] =  hist[0][j-1][k] + hist[0][j][k];
		}
	}
	
	for(unsigned int i = 1 ; i < w; i++){
		sumHueX[i][0] =  sumHueX[i-1][0] + cos((im.getPixel(i,0)->h)*(PI/180));
		sumHueY[i][0] =  sumHueY[i-1][0] + sin((im.getPixel(i,0)->h)*(PI/180));
		sumSat[i][0] = sumSat[i-1][0] + im.getPixel(i,0)->s;
		sumLum[i][0] = sumLum[i-1][0] + im.getPixel(i,0)->l;
		
		//hist init
		hist[i][0][(int)(im.getPixel(i,0)->h)/10] = 1; 
		for(int k = 0; k<36; k++){
			hist[i][0][k] =  hist[i-1][0][k] + hist[i][0][k];
		}
	}
	
	
	//general case
	for(unsigned int i = 1; i < w ; i++){
		for(unsigned int j = 1; j < h ; j++){
			sumHueX[i][j] = sumHueX[i-1][j] + sumHueX[i][j - 1] - sumHueX[i-1][j-1] + cos((im.getPixel(i,j)->h)*(PI/180));
			sumHueY[i][j] = sumHueY[i-1][j] + sumHueY[i][j - 1] - sumHueY[i-1][j-1] + sin((im.getPixel(i,j)->h)*(PI/180));
			sumSat[i][j] = sumSat[i-1][j] + sumSat[i][j - 1] - sumSat[i-1][j-1] + im.getPixel(i,j)->s;
			sumLum[i][j] = sumLum[i-1][j] + sumLum[i][j - 1] - sumLum[i-1][j-1] + im.getPixel(i,j)->l;
			
			for(int k = 0 ; k<36 ; k++){
				hist[i][j][k] = hist[i-1][j][k] + hist[i][j-1][k] - hist[i-1][j-1][k]; 
				
			}
			hist[i][j][(int)(im.getPixel(i,j)->h)/10] = hist[i][j][(int)(im.getPixel(i,j)->h)/10]+1;
		}
		
	}
	/*
	
	cout<< "last print ---------"<< "\n";
	for(unsigned int j = 0; j < h ; j++){
		
		for(unsigned int i = 0; i < w ; i++){
			cout << sumSat[i][j] << " |";
		}
		cout<< "\n";
	}
	*/
	/*
	cout<< "print ori ---------"<< "\n";
	for(unsigned int j = 0; j < h ; j++){
		
		for(unsigned int i = 0; i < w ; i++){
			cout << im.getPixel(i,j)->h << " |";
		}
		cout<< "\n";
	}
	
	cout<< "print hist -----"<<"\n";
	for(int k = 0; k<36; k++){
		cout<< hist[w-1][h-1][k]<< " |";
	}
	
	cout<<"\n";
	*/

/* your code here */

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
	 
	return (lr.first - ul.first + 1)*(lr.second - ul.second + 1);
	
/* your code here */

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	
	int ulx = ul.first - 1;
	int uly = ul.second - 1;
	int lrx = lr.first;
	int lry = lr.second;
	double tHueX = sumHueX[lrx][lry];
	double tHueY = sumHueY[lrx][lry];
	double tSat = sumSat[lrx][lry];
	double tLum = sumLum[lrx][lry];
	long a = rectArea(ul,lr);
	
	
	 if(ulx  >= 0){
		 tHueX -= sumHueX[ulx][lry];
		 tHueY -= sumHueY[ulx][lry];
		 tSat -= sumSat[ulx][lry];
		 tLum -= sumLum[ulx][lry];
	 }
	 
	 if(uly >= 0 ){
		 tHueX -= sumHueX[lrx][uly];
		 tHueY -= sumHueY[lrx][uly];
		 tSat -= sumSat[lrx][uly];
		 tLum -= sumLum[lrx][uly];
	 }
	 
	 if(ulx >= 0 && uly >= 0){
		 tHueX += sumHueX[ulx][uly];
		 tHueY += sumHueY[ulx][uly];
		 tSat += sumSat[ulx][uly];
		 tLum += sumLum[ulx][uly];
	 }
	
	double tHue = atan2(tHueY,tHueX)*180/PI;
	
	if(tHue<0){
		tHue += 360;
	}
	
	tSat /= a;
	tLum /= a;
	
	HSLAPixel p(tHue, tSat, tLum, 1.0);
	return p;

/* your code here */
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){
	int ulx = ul.first - 1;
	int uly = ul.second - 1;
	int lrx = lr.first;
	int lry = lr.second;
	
	vector<int> temp(36);
	// initialize
	temp = hist[lrx][lry];
	for(int k = 0; k<36; k++){
		 if(ulx  >= 0){
			 temp[k] -= hist[ulx][lry][k];
		 }
		 
		 if(uly >= 0 ){
			 temp[k] -= hist[lrx][uly][k];
		 }
		 
		 if(ulx >= 0 && uly >= 0){
			 temp[k] += hist[ulx][uly][k];
		 }
		
	}
	
	return temp;

/* your code here */
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;

/* your code here */

    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    }

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){
		
	double area = rectArea(ul,lr);
	
	vector<int> dist = buildHist(ul,lr);
	
	double entpy = entropy(dist, area);
	return entpy;

/* your code here */

}
