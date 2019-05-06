
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

toqutree::toqutree(PNG & imIn, int k){ 

/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */
	
	root = buildTree(&imIn, k);

/* your code here */
}

int toqutree::sizeHelper(Node * n){
	if(n->SE == NULL){
		return 1;
	}
	return 1 + sizeHelper(n->NW) + sizeHelper(n->NE) + sizeHelper(n->SE) + sizeHelper(n->SW);
}

int toqutree::size() {
/* your code here */
	return sizeHelper(root);
}



//==================helper functions================

//fix coordnate of given x,y //done, debug?
//k is larger square dim 
pair<int,int> toqutree::fixCoord(int x, int y, int k){
	
	int maxdim = pow(2,k);
	if(x >= maxdim){
		x = x - maxdim;
	}else if(x < 0){
		printf("x: NEGATIVE ! HEY BUG SON!! GO FIX iT ! \n");
	}
	
	if(y >= maxdim){
		y = y - maxdim;
	}else if(y < 0 ){
		printf("y: NEGATIVE ! HEY BUG SON!! GO FIX iT ! \n");
	}
	
	pair<int,int> c(x,y);
	return c;
	
}

// k is large square's dim
// given lr (p), get lr of square
pair<int,int> toqutree::getCoordlr(pair<int,int>p, int k){
	int lrx = p.first + pow(2,k-1)- 1;
	int lry = p.second + pow(2,k-1) - 1;
	
	
	return fixCoord(lrx, lry, k);
}



// get vector = [SW NE NW] //done, debug?
vector<pair<int,int>> toqutree::getSplitCoord(pair<int,int> SE, int k){
	pair<int,int> SElr = getCoordlr(SE, k);
	vector<pair<int,int>> temp(3);
	
	//possible bug: dimesion

	pair<int, int>  SW = fixCoord(SElr.first + 1, SE.second,k);
	pair<int, int>  NE = fixCoord(SE.first, SElr.second + 1,k);
	pair<int, int>  NW = fixCoord(SElr.first + 1, SElr.second + 1,k);
	
	temp[0] = SW;
	temp[1] = NE;
	temp[2] = NW;	
	
	return temp;
}

//add 2 vectors
vector<int> toqutree::addvec(vector<int> vec1, vector<int> vec2){
	if(vec1.size() != vec2.size()){
		printf("KAY, VECT BUG! \n");
	}
	
	vector<int> temp(vec1.size());
	
	for(int i = 0; i < (int) vec1.size(); i++){
		temp[i] = vec1[i]+vec2[i];
	}
	
	return temp;
}
// k is large square's
double toqutree::getindvEntropy(stats & s, pair<int,int> p, int k){
	pair<int,int> plr = getCoordlr(p, k);
	
	
	int x1 = p.first;
	int y1 = p.second;
	int x2 = plr.first;
	int y2 = plr.second;
	
	int edgex = pow(2,k)-1;
	int edgey = pow(2,k)-1;
	
	
	if(x2 < x1){
		if(y2 > y1){ //only one side go over (x go over only)
			// vector<int> buildHist(pair<int,int> ul, pair<int,int> lr)
			
			//cout<< "case 1" <<"\n";
			
			pair<int,int> q1(edgex, y2);
			pair<int,int> q2(0,y1);
			vector<int> temp1 = s.buildHist(p, q1);
			vector<int> temp2 = s.buildHist(q2, plr);
			
			vector<int> temp = addvec(temp1, temp2);
			
			int area1 = s.rectArea(p,q1);
			int area2 = s.rectArea(q2,plr);
			int a = area1 + area2;
			
			return s.entropy(temp, a);
		
		}else{ // complicated case // y2 < y1
		
			//cout<< "case 2" <<"\n";
			
			pair<int,int> c1(0,0);   //goes with plr (c1,plr) ;; origin
			pair<int,int> c2(x1,0);  // goes with c3 (c2,c3)  ;; upper right ul 
			pair<int,int> c3(edgex, y2); //upper right lr
			pair<int,int> c4(0, y1); //goes with c5  (c4,c5) ;; bottom left ul 
			pair<int,int> c5(x2, edgey); // bottom left lr 
			pair<int,int> c6(edgex,edgey); //goes with p  (p,c6) 
			
			vector<int> vecc1 = s.buildHist(c1,plr);
			vector<int> vecc2 = s.buildHist(c2,c3);
			vector<int> vecc3 = s.buildHist(c4,c5);
			vector<int> vecc4 = s.buildHist(p,c6);
			
			vecc1 = addvec(vecc1,vecc2);
			vecc3 = addvec(vecc3,vecc4);
			vecc4 = addvec(vecc1,vecc3);
			
			int areac1 = s.rectArea(c1,plr);
			int areac2 = s.rectArea(c2,c3);
			int areac3 = s.rectArea(c4,c5);
			int areac4 = s.rectArea(p,c6);
			
			int ac = areac1 + areac2 + areac3 + areac4;
			
			return s.entropy(vecc4, ac);
			
		}
	}else { // x2 > x1 
	
		
		if(y2 > y1){ //no complication case
			//cout<< "case 3" <<"\n";
			
			
			
			return s.entropy(p,plr);
			
		} else{  // y go over only // y2 < y1
			//cout<< "case 4" <<"\n";
			
			pair<int,int> r1(x1 ,0);
			pair<int,int> r2(x2, edgey);
			
			vector<int> vecty1 = s.buildHist(r1, plr);
			vector<int> vecty2 = s.buildHist(p, r2);
			
			vector<int> vecty = addvec(vecty1, vecty2);
			
			int areay1 = s.rectArea(r1,plr);
			int areay2 = s.rectArea(p,r2);
			
			int ay = areay1 + areay2;
			
			return s.entropy(vecty, ay);
			
		}
	}
}

//k is large img k
double toqutree::aveEntrpy(stats & s, pair<int,int> SE, int k){
	//cout<< __LINE__ <<"\n";
	vector<pair<int,int>> temp = getSplitCoord(SE, k);
	//cout<< __LINE__ <<"\n";
	pair<int, int> SW = temp[0];
	pair<int, int> NE = temp[1];
	pair<int, int> NW = temp[2];	
	
	//cout<< "get SE entropy============" <<"\n";
	double SEentropy = getindvEntropy(s,SE, k);
	//cout<< "get SW entropy============" <<"\n";
	double SWentropy = getindvEntropy(s,SW, k);
	//cout<< "get NE entropy============" <<"\n";
	double NEentropy = getindvEntropy(s,NE, k);
	//cout<< "get NW entropy============" <<"\n";
	double NWentropy = getindvEntropy(s,NW, k);
	
	//cout<< __LINE__ <<"\n";
	return (SEentropy + SWentropy + NEentropy + NWentropy)/4;
}



//k is larger square's dim
void toqutree::genSqrImg(pair<int,int> p, int k, PNG & putImg, PNG & oriImg){
	pair<int,int> plr = getCoordlr(p, k);
	
	//putPatchImg(putImg, oriImg, k, origin, sqlr, p, plr);
	int sqrSize = pow(2,k-1);
	int imgSize = pow(2,k);
	
	for(int i = 0; i < sqrSize; i++){
		for(int j = 0; j < sqrSize; j++){
			*putImg.getPixel(i,j) = *oriImg.getPixel((i+p.first)%imgSize, (j+p.second)%imgSize);
		}
	}
	
}


//===============end of helper functions ==============
toqutree::Node * toqutree::buildTree(PNG * im, int k) {

	//if k-2 == 0, base case to do// go write it kid
	stats s(*im);
	
	if(k == 1){
		
		pair<int,int> b(1,1);
		pair<int,int> z1(0,0);
		pair<int,int> z2(pow(2,k)-1,pow(2,k)-1);
			
	
		Node * baseNode = new Node(z1, k, s.getAvg(z1, z2));
	
		
		baseNode->SE = new Node(z1, 0, *(im->getPixel(0,0))); 
		baseNode->SW = new Node(z1, 0, *(im->getPixel(1,0)));
		baseNode->NE = new Node(z1, 0, *(im->getPixel(0,1)));
		baseNode->NW = new Node(z1, 0, *(im->getPixel(1,1)));
		
		return baseNode;
	}else if(k == 0){
		pair<int,int> s(0,0);
		Node * baseNodeOne = new Node(s,0,*(im->getPixel(0,0)));
		return baseNodeOne;
	}
	
	pair<int, int> initp(pow(2,k-1),pow(2,k-1));
	pair<int,int> cur_op_p = initp;  //optimum for SE
	//cout<< __LINE__<< "\n";
	//cout<< "init p: (" << initp.first << " ," << initp.second << ") \n";
	double lowh = aveEntrpy(s, initp, k);  
	//cout<< __LINE__<< "\n";
	pair<int,int> temp;
	
	for(int j = initp.second; j < (initp.second + pow(2,k-1)); j++){ //bug, go fix
		for(int i = initp.first; i< (initp.first + pow(2,k-1));i++){
			temp = make_pair(i,j);
			if(aveEntrpy(s, temp, k) < lowh){
				cur_op_p = temp;
				lowh = aveEntrpy(s,temp, k);
			}
		}
	}
	
	
	vector<pair<int,int>> corner = getSplitCoord(cur_op_p, k);
	//cout<< __LINE__<< "\n";
	pair<int, int> SE = cur_op_p;
	pair<int, int> SW = corner[0];
	pair<int, int> NE = corner[1];
	pair<int, int> NW = corner[2];	
	
	pair<int,int> z1(0,0);
	pair<int,int> z2(pow(2,k)-1,pow(2,k)-1);
	Node * curr = new Node(cur_op_p, k, s.getAvg(z1,z2));
	
	int newsize = pow(2,k-1);
	
	PNG SEim(newsize,newsize);
	genSqrImg(SE, k, SEim, *im);
	
	PNG SWim(newsize,newsize);
	genSqrImg(SW, k, SWim, *im);
	
	PNG NEim(newsize,newsize);
	genSqrImg(NE, k, NEim, *im);
	
	PNG NWim(newsize,newsize);
	genSqrImg(NW, k, NWim, *im);
	
	curr->SE = buildTree(&SEim, k-1);
	curr->SW = buildTree(&SWim, k-1);
	curr->NE = buildTree(&NEim, k-1);
	curr->NW = buildTree(&NWim, k-1);
	
	
	return curr;
	
// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to 
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.

}


void toqutree::renderPatch(Node * n, PNG & img){
	
	if(n->NW == NULL){
		int size = pow(2,(n->dimension));
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				*img.getPixel(i,j) = n->avg;
			}
		}
		return;
	}
	int k = n->dimension;
	
	int size = pow(2,k) ;
	int subsize = pow(2,k-1);
	
	PNG subImg(subsize,subsize);
	
	pair<int,int> SEp = n->center;
	
	vector<pair<int,int>> u = getSplitCoord(SEp, k);
	pair<int, int> SWp = u[0];
	pair<int, int> NEp = u[1];
	pair<int, int> NWp = u[2];
	
	
	renderPatch(n->SE, subImg);
	putImgRender(img, subImg, SEp, k);
	
	renderPatch(n->SW, subImg);
	putImgRender(img, subImg, SWp, k);
	
	renderPatch(n->NE, subImg);
	putImgRender(img, subImg, NEp, k);
	
	renderPatch(n->NW, subImg);
	putImgRender(img, subImg, NWp, k);
	
}



PNG toqutree::render(){
	int size = pow(2,(root->dimension));
	PNG img(size,size);
	
	renderPatch(root, img);
	
	return img;

// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the 
// quadtree, instead.

/* your code here */

}
void toqutree::putImgRender(PNG & toPutImg, PNG & oriImg, pair<int,int> p, int k){
	int size = pow(2,k);
	int subSize = pow(2,k-1);
	for(int i = 0; i < subSize; i++){
		for(int j = 0; j < subSize; j++){
				*toPutImg.getPixel((i+p.first)%size,(j+p.second)%size) = *oriImg.getPixel(i,j); 
		}
	}
		
}

/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){
	prune(root,tol);
}

/*helper for prune*/
void toqutree::prune(Node* root,double tol) {
  if (root == NULL) return;

  if (checkTol(tol, root->NW, root->avg) && checkTol(tol, root->NE, root->avg) && checkTol(tol, root->SE, root->avg)  && checkTol(tol, root->SW, root->avg)) {
	 	 clear (root->NW);
		 clear (root->NE);
		 clear (root->SE);
		 clear (root->SW);

		 root->NW = NULL;
		 root->NE = NULL;
		 root->SE = NULL;
		 root->SW = NULL;
  } else { 
      prune(root->NW, tol);
      prune(root->NE, tol);
	  prune(root->SE, tol);
	  prune(root->SW, tol);
  }
}

 
bool toqutree::checkTol(double tol, Node* node, HSLAPixel avg) {
	if (node == NULL) return true;
	
	if (node->NW == NULL && node->NE==NULL && node->SE== NULL && node->SW == NULL) {
	  return node->avg.dist(avg) <= tol;
	} else {
		return checkTol(tol, node->NW, avg) && checkTol(tol, node->NE, avg) && checkTol(tol, node->SE, avg) && checkTol(tol, node->SW, avg);
	}
}

/*helper for clear*/
void toqutree::clear() {
	clear(root);
}

/* called by destructor and assignment operator*/
void toqutree::clear(Node * & croot){
     if (croot == NULL) return;
	 if (croot->NW == NULL && croot->NE == NULL && croot->SE == NULL && croot->SW == NULL) {
	 	 delete(croot);
		 return;
	 }
	 clear(croot->NW);
	 clear(croot->NE);
	 clear(croot->SE);
	 clear(croot->SW);

	 delete(croot);
}


/* done */
/* called by assignment operator and copy constructor */


toqutree::Node * toqutree::copy(const Node * other) {
	if(other->NE == NULL){
		Node *baseN = new Node(other->center, other->dimension, other->avg);
		return baseN;
	}else if(other == NULL){
		return NULL;
	}
	
	Node *n = new Node(other->center, other->dimension, other->avg);
	
	n->NW = copy(other->NW);
	n->NE = copy(other->NE);
	n->SE = copy(other->SE);
	n->SW = copy(other->SW);
	
	return n;
}


