
#include <stdio.h>
#include <stdlib.h>

int n,count=0;
int **c;
int *W,*LongWay,**Stations,start=-1,finish=-1;
int *Way2;
void Print(int *Way1)
{
	int i,j;
	count=0;
	for(j=0;j<n;j++)
	if(!(Way1[j]==Way2[j]))
		count++;
	if(count>0)
	
	{
	i=finish;
		  printf("%s=>",Stations[i]);
		 
		   while(Way1[i]!=start)
		  {
			  i=Way1[i];			  
			  printf("%s=>",Stations[i]);
		  }
		  printf("%s",Stations[Way1[i]]);
			printf("\n");
printf("\n");
	}
	
	for(j=0;j<n;j++)

		Way2[j]=Way1[j];




}


void SaveWay(int finish)
{
	int k;
	for(k=0;k<n;k++)LongWay[k]=W[k]; 
	
	 
	Print(LongWay);
}

void Ways(int start, int finish, unsigned char *flag)
{
	int i;
	unsigned char *f;
	f=(unsigned char *)malloc(n*sizeof(unsigned char));
	for(i=0;i<n;i++)f[i]=flag[i];
	if (start!=finish)
	{
		for(i=0;i<n;i++)
		{
			if (!(f[i])&&(c[i][start]!=0))
			{
			

				W[i]=start;
				f[i]=1;
				Ways(i,finish,f);
				f[i]=0;
			
			}
		}
		
	}
	SaveWay(finish);
	free(f);
}

int CmpStr(int *str1,int *str2)
{
	int i=0;
	while((i<16)&&(str1[i]==str2[i]))i++;
	return i;
}

int main()
{
  FILE *f;
  char *argv;
  unsigned char *flag;
  int i,j;  
  int StartStation[16],FinishStation[16],FData[16];
  argv=(char*)malloc(20*sizeof(char));
  printf("enter filename:\n");
  gets(argv);
  if((f=fopen(argv,"r"))!=NULL)
  {
	  fscanf(f,"%u",&n);
	  W=(int *)malloc(n*sizeof(int));
	  LongWay=(int *)malloc(n*sizeof(int));
	  Way2=(int*)malloc(n*sizeof(int));
	  c=(int **)malloc(n*sizeof(int));
	  for(j=0;j<n;j++) c[j]=(int *)malloc(n*sizeof(int));
	  flag=(unsigned char *)malloc(n*sizeof(unsigned char));
	  
	  Stations=(int **)malloc(n*sizeof(int*));
	  for(j=0;j<n;j++) Stations[j]=(int *)malloc(16*sizeof(int));	  
	  
	  printf("Go from ");
	  for(i=0;i<16;i++)StartStation[i]=0;
	  scanf("%15s",&StartStation);
	  printf("To station ");
	  for(i=0;i<16;i++)FinishStation[i]=0;
	  scanf("%15s",&FinishStation);
	  for(i=0;i<n;i++)
	  {
		  flag[i]=0;
		  for(j=0;j<n;j++)fscanf(f,"%u",&c[i][j]);
	  }
	  for(j=0;j<n;j++)
	  {
		  for(i=0;i<16;i++)FData[i]=0;
		  fscanf(f,"%15s",&FData);
		  i=0;
		  while(Stations[j][i]=FData[i])i++;
		  if(CmpStr(FData,StartStation)==16)finish=j;
		  if(CmpStr(FData,FinishStation)==16)start=j;
	  }
	  if((start!=-1)&&(finish!=-1))
	  {
		  for(i=0;i<n;i++)
		  {
			  W[i]=start;
			  LongWay[i]=start;
			  Way2[i]=finish;
			 
		  }
		  flag[start]=1;
		  Ways(start,finish,flag);
		
		  
		
	  }else
		  printf("One of or both stations is not found!!!");
	  for(j=0;j<n;j++) free(c[j]);
	  for(j=0;j<n;j++)
	  {
		  free(Stations[j]);
	  }
	  free(Stations);
	  free(c);
	  free(flag);
	  free(W);
	  free(LongWay);
	 free(Way2);
	  fclose(f);
  }
  else
  {
	  printf("Error!!!\n");
	  printf("Can't to open file %s",argv);
  }
  return 0;
}

