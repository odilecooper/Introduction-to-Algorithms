#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define WHITE 1
//正向涂色
#define FGRAY 2
#define FBLACK 3
//逆向涂色
#define BGRAY 4
#define BBLACK 5
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
int row_ptr[N+1];
int color[N];
int fd[N];
int bd[N];
int fpie[N];
int bpie[N];
int fQ[N];
int bQ[N];
int fqhead;
int fqtail;
int bqhead;
int bqtail;
int tmp[N][N];

void ENQUEUE(int Q[], int s, int *qtail) {
    *qtail = (*qtail+1) % N;
    Q[*qtail] = s;
    int i;
/*
    printf("    QUEUE:");
    for (i=qhead+1; i<=qtail; i++)
    	printf("%d ", Q[i]);
    printf("\n");
*/
}
int DEQUEUE(int Q[], int *qhead) {
    int u = Q[*qhead+1];
    Q[*qhead+1] = -1;
    *qhead = (*qhead+1) % N;
/*
    printf("    QUEUE:");
    int i;
    for (i=qhead+1; i<=qtail; i++)
    	printf("%d ", Q[i]);
    printf("\n");
*/
    return u;
}
void twoWayBFS(s, d) {
    //初始化点集
    int k;
    for (k=0; k<N; k++) {
    	color[k] = WHITE;
    	fd[k] = N+1;//infinite
    	fpie[k] = -1;//null
    	fQ[k] = -1;
        bd[k] = N+1;//infinite
    	bpie[k] = -1;//null
    	bQ[k] = -1;
	}
    //初始化s
    color[s] = FGRAY;
    color[d] = BGRAY;
    fd[s] = 0;
    bd[d] = 0;
    fqhead = 0;
    fqtail = 0;
    bqhead = 0;
    bqtail = 0;
    ENQUEUE(fQ, s, &fqtail);
    ENQUEUE(bQ, d, &bqtail);
    while (fqhead!=fqtail && bqhead!=bqtail) {
	    if (fqhead-fqtail <= bqhead-bqtail) {
	   	    //正向搜索一层
   		    int u = DEQUEUE(fQ, &fqhead);
        	for  (k=row_ptr[u]; k<row_ptr[u+1]; k++) {
            	int v = col_ind[k];
	            if (color[v]==BGRAY | color[v]==BBLACK) {
	                //和反向搜索树相遇
					printf("distance between source and destiny: %d\n", fd[u]+1+bd[v]);
	                while (v != d) {
	                	fpie[v] = u;
	                	u = v;
	                	v = bpie[v];
					}
					fpie[v] = u;
					printf("route from destiny to source: %d ", d);
					while (u != s) {
						printf("%d ",u);
						u = fpie[u];
					}
					printf("%d\n", s);
					return;
	            } else if (color[v] == WHITE) {
	                color[v] = FGRAY;
	                fd[v] = fd[u] + 1;
	                fpie[v] = u;
	                ENQUEUE(fQ, v, &fqtail);
	            }
	        }
	        color[u] = FBLACK;
	    } else {
	        //反向搜索一层
	        int u = DEQUEUE(bQ, &bqhead);
	        for  (k=row_ptr[u]; k<row_ptr[u+1]; k++) {
	            int v = col_ind[k];
	            if (color[v]==FGRAY | color[v]==FBLACK) {
	                //和正向搜索树相遇
	                printf("distance between source and destiny: %d\n", bd[u]+1+fd[v]);
	                while (v != d) {
	                	bpie[v] = u;
	                	u = v;
	                	v = fpie[v];
					}
					bpie[v] = u;
					printf("route from source to destiny: %d ", s);
					while (u != d) {
						printf("%d ",u);
						u = bpie[u];
					}
					printf("%d\n", s);
					return;
	            } else if (color[v] == WHITE) {
	                color[v] = BGRAY;
	                bd[v] = bd[u] + 1;
	                bpie[v] = u;
	                ENQUEUE(bQ, v, &bqtail);
	            }
	        }
	        color[u] = BBLACK;
	    }
	}
}
void getInput() {
    int i, j, k;
//    int tmp[N][N];
    memset(tmp, 0, sizeof(tmp));
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
    row_ptr[N+1] = 2*NNZ+1;
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
		twoWayBFS(src, des);
	    printf("source:");
    	scanf("%d", &src);
    	printf("destiny:");
    	scanf("%d", &des);
	}
    return 0;
}
