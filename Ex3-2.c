//动态规划最长子串
//KR算法模式串匹配
#include <stdio.h>
#include <string.h>

#define N 16385
#define M 33
#define d 62
#define q 7

char s1[N], s2[M];
int dp[N][M];

void DP() {
    //在LCS问题的基础上，利用动态规划解决最长子串问题
    //和LCS的差别在于最长子串是连续的，LCS最长子序列可以简短
    //相应地，在b表（此处的dp表）中，只能向左上方看
    //其余部分和LCS代码相同
    int l1,l2,i,j;
    l1=strlen(s1);
    l2=strlen(s2);
    int max = 0;
    for (i=1; i<=l1; i++) {
        for (j=1; j<=l2; j++) {
            if (s1[i-1]==s2[j - 1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = 0;
            }
            if (max < dp[i][j]) {
                max = dp[i][j];
            }
        }    
    }
    printf("Longest-length: %d\n", max);
}

void KR(){
    //KR算法解决模式串匹配问题
    int n, m, i, j;
    int p=0, t=0, h = 1;
    n = strlen(s1);
    m = strlen(s2);
    for (i=0; i<m-1; i++)//计算哈希值
		h = (h*d) % q;  
    for (i=0; i<m; i++) {//计算模式串和被匹配串的哈希值
        p = (d*p + s2[i]) % q;
        t = (d*t + s1[i]) % q;
    }
    for (i=0; i<n-m; i++) {
        if (p == t) {//哈希值相同，看字符是否相同
        	for (j=0; j<m; j++)
                if (s1[i+j] != s2[j]) break;
            if (j == m) 
	            printf("Pattern occurs with shift %d\n", i);
        }
        if (i < n-m) {//继续匹配之后的字符
            t = (d*(t-s1[i]*h) + s1[i+m]) % q;  
            if(t < 0) t += q;
        }
    }
}

int main(){
    scanf("%s %s", s1, s2);
    memset(dp, 0, sizeof(dp));
    DP();
    KR(); 
    return 0;
}
