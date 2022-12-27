#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int varibl;
    struct Node* next;
}Node;

void insert_end(Node** root, int value)
{
    Node* new_node = malloc(sizeof(Node));
    if(new_node == NULL)
    {
        exit(1);
    }
    new_node->next = NULL;
    new_node->varibl = value;

    if(*root == NULL)
    {
        *root = new_node;
        return;
    }

    Node* current_variable = *root;
    while(current_variable->next != NULL)
    {
        current_variable = current_variable->next;
    }
    current_variable->next = new_node;


}

void deallocate(Node** root)
{
    Node* current_variable = *root;
    while(current_variable->next != NULL)
    {
        Node* auxiliary_variable = current_variable;
        current_variable = current_variable->next;
        free(auxiliary_variable);
    }
    *root = NULL;
}

void serialize(Node* root)
{
    FILE* file = fopen("text.txt","w");
    if(file == NULL)
    {
        exit(1);
    }
    for(Node* current_variable = root; current_variable != NULL; current_variable = current_variable->next)
    {
        fprintf(file, "%d, ", current_variable->varibl);

    }
    fclose(file);
}


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
