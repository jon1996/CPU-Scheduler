//Import necessairy library
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define BUFF_SIZE 999999




struct node
{
	int Id;
	int BT;
	int AT;
	int priority;
	int WT;
	int TAT;
	int RT;
	int LHM;
	int ST;
	int BLS;
	bool EXT;
	bool in_Q;
	struct node *next;
};

char buff[BUFF_SIZE];
char buffer_output[BUFF_SIZE * 6];
char *inputFile = NULL;
char *outFile = NULL;
bool firstComeFirst = true;
char *exe;
bool mode = false;
char *mode_char = "OFF";
char *method_char = "None";
bool shortNotPreFirst = true;
bool shortPreFirst = true;
bool priorNotPreFirst = true;
bool priorPreFirst = true;
bool RoundRFirst = true;
int TQ;
char c;

void menu();
void preemptive();
void notpreemotivemenu();
void ModeMenu();
void EndMenu();
void showResult();
void tq_menu();
void firstCome();
void shortNotPre();
void shortPre();
void RoundR();
void priorityNotPre();
void priorityPre();


struct node *header_original = NULL;
struct node *createNode(int pid, int BT, int AT, int priority)
{
	struct node *temp;
	temp = (struct node *)malloc(sizeof(struct node));
	memset(temp, '\0', sizeof(struct node));

	temp->Id = pid;
	temp->BT = BT;
	temp->AT = AT;
	temp->priority = priority;
	temp->WT = 0;
	temp->TAT = 0;
	temp->LHM = BT;
	temp->RT = 0;
	temp->ST = 0;
	temp->BLS = 0;
	if (temp->BT == 0)
		temp->EXT = false;
	temp->in_Q = false;
	temp->next = NULL;

	return temp;
}

struct node *insertBack(struct node *header, int id, int BT, int AT, int priority)
{
	struct node *temp = createNode(id, BT, AT, priority);
	struct node *header_temp;

	// Check if the linked list is empty
	if (header == NULL)
	{
		header = temp;
		return header;
	}

	header_temp = header;
	while (header_temp->next != NULL) // Iterate until we reach the last node
		header_temp = header_temp->next;

	header_temp->next = temp;
	return header;
}

struct node *deleteFront(struct node *header)
{
	struct node *temp;

	if (header == NULL)
	{
		return header;
	}

	temp = header;
	header = header->next;
	memset(temp, '\0', sizeof(struct node));
	free(temp);
	return header;
}



struct node *cloneList(struct node *header)
{
	struct node *header_temp = header;
	struct node *clone_header = NULL;

	while (header_temp != NULL)
	{
		int pid = 0, burst = 0, aRoundRival = 0, priority = 0;
		pid = header_temp->Id;
		burst = header_temp->BT;
		aRoundRival = header_temp->AT;
		priority = header_temp->priority;
		clone_header = insertBack(clone_header, pid, burst, aRoundRival, priority);

		header_temp = header_temp->next;
	}

	return clone_header;
}

void pHelp()
{
	printf("Usage: %s -f <input filename> -o <output filename>\n", exe);
	exit(1);
}

void putInputToList(char *inputFile)
{
	FILE *finput = fopen(inputFile, "r");
	int id_counter = 1;
	if (feof(finput))
	{
		printf("The input file is empty\n");
		exit(1);
	}
	else
	{
		while (!feof(finput)) // Reading the input file and recording the values to Linked List
		{
			int a, b, c;
			fscanf(finput, "%d:%d:%d\n", &a, &b, &c);
			header_original = insertBack(header_original, id_counter, a, b, c);
			id_counter++;
		}
	}
	fclose(finput);
}

int pCounter(struct node *header)
{
	struct node *temp = header;
	int counter = 0;
	while (temp != NULL)
	{
		counter++;
		temp = temp->next;
	}

	return counter;
}

struct node *swapNode(struct node *tmp, struct node *tmp2)
{
	struct node *tmp = tmp2->next;
	tmp2->next = tmp;
	tmp->next = tmp;
	return tmp2;
}

void bubble_sort(struct node **header, int counter, char *sort_mode)
{
	struct node **header_temp;
	int swapped, max_at = 0;
	int i, j;

	for (i = 0; i < counter; i++)
	{
		header_temp = header;
		swapped = 0;
		max_at = 0;

		for (j = 0; j < counter - 1 - i; j++)
		{
			struct node *tmp = *header_temp;
			struct node *tmp2 = tmp->next;

			if (!strcmp(sort_mode, "PID"))
			{
				if (tmp->Id >= tmp2->Id)
				{
					*header_temp = swapNode(tmp, tmp2);
					swapped = 1;
				}
				header_temp = &(*header_temp)->next; // Setting the header_temp's addres to the address of next node which is in the header_temp's address
			}

			else if (!strcmp(sort_mode, "AT"))
			{
				if (tmp->AT > tmp2->AT)
				{
					*header_temp = swapNode(tmp, tmp2);
					swapped = 1;
				}

				else if (tmp->AT == tmp2->AT)
				{
					if (tmp->Id > tmp2->Id)
					{
						*header_temp = swapNode(tmp, tmp2);
						swapped = 1;
					}
				}
				header_temp = &(*header_temp)->next;
			}

			else if (!strcmp(sort_mode, "SJF"))
			{
				if (tmp->AT <= max_at && tmp2->AT <= max_at)
				{
					if (tmp->BT > tmp2->BT)
					{
						*header_temp = swapNode(tmp, tmp2);
						swapped = 1;
					}

					else if (tmp->BT == tmp2->BT)
					{
						if (tmp->Id > tmp2->Id)
						{
							*header_temp = swapNode(tmp, tmp2);
							swapped = 1;
						}
					}
					max_at += (*header_temp)->BT;
				}
				else
				{
					if (tmp2->AT > max_at)
						max_at = tmp2->AT;
				}

				header_temp = &(*header_temp)->next;
			}

			else if (!strcmp(sort_mode, "PS"))
			{
				if (tmp->AT <= max_at && tmp2->AT <= max_at)
				{
					if (tmp->priority > tmp2->priority)
					{
						*header_temp = swapNode(tmp, tmp2);
						swapped = 1;
					}

					else if (tmp->priority == tmp2->priority)
					{
						if (tmp->Id > tmp2->Id)
						{
							*header_temp = swapNode(tmp, tmp2);
							swapped = 1;
						}
					}
					max_at += (*header_temp)->BT;
				}
				else
				{
					if (tmp2->AT > max_at)
						max_at = tmp2->AT;
				}

				header_temp = &(*header_temp)->next;
			}
		}

		if (swapped == 0)
		{
			break;
		}
	}
}

bool isDone(struct node *header)
{
	bool done = true;
	while (header != NULL)
	{
		if (!header->EXT)
			done = false;
		header = header->next;
	}

	return done;
}

bool isPrevDone(struct node *header, int at_limit)
{
	bool done = true;
	while (header != NULL)
	{
		if (header->AT <= at_limit)
		{
			if (!header->EXT)
			{
				done = false;
			}
		}
		header = header->next;
	}

	return done;
}

struct node *findInQ(struct node *header, int at_limit)
{
	struct node *temp = NULL;
	int x = INT_MAX;
	while (header != NULL)
	{
		if (!header->EXT)
		{
			if (header->AT <= at_limit)
			{
				if (header->LHM < x)
				{
					temp = header;
					x = header->LHM;
				}
			}
		}
		header = header->next;
	}

	return temp;
}


struct node *findPriorInQ(struct node *header, int at_limit)
{
	struct node *temp = NULL;
	int x = INT_MAX;
	while (header != NULL)
	{
		if (!header->EXT)
		{
			if (header->AT <= at_limit)
			{
				if (header->priority < x)
				{
					temp = header;
					x = header->priority;
				}
			}
		}
		header = header->next;
	}

	return temp;
}

int main(int argc, char *argv[])
{
    exe = argv[0];
    int options = 0;
   
	while ((options = getopt(argc, argv, "f:o:")) != -1)
	{
		switch (options)
		{
		case 'f':
			inputFile = optarg;
			break;
		case 'o':
			outFile = optarg;
			break;
		default:
			pHelp();
			break;
		}
	}
   
	if (inputFile == NULL || outFile == NULL)
	{
		pHelp();
	}

	FILE *finput = fopen(inputFile, "r");
	if (finput == NULL) // Checking if the input file argument exists.
	{
		printf("this input file does not exist\n");
		exit(1);
	}
	fclose(finput);

	putInputToList(inputFile);

	menu();

	return 0;
}

void menu()
{
  system("clear");
	int n;
  printf("\t\t CPU schedule Simulator\t\n");
  printf("1) Scheduling Method(%s)\n",method_char);
  if(mode==false)
  {
   printf("2) Preemptive Mode (OFF)\n");
    mode_char == "OFF";

  }else{
      printf("2) Preemptive Mode (ON)\n");
      mode_char == "ON";
  }

  printf("3) Show Result\n");
  printf("4) End Program\n");
  printf("Option > \n");
  scanf("%d",&n);
  if(n==1)
  {
      if(mode==false)
      {
           preemptive();
      }
      else if(mode==true)
      {
          notpreemotivemenu();
      }


  }else if(n ==2)
  {
      ModeMenu();
  }else if(n == 3)
  {
     showResult();
  }else if(n == 4)
  {
      EndMenu();
  }
  else
    {
        printf("Please select a valid option\n");
        system("sleep 2");
        menu();
    }
}

void notpreemotivemenu()
{
    system("clear");
    int n;
    printf("\t\t Menu \t\n");
    printf("1-) None: None of scheduling method chosen\n");
    printf("2-) Shortest-Job-First Scheduling\n");
    printf("3-) Priority Scheduling\n");
    printf("Option > ");
    scanf("%1d", &n);
    if(n==1)
    {

        method_char = "None";
         menu();
    }
    else if(n==2)
    {

        method_char = "SJS";
		shortNotPre();
          menu();
    }
    else if(n==3)
    {
        method_char = "Priority";
		priorityNotPre();
        menu();
    }
    else
    {
        printf("Please select a valid option\n");
        system("sleep 2");
        notpreemotivemenu();
    }
}

void preemptive()
{
    system("clear");
    int n;
    printf("1-) None: None of scheduling method chosen\n");
		printf("2-) First Come, First Served Scheduling\n");
		printf("3-) Shortest-Job-First Scheduling\n");
		printf("4-) Priority Scheduling\n");
		printf("5-) Round-Robin Scheduling\n");
		printf("Option > ");
    scanf("%d", &n);
    if(n==1)
    {

        method_char = "None";
         menu();
    }
    else if(n==2)
    {

        method_char = "firstCome";
		firstCome();
          menu();
    }
    else if(n==3)
    {
        method_char = "SJB";
		shortPre();
        menu();
    }
    else if(n==4)
    {
        method_char = "Priority";
		priorityPre();
        menu();
    }
    else if(n==5)
    {
        method_char = "RoundR";
		RoundR();
        menu();
    }else
    {
        printf("Please select a valid option\n");
        system("sleep 2");
        preemptive();
    }
}

void ModeMenu()
{
    system("clear");
    int n;
    printf("\t\t Change mode \t\n");
    if(method_char=="None")
    {
    printf("There is no selected scheduling method in the Main Menu \n");
    printf("1-) ON\n");
    printf("2-) OFF\n");
    printf("3) Back to main menu \n");
     printf("Option > ");
    }
    else if(method_char!="None"){
        printf("1-) ON\n");
    printf("2-) OFF\n");
    printf("Option > ");
    }

    scanf("%1d", &n);
    if(n==1)
    {
        mode=true;
         mode_char == "ON";
         if(method_char == "firstCome" ||method_char == "RoundR")
        {
            system("clear");
            printf("preemptive mode is not available for selected Scheduling Method\n");
            printf("Select another shedule method for this mode\n");
            system("sleep 3");
             ModeMenu();
        }
        else{
             menu();
        }

    }else if(n==2){
    mode=false;
     mode_char == "OFF";
    menu();
    }
    else if(n==3)
    {
         menu();
    }
    else
    {
        printf("Please select a valid option\n");
        system("sleep 2");
        ModeMenu();
    }
}


void tq_menu()
{
	while (true)
	{
		system("clear");
		printf("Please enter the time quantum for Round-Robin Method\n");
		printf("Time Quantum > ");
		scanf("%d", &TQ);
		break;
	}
}

void EndMenu()
{
	if (TQ == 0)
		tq_menu();
	if (firstComeFirst)
	{
		firstCome();
	}
	if (shortNotPreFirst)
	{
		shortNotPre();
	}
	if (shortPreFirst)
	{
		shortPre();
	}
	if (priorNotPreFirst)
	{
		priorityNotPre();
	}
	if (priorPreFirst)
	{
		priorityPre();
	}
	if (RoundRFirst)
	{
		RoundR();
	}

	printf("%s", buffer_output);
	FILE *f = fopen(outFile, "w");
	fprintf(f, "%s", buffer_output);
	fclose(f);

	while (header_original != NULL)
	{
		header_original = deleteFront(header_original);
	}

	exit(0);
}

void showResult(){
	if(method_char == "None")
    {
        menu();
    }else{
		firstCome();
		shortNotPre();
		shortPre();
		priorityNotPre();
		priorityPre();
		printf("Press enter to return to the main menu.\n");
		getchar();
		getchar();
		menu();
	}
	
	

}
void firstCome()
{
	struct node *clone_header = cloneList(header_original);
	struct node *tmp, *tmp2, *t;
	int program_counter = 0;
	float AWT = 0.0f;
	int number_of_process = pCounter(clone_header);
	bool is_first = true;
	bubble_sort(&clone_header, number_of_process, "AT");
	tmp = cloneList(clone_header);
	while (clone_header != NULL)
	{
		clone_header = deleteFront(clone_header);
	}
	t = tmp2 = tmp;

	while (tmp != NULL)
	{
		if (tmp->AT <= program_counter)
		{
			program_counter += tmp->BT;
			tmp->TAT = program_counter;
			if (is_first)
			{
				if ((tmp->WT = tmp->TAT - tmp->BT) < 0)
					tmp->WT = 0;
				is_first = false;
			}
			else
			{
				if ((tmp->WT = tmp->TAT - tmp->BT - tmp->AT) < 0)
					tmp->WT = 0;
			}
		}

		else
		{
			program_counter = tmp->AT;
			program_counter += tmp->BT;
			tmp->TAT = program_counter;
			if ((tmp->WT = tmp->TAT - tmp->BT - tmp->AT) < 0)
				tmp->WT = 0;
		}

		tmp = tmp->next;
	}

	strcpy(buff, "");
	bubble_sort(&tmp2, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: First Come First Served\n");
	strcat(buff, "Process\n");
	while (tmp2 != NULL)
	{
		int pid = tmp2->Id;
		int wait = tmp2->WT;
		AWT += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		tmp2 = tmp2->next;
	}
	AWT /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", AWT);
	strcat(buff, buff_2);
	if (firstComeFirst)
	{
		strcat(buffer_output, buff);
		firstComeFirst = false;
	}
	printf("%s", buff);

	while (t != NULL)
	{
		t = deleteFront(t);
	}
}

void shortNotPre()
{

	struct node *clone_header = cloneList(header_original);
	struct node *temp, *tmp, *t;
	int program_counter = 0;
	float AWT = 0.0f;
	int number_of_process = pCounter(clone_header);
	bubble_sort(&clone_header, number_of_process, "AT");
	bubble_sort(&clone_header, number_of_process, "SJF");
	temp = cloneList(clone_header);
	while (clone_header != NULL)
	{
		clone_header = deleteFront(clone_header);
	}
	t = tmp = temp;

	bool is_first = true;
	while (temp != NULL)
	{
		if (temp->AT <= program_counter)
		{
			program_counter += temp->BT;
			temp->TAT = program_counter;
			if (is_first)
			{
				if ((temp->WT = temp->TAT - temp->BT) < 0)
					temp->WT = 0;
				is_first = false;
			}
			else
			{
				if ((temp->WT = temp->TAT - temp->BT - temp->AT) < 0)
					temp->WT = 0;
			}
		}

		else
		{
			program_counter = temp->AT;
			program_counter += temp->BT;
			temp->TAT = program_counter;
			if ((temp->WT = temp->TAT - temp->BT - temp->AT) < 0)
				temp->WT = 0;
		}

		temp = temp->next;
	}

	strcpy(buff, "");
	bubble_sort(&tmp, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: Shortest Job First (Non-preemptive)\n");
	strcat(buff, "Process\n");
	while (tmp != NULL)
	{
		int pid = tmp->Id;
		int wait = tmp->WT;
		AWT += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		tmp = tmp->next;
	}
	AWT /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", AWT);
	strcat(buff, buff_2);
	if (shortNotPreFirst)
	{
		strcat(buffer_output, buff);
		shortNotPreFirst = false;
	}
	printf("%s", buff);

	while (t != NULL)
	{
		t = deleteFront(t);
	}
}

void shortPre()
{
	struct node *clone_header = cloneList(header_original);
	struct node *temp, *tmp, *tmp2;
	int program_counter = 0;
	float AWT = 0.0f;
	int number_of_process = pCounter(clone_header);
	bubble_sort(&clone_header, number_of_process, "AT");
	bubble_sort(&clone_header, number_of_process, "SJF");
	temp = tmp = tmp2 = clone_header;

	while (!isDone(clone_header))
	{
		struct node *in_Q_node = findInQ(clone_header, program_counter);
		bool is_found = true;
		if (in_Q_node == NULL)
		{
			temp = clone_header;
			while (temp != NULL)
			{
				if (!temp->EXT)
				{
					if (temp->AT > program_counter && is_found)
					{
						is_found = false;
						program_counter = temp->AT;
						in_Q_node = findInQ(clone_header, program_counter);
						in_Q_node->LHM--;
						program_counter++;
						if (in_Q_node->LHM == 0)
						{
							in_Q_node->TAT = program_counter;
							in_Q_node->EXT = true;
						}
					}
				}

				temp = temp->next;
			}
		}
		else
		{
			program_counter++;
			in_Q_node->LHM--;

			if (in_Q_node->LHM == 0)
			{
				in_Q_node->TAT = program_counter;
				in_Q_node->EXT = true;
			}
		}
	}

	bool is_first = true;
	while (tmp != NULL)
	{
		if (is_first)
		{
			tmp->WT = tmp->TAT - tmp->BT;
			if (tmp->WT < 0)
				tmp->WT = 0;

			is_first = false;
		}

		else
		{
			tmp->WT = tmp->TAT - tmp->BT - tmp->AT;
			if (tmp->WT < 0)
				tmp->WT = 0;
		}

		tmp = tmp->next;
	}

	strcpy(buff, "");
	bubble_sort(&tmp2, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: Shortest Job First Scheduling (preemptive)\n");
	strcat(buff, "Process\n");
	while (tmp2 != NULL)
	{
		int pid = tmp2->Id;
		int wait = tmp2->WT;
		AWT += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		tmp2 = tmp2->next;
	}
	AWT /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", AWT);
	strcat(buff, buff_2);
	if (shortPreFirst)
	{
		strcat(buffer_output, buff);
		shortPreFirst = false;
	}
	printf("%s", buff);

	while (clone_header != NULL)
	{
		clone_header = deleteFront(clone_header);
	}
}


void RoundR()
{
	struct node *clone_header = cloneList(header_original);
	struct node *tmp, *tmp2, *temp3;
	int program_counter = 0;
	float AWT = 0.0f;
	int number_of_process = pCounter(clone_header);
	bool is_first = true;
	bool previous_ones_done = false;
	bubble_sort(&clone_header, number_of_process, "AT");
	tmp = tmp2 = temp3 = clone_header;

	while (temp3 != NULL)
	{
		temp3->ST = temp3->BT / TQ;
		temp3->BLS = temp3->BT % TQ;
		temp3 = temp3->next;
	}

	while (!isDone(clone_header))
	{
		tmp = clone_header;
		is_first = true;
		while (tmp != NULL)
		{
			if (!tmp->EXT)
			{
				if (tmp->AT <= program_counter)
				{
					if (is_first)
					{
						if (tmp->ST == 0)
						{
							program_counter += tmp->BLS;
							if (tmp->BLS != 0)
								tmp->TAT = program_counter;
							tmp->WT = tmp->TAT - tmp->BT;
							if (tmp->WT < 0)
								tmp->WT = 0;
							tmp->EXT = true;
						}
						else
						{
							program_counter += TQ;
							tmp->TAT = program_counter;
							tmp->ST--;
						}
						is_first = false;
					}

					else
					{
						if (tmp->ST == 0)
						{
							program_counter += tmp->BLS;
							if (tmp->BLS != 0)
								tmp->TAT = program_counter;
							tmp->WT = tmp->TAT - tmp->BT - tmp->AT;
							if (tmp->WT < 0)
								tmp->WT = 0;
							tmp->EXT = true;
						}
						else
						{
							program_counter += TQ;
							tmp->TAT = program_counter;
							tmp->ST--;
						}
					}
				}

				else
				{
					previous_ones_done = isPrevDone(clone_header, program_counter);
					if (previous_ones_done)
					{
						program_counter = tmp->AT;
						if (tmp->ST == 0)
						{
							program_counter += tmp->BLS;
							if (tmp->BLS != 0)
								tmp->TAT = program_counter;
							tmp->WT = tmp->TAT - tmp->BT - tmp->AT;
							if (tmp->WT < 0)
								tmp->WT = 0;
							tmp->EXT = true;
						}
						else
						{
							program_counter += TQ;
							tmp->TAT = program_counter;
							tmp->ST--;
						}
					}
				}
			}

			is_first = false;
			tmp = tmp->next;
		}
	}

	strcpy(buff, "");
	bubble_sort(&clone_header, number_of_process, "PID");
	tmp2 = clone_header;
	system("clear");
	snprintf(buff, 999, "Scheduling Method: Round-Robin (Time quantum: %d)\n", TQ);
	strcat(buff, "Process\n");
	while (tmp2 != NULL)
	{
		int pid = tmp2->Id;
		int wait = tmp2->WT;
		AWT += wait;
		char buff_1[20];
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		tmp2 = tmp2->next;
	}
	AWT /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", AWT);
	strcat(buff, buff_2);
	if (RoundRFirst)
	{
		strcat(buffer_output, buff);
		RoundRFirst = false;
	}
	printf("%s", buff);
	while (clone_header != NULL)
	{
		clone_header = deleteFront(clone_header);
	}
}

void priorityNotPre()
{
	struct node *clone_header = cloneList(header_original);
	struct node *temp, *tmp, *t;
	int program_counter = 0;
	float AWT = 0.0f;
	int number_of_process = pCounter(clone_header);
	bubble_sort(&clone_header, number_of_process, "AT");
	bubble_sort(&clone_header, number_of_process, "PS");
	temp = cloneList(clone_header);
	while (clone_header != NULL)
	{
		clone_header = deleteFront(clone_header);
	}
	t = tmp = temp;

	bool is_first = true;
	while (temp != NULL)
	{
		if (temp->AT <= program_counter)
		{
			program_counter += temp->BT;
			temp->TAT = program_counter;
			if (is_first)
			{
				if ((temp->WT = temp->TAT - temp->BT) < 0)
					temp->WT = 0;
				is_first = false;
			}
			else
			{
				if ((temp->WT = temp->TAT - temp->BT - temp->AT) < 0)
					temp->WT = 0;
			}
		}

		else
		{
			program_counter = temp->AT;
			program_counter += temp->BT;
			temp->TAT = program_counter;
			if ((temp->WT = temp->TAT - temp->BT - temp->AT) < 0)
				temp->WT = 0;
		}

		temp = temp->next;
	}

	strcpy(buff, "");
	bubble_sort(&tmp, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: Priority Scheduling (Non-preemptive)\n");
	strcat(buff, "Process\n");
	while (tmp != NULL)
	{
		int pid = tmp->Id;
		int wait = tmp->WT;
		AWT += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		tmp = tmp->next;
	}
	AWT /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", AWT);
	strcat(buff, buff_2);
	if (priorNotPreFirst)
	{
		strcat(buffer_output, buff);
		priorNotPreFirst = false;
	}
	printf("%s", buff);

	while (t != NULL)
	{
		t = deleteFront(t);
	}
}


void priorityPre()
{
	struct node *clone_header = cloneList(header_original);
	struct node *temp, *tmp, *tmp2;
	int program_counter = 0;
	float AWT = 0.0f;
	int number_of_process = pCounter(clone_header);
	bubble_sort(&clone_header, number_of_process, "AT");
	bubble_sort(&clone_header, number_of_process, "PS");
	temp = tmp = tmp2 = clone_header;

	while (!isDone(clone_header))
	{
		struct node *in_Q_node = findPriorInQ(clone_header, program_counter);
		bool is_found = true;

		if (in_Q_node == NULL)
		{
			temp = clone_header;
			while (temp != NULL)
			{
				if (!temp->EXT)
				{
					if (temp->AT > program_counter && is_found)
					{
						is_found = false;
						program_counter = temp->AT;
						in_Q_node = findPriorInQ(clone_header, program_counter);
						in_Q_node->LHM--;
						program_counter++;
						if (in_Q_node->LHM == 0)
						{
							in_Q_node->TAT = program_counter;
							in_Q_node->EXT = true;
						}
					}
				}

				temp = temp->next;
			}
		}

		else
		{
			program_counter++;
			in_Q_node->LHM--;

			if (in_Q_node->LHM == 0)
			{
				in_Q_node->TAT = program_counter;
				in_Q_node->EXT = true;
			}
		}
	}

	bool is_first = true;
	while (tmp != NULL)
	{
		if (is_first)
		{
			tmp->WT = tmp->TAT - tmp->BT;
			if (tmp->WT < 0)
				tmp->WT = 0;

			is_first = false;
		}

		else
		{
			tmp->WT = tmp->TAT - tmp->BT - tmp->AT;
			if (tmp->WT < 0)
				tmp->WT = 0;
		}
		tmp = tmp->next;
	}

	strcpy(buff, "");
	bubble_sort(&tmp2, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: Priority Scheduling (preemptive)\n");
	strcat(buff, "Process\n");
	while (tmp2 != NULL)
	{
		int pid = tmp2->Id;
		int wait = tmp2->WT;
		AWT += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		tmp2 = tmp2->next;
	}
	AWT /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", AWT);
	strcat(buff, buff_2);
	if (priorPreFirst)
	{
		strcat(buffer_output, buff);
		priorPreFirst = false;
	}
	printf("%s", buff);
	while (clone_header != NULL)
	{
		clone_header = deleteFront(clone_header);
	}
}

