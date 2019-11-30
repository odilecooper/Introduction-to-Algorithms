//最长公共子序列LCS
#include <stdio.h>
#include <string.h>
#define M 257
#define N 257
char s1[M],s2[N];
int b[M][N];
int max(int a, int b){
	return (a>b? a:b);
}
void LCS_LENGTH(){
    int l1,l2,i,j;
    l1=strlen(s1);
    l2=strlen(s2);
    for (i=1; i<=l1; i++) {
        for (j=1; j<=l2; j++) {
            if (s1[i-1] == s2[j-1])//匹配成功，加入LCS
                b[i][j] = b[i-1][j-1]+1;
            else b[i][j] = max(b[i-1][j], b[i][j-1]);
        }
    }
    printf("LCS-LENGTH = %d\n", b[l1][l2]);//b表右下角一格为LCS长度
}
void PRINT_LCS(){
	//根据表b，找到最长公共子序列并打印
	int i=strlen(s1), j=strlen(s2);
    int k = 0;
    char lcs[257];
    while (b[i][j] != 0) {
        if(b[i][j] == b[i-1][j])//从左侧继承
			i--;
		else if(b[i][j] == b[i][j-1])//从上方继承
			j--;
		else if(b[i][j] > b[i-1][j-1]) {
			//从左上方来，说明左上方对应字符属于LCS
			i--;
			j--;
			lcs[k] = s1[i];//倒序存的
			k++;
		}
    }
    //print
    for (k--; k>=0; k--)
        printf("%c",lcs[k]);
}
int main(){
    scanf("%s %s", s1, s2);
    memset(b, 0, sizeof(b));
    LCS_LENGTH();
    PRINT_LCS();
    return 0;
}
