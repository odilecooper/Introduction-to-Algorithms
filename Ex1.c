#include <stdio.h>
#include <math.h>

typedef struct point{
	int x;
	int y;
}pt;

void exchange(pt *p1, pt *p2){
	int tmpx, tmpy;
	tmpx = p1->x;
	tmpy = p1->y;
	p1->x = p2->x;
	p1->y = p2->y;
	p2->x = tmpx;
	p2->y = tmpy;
}
int partition(pt *P, int l, int r, int flag){
	int i = l-1, j, tmpx, tmpy;
	if (flag == 1){//sort as x
		int key = P[r].x;
		for (j=l; j<r; j++){
			if (P[j].x <= key){
				i++;
				exchange(&P[i], &P[j]);
			}
		}
	}
	else {//sort as y
		int key = P[r].y;
		for (j=l; j<r; j++){
			if (P[j].y <= key){
				i++;
				exchange(&P[i], &P[j]);
			}
		}
	}
	exchange(&P[i+1],&P[r]);
	return i+1;
}
void quickSort(pt *P, int l, int r, int flag){
	if (l < r){
		int q = partition(P, l, r, flag);
		quickSort(P, l, q-1, flag);
		quickSort(P, q+1, r, flag);
	}
}
double distance(pt p1, pt p2){
	return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}
//合并
double merge(pt* P, pt* Y, double minDist, int mid, int n){
	int i, j, index, lenL=0, lenR=0, lenY=0;
	double dist = minDist;
	pt L[(n+1)/2];
	pt R[n/2];
	pt YY[n];
	//将Y中在长方形内的点放到YY中
	for (i=0; i<n; i++){
		if (Y[i].x-P[mid].x<minDist && Y[i].x-P[mid].x>-minDist){
			YY[lenY].x = Y[i].x;
			YY[lenY].y = Y[i].y;
			lenY++;
		}
	}
	for (i=0; i<lenY; i++){
		//只需考虑后续的7个点
		for (j=1; j<=7; j++){
			if (i+j >= lenY) break;
			if ((YY[i].x-P[mid].x)*(YY[i+j].x-P[mid].x)<=0){
				//YY[i]和YY[i+j]分别在l左右
				double tmpdist = distance(YY[i],YY[i+j]);
				//计算距离
				if (tmpdist < dist)
					dist = tmpdist;
					//更新δ'
				}
		}
	}
	return dist;
}
//分解
double findClosest(pt* P, pt* Y, int n){
	if (n == 2)
		//P中只有两个点，直接返回距离
		return distance(P[0], P[1]);
	if (n == 3){
		//P中有3个点，分别计算距离，返回最小
		double d1 = distance(P[0], P[1]);
		double d2 = distance(P[1], P[2]);
		double d3 = distance(P[0], P[2]);
		if (d1 < d2)
			return d3 < d1? d3:d1;
		else return d2 < d3? d2:d3;
	}
	//P中点的个数>3
	int lenL=(n+1)/2, lenR=n/2, i, j=0, k=0;
	pt PL[lenL], PR[lenR], YL[lenL], YR[lenR];
	//pt:存储点的坐标的数据结构的指针
	//P中点已经按照x坐标增序排列，前一半点放入PL
	for (i=0; i<lenL; i++){
		PL[i].x = P[i].x;
		PL[i].y = P[i].y;
	}
	//后一半点放入PR
	for (i=0; i<lenR; i++){
		PR[i].x = P[lenL+i].x;
		PR[i].y = P[lenL+i].y;
	}
	//Y中的点已按照y坐标增序排列
	//P[lenL]为直线l位置
	for (i=0; i<n; i++){
		if (Y[i].x < P[lenL].x){
			//在l左边，放入YL
			YL[j].x = Y[i].x;
			YL[j].y = Y[i].y;
			j++;
		}
		else{//在l右边，放入YR
			YR[k].x = Y[i].x;
			YR[k].y = Y[i].y;
			k++;
		}
	}
	//递归调用
	double dl = findClosest(PL, YL, lenL);
	double dr = findClosest(PR, YR, lenR);
	double d = dl < dr? dl:dr;
	//返回PL，PR，合并计算中距离最小的
	return merge(P, Y, d, lenL-1, n);
}
int main(){
	int n;
	printf("input number of the points:");
	scanf("%d", &n);
	int i;
	pt P[n], Y[n];
	for (i=0; i<n; i++){
		printf("input location of the %dth point(x,y):", i);
		scanf("%d %d", &P[i].x, &P[i].y);
		Y[i].x = P[i].x;
		Y[i].y = P[i].y;
	}
	quickSort(P, 0, n-1, 1);// x from small to large
	quickSort(Y, 0, n-1, 0);// y from small to large
	//see if right
	for (i=0; i<n; i++)
		printf("(%d,%d) ", P[i].x, P[i].y);
	printf("\n");
	for (i=0; i<n; i++)
		printf("(%d,%d) ", Y[i].x, Y[i].y);
	printf("\n");
	//end debug
	double dist = findClosest(P,Y,n);
	printf("min distance = %lf\n", dist);
	return 0;
}
