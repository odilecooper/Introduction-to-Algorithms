//最佳调度问题
#include <stdio.h>

int n; //n tasks
int k; //k machines
int minTime;

void bestDisp(int task_num, int *t, int *time, int *disp, int *BestDisp){
	int i, j, needTime=0;
	if (task_num >= n){
		//所有任务都分配完毕
		for (i=0; i<k; i++){
			if (time[i] > needTime)
				needTime = time[i];//计算完成任务的时间
		}
		if (needTime < minTime){
			//记录完成任务时间最早的分配方案
			minTime = needTime;
			for (i=0; i<n; i++)
				BestDisp[i] = disp[i];
		}
	}
	else {
		for (i=0; i<k; i++){
			//给第i个机器分配任务task_num
			time[i] += t[task_num];
			disp[task_num] = i;
			bestDisp(task_num+1, t, time, disp, BestDisp);
			time[i] -= t[task_num];//回溯
			disp[task_num] = -1;//回溯
		}
	}
}

int main(){
	int i, j;
	printf("input n:");
	scanf("%d", &n);
	printf("input k:");
	scanf("%d", &k);
	int t[n];//每个任务所需时间t(i)
	int time[k];//记录每台机器完成任务需要的时间
	int disp[n];//记录分配方案，disp[i]=j表示第i个任务分给第j台机器
	int BestDisp[n];
	printf("input t:");
	for (i=0; i<n; i++){
		scanf("%d",&t[i]);
		minTime += t[i];
	}
	for (i=0; i<n; i++)
		disp[i] = -1;
	for (i=0; i<k; i++)
		time[i] = 0;

	bestDisp(0, t, time, disp, BestDisp);

	for (i=0; i<k; i++){
		printf("machine %d: ", i);
		for (j=0; j<n; j++){
			if (BestDisp[j]==i)
				printf("%d ", j);
		}
		printf("\n");
	}
	return 0;
}
