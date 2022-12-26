#include <stdio.h>

int main() {
    int n;
  printf("\t\t CPU schedule Simulator\t\n");
  printf("1) Scheduling Method(None)\n");
  printf("2) Preemptive Mode\n");
  printf("3) Show Result\n");
  printf("4) End Program\n");
  printf("Option > \n");
  scanf("%d",&n);
  if(n==1)
  {
      printf("Scheduling Method(None)");
  }else if(n ==2)
  {
      printf("Preemptive Mode");
  }else if(n == 3)
  {
      printf("Show Result");
  }else if(n == 4)
  {
      printf("End Program");
  }
  return 0;
}
