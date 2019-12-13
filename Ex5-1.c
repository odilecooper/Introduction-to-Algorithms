#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define WHITE 1
#define GRAY 2
#define BLACK 3
//#define N 10
//#define NNZ 13
#define N 4038//矩阵行/列数
#define NNZ 88234//上三角矩阵非零元素

//val保存矩阵A的非零元素（从上往下，从左往右）
//col_ind保存val数组中元素的列索引
//如果val(k)=a(i,j)，则col_ind(k)=j。
//row_ptr保存矩阵A的每行第一个非零元素在val中的索引
//如果val(k)=a(i,j)，则row_ptr(i)<= k < row_ptr(i+1)

int col_ind[NNZ*2];
int row_ptr[N];
int color[N];
int d[N];
int pie[N];
int Q[N];
int qhead;
int qtail;
int tmp[N][N];

void ENQUEUE(int s) {
    qtail = (qtail+1) % N;
    Q[qtail] = s;
    int i;
/*
    printf("    QUEUE:");
    for (i=qhead+1; i<=qtail; i++)
    	printf("%d ", Q[i]);
    printf("\n");
*/
}
int DEQUEUE() {
    int u = Q[qhead+1];
    Q[qhead+1] = -1;
    qhead = (qhead+1) % N;
/*
    printf("    QUEUE:");
    int i;
    for (i=qhead+1; i<=qtail; i++)
    	printf("%d ", Q[i]);
    printf("\n");
*/
    return u;
}
void BFS(s) {
    //初始化点集
    int k;
    for (k=0; k<N; k++) {
    	color[k] = WHITE;
    	d[k] = N+1;//infinite
    	pie[k] = -1;//null
    	Q[k] = -1;
	}
    //初始化s
    color[s] = GRAY;
    d[s] = 0;
    qhead = 0;
    qtail = 0;
    ENQUEUE(s);
//    printf("ENQUEUE %d\n", s);
    while (qhead != qtail) {//Q not empty
        int u = DEQUEUE();
//        printf("DEQUEUE %d\n", u);
        for (k=row_ptr[u]; k<row_ptr[u+1]; k++) {
            int v = col_ind[k];
//            printf("v = %d\n", v);
            if (color[v] == WHITE) {
                color[v] = GRAY;
                d[v] = d[u] + 1;
                pie[v] = u;
                ENQUEUE(v);
//                printf("ENQUEUE %d\n", v);
            }
        }
        color[u] = BLACK;
    }
}
void getInput() {
    int i, j, k;
//    int tmp[N][N];
    //读入上三角矩阵变成完整矩阵
    //虽然用了N*N空间，但是调用完函数之后归还
    for (i=0; i<N; i++)
    	for (j=0; j<N; j++)
    		tmp[i][j] = 0;
    for (k=0; k<NNZ; k++) {
        scanf("%d %d", &i, &j);
        tmp[i][j] = 1;
        tmp[j][i] = 1;
    }
    k = 0;
    int row = -1;
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            if (tmp[i][j] == 0)
                continue;
            col_ind[k] = j;
            if (row != i)
                row_ptr[i] = k;
            row = i;
            k++; 
        }
    }
/*
    for (i=0; i<NNZ*2; i++)
    	printf("%d ",col_ind[i]);
    printf("\n");
    for (i=0; i<N; i++)
    	printf("%d ", row_ptr[i]);
*/
}
int main() {
    getInput();
    int src, des;
    printf("source:");
    scanf("%d", &src);
    printf("destiny:");
    scanf("%d", &des);
    while (1) {
    	if (src==-1) break;
    	BFS(src);
    	printf("distance between source and destiny: %d\n", d[des]);
    	int u=des;
    	printf("route from destiny to source:");
		while (u!=src) {
			printf("%d ", u);
			u = pie[u];
		}
		printf("%d\n", src);
	    printf("source:");
    	scanf("%d", &src);
    	printf("destiny:");
    	scanf("%d", &des);
	}
    return 0;
}
