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
	int process_id;
	int burst_time;
	int arrival_time;
	int priority;
	int waiting_time;
	int turnaround_time;
	int first_response;
	int how_much_left;
	int time_slices;
	int last_slice_burst;
	bool is_terminated;
	bool in_cpu;
	struct node *next;
};

char buff[BUFF_SIZE];
char buffer_output[BUFF_SIZE * 6];
char *input_filename = NULL;
char *output_filename = NULL;
bool fcfs_first = true;
char *exe;
bool mode = false;
char *mode_char = "OFF";
char *method_char = "None";
bool fcfs_first = true;
bool sjf_np_first = true;
bool sjf_p_first = true;
bool ps_np_first = true;
bool ps_p_first = true;
bool rr_first = true;
int time_quantum;

struct node *header_original = NULL;
struct node *create_node(int pid, int burst_time, int arrival_time, int priority)
{
	struct node *temp;
	temp = (struct node *)malloc(sizeof(struct node));
	memset(temp, '\0', sizeof(struct node));

	temp->process_id = pid;
	temp->burst_time = burst_time;
	temp->arrival_time = arrival_time;
	temp->priority = priority;
	temp->waiting_time = 0;
	temp->turnaround_time = 0;
	temp->how_much_left = burst_time;
	temp->first_response = 0;
	temp->time_slices = 0;
	temp->last_slice_burst = 0;
	if (temp->burst_time == 0)
		temp->is_terminated = false;
	temp->in_cpu = false;
	temp->next = NULL;

	return temp;
}

// Insert back to the LL (Function)
struct node *insert_back(struct node *header, int id, int burst_time, int arrival_time, int priority)
{
	struct node *temp = create_node(id, burst_time, arrival_time, priority);
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

// Delete front of the LL (Function)
struct node *delete_front(struct node *header)
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

// Displaying the Linked List Items(For Debugging Purposes Only) (Function)
void display_LL(struct node *header)
{
	struct node *temp = header;
	while (temp != NULL)
	{
		int a, b, c, d, e, f, g, h, i, j;
		bool t;
		a = temp->process_id;
		b = temp->burst_time;
		c = temp->arrival_time;
		d = temp->priority;
		e = temp->waiting_time;
		f = temp->turnaround_time;
		g = temp->how_much_left;
		h = temp->first_response;
		i = temp->time_slices;
		j = temp->last_slice_burst;

		printf("ID:%d\tBurst:%d\tArrival:%d\tPriority:%d\tWait:%d\tTurn:%d\tLeft:%d\tResponse:%d\tSlices:%d\tLastSlice:%d\n", a, b, c, d, e, f, g, h, i, j);
		temp = temp->next;
	}

	getchar();
	getchar();
}

// Cloning Main LL (Function)
struct node *clone_LL(struct node *header)
{
	struct node *header_temp = header;
	struct node *clone_header = NULL;

	while (header_temp != NULL)
	{
		int pid = 0, burst = 0, arrival = 0, priority = 0;
		pid = header_temp->process_id;
		burst = header_temp->burst_time;
		arrival = header_temp->arrival_time;
		priority = header_temp->priority;
		clone_header = insert_back(clone_header, pid, burst, arrival, priority);

		header_temp = header_temp->next;
	}

	return clone_header;
}

// This funtions is used to print programs usage and what arguments are needed to pass (Function)

void print_usage()
{
	printf("Usage: %s -f <input filename> -o <output filename>\n", exe);
	exit(1);
}
// Reading from Input File to Write it to LL (Function)
void write_input_to_LL(char *input_filename)
{
	FILE *finput = fopen(input_filename, "r");
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
			header_original = insert_back(header_original, id_counter, a, b, c);
			id_counter++;
		}
	}
	fclose(finput);
}
void fcfs();

// Counts How many process' are in the LL (Function)
int process_counter(struct node *header)
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

// Swapping nodes (Function)
struct node *swap_nodes(struct node *temp1, struct node *temp2)
{
	struct node *tmp = temp2->next;
	temp2->next = temp1;
	temp1->next = tmp;
	return temp2;
}

// Sorts LL in ascending order (Function)
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
			struct node *temp1 = *header_temp;
			struct node *temp2 = temp1->next;

			if (!strcmp(sort_mode, "PID"))
			{
				if (temp1->process_id >= temp2->process_id)
				{
					*header_temp = swap_nodes(temp1, temp2);
					swapped = 1;
				}
				header_temp = &(*header_temp)->next; // Setting the header_temp's addres to the address of next node which is in the header_temp's address
			}

			else if (!strcmp(sort_mode, "AT"))
			{
				if (temp1->arrival_time > temp2->arrival_time)
				{
					*header_temp = swap_nodes(temp1, temp2);
					swapped = 1;
				}

				else if (temp1->arrival_time == temp2->arrival_time)
				{
					if (temp1->process_id > temp2->process_id)
					{
						*header_temp = swap_nodes(temp1, temp2);
						swapped = 1;
					}
				}
				header_temp = &(*header_temp)->next;
			}

			else if (!strcmp(sort_mode, "SJF"))
			{
				if (temp1->arrival_time <= max_at && temp2->arrival_time <= max_at)
				{
					if (temp1->burst_time > temp2->burst_time)
					{
						*header_temp = swap_nodes(temp1, temp2);
						swapped = 1;
					}

					else if (temp1->burst_time == temp2->burst_time)
					{
						if (temp1->process_id > temp2->process_id)
						{
							*header_temp = swap_nodes(temp1, temp2);
							swapped = 1;
						}
					}
					max_at += (*header_temp)->burst_time;
				}
				else
				{
					if (temp2->arrival_time > max_at)
						max_at = temp2->arrival_time;
				}

				header_temp = &(*header_temp)->next;
			}

			else if (!strcmp(sort_mode, "PS"))
			{
				if (temp1->arrival_time <= max_at && temp2->arrival_time <= max_at)
				{
					if (temp1->priority > temp2->priority)
					{
						*header_temp = swap_nodes(temp1, temp2);
						swapped = 1;
					}

					else if (temp1->priority == temp2->priority)
					{
						if (temp1->process_id > temp2->process_id)
						{
							*header_temp = swap_nodes(temp1, temp2);
							swapped = 1;
						}
					}
					max_at += (*header_temp)->burst_time;
				}
				else
				{
					if (temp2->arrival_time > max_at)
						max_at = temp2->arrival_time;
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

// Checking if all the processes are done returning true if all done (Function)
bool is_all_done(struct node *header)
{
	bool done = true;
	while (header != NULL)
	{
		if (!header->is_terminated)
			done = false;
		header = header->next;
	}

	return done;
}

// Checking if all the processes before arrival time is done (Function)
bool is_previous_ones_done(struct node *header, int at_limit)
{
	bool done = true;
	while (header != NULL)
	{
		if (header->arrival_time <= at_limit)
		{
			if (!header->is_terminated)
			{
				done = false;
			}
		}
		header = header->next;
	}

	return done;
}

// Finding the node which has the least time left
struct node *find_least_left(struct node *header, int at_limit)
{
	struct node *temp = NULL;
	int x = INT_MAX;
	while (header != NULL)
	{
		if (!header->is_terminated)
		{
			if (header->arrival_time <= at_limit)
			{
				if (header->how_much_left < x)
				{
					temp = header;
					x = header->how_much_left;
				}
			}
		}
		header = header->next;
	}

	return temp;
}

// Finding the node which has the least priority
struct node *find_least_priority(struct node *header, int at_limit)
{
	struct node *temp = NULL;
	int x = INT_MAX;
	while (header != NULL)
	{
		if (!header->is_terminated)
		{
			if (header->arrival_time <= at_limit)
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
    // Here we check if the correct options are used
	while ((options = getopt(argc, argv, "f:o:")) != -1)
	{
		switch (options)
		{
		case 'f':
			input_filename = optarg;
			break;
		case 'o':
			output_filename = optarg;
			break;
		default:
			print_usage();
			break;
		}
	}
    // Here we check if the arguments are passed for options
	if (input_filename == NULL || output_filename == NULL)
	{
		print_usage();
	}

	FILE *finput = fopen(input_filename, "r");
	if (finput == NULL) // Checking if the input file argument exists.
	{
		printf("this input file does not exist\n");
		exit(1);
	}
	fclose(finput);

	write_input_to_LL(input_filename);

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
           menu4();
      }
      else if(mode==true)
      {
          notpreemotivemenu();
      }


  }else if(n ==2)
  {
      menu2();
  }else if(n == 3)
  {
      printf("Show Result");
  }else if(n == 4)
  {
      printf("End Program");
  }
  else
    {
        printf("Please select a valid option\n");
        system("sleep 2");
        menu();
    }
}
//Not Preemptive
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
          menu();
    }
    else if(n==3)
    {
        method_char = "Priority";
        menu();
    }
    else
    {
        printf("Please select a valid option\n");
        system("sleep 2");
        notpreemotivemenu();
    }
}
// preemtive mode is on
void menu4()
{
    system("clear");
    int n;
    printf("1-) None: None of scheduling method chosen\n");
		printf("2-) First Come, First Served Scheduling\n");
		printf("3-) Shortest-Job-First Scheduling\n");
		printf("4-) Priority Scheduling\n");
		printf("5-) Round-Robin Scheduling\n");
		printf("Option > ");
    scanf("%1d", &n);
    if(n==1)
    {

        method_char = "None";
         menu();
    }
    else if(n==2)
    {

        method_char = "FCFS";
		fcfs();
          menu();
    }
    else if(n==3)
    {
        method_char = "SJB";
        menu();
    }
    else if(n==4)
    {
        method_char = "Priority";
        menu();
    }
    else if(n==5)
    {
        method_char = "RR";
        menu();
    }else
    {
        printf("Please select a valid option\n");
        system("sleep 2");
        menu4();
    }
}
//Change mode
void menu2()
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
         if(method_char == "FCFS" ||method_char == "RR")
        {
            system("clear");
            printf("Preemtive mode is not available for selected Scheduling Method\n");
            printf("Select another shedule method for this mode\n");
            system("sleep 3");
             menu2();
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
        menu2();
    }
}
// First-Come-First-Serve (Function)
void fcfs()
{
	struct node *clone_header = clone_LL(header_original);
	struct node *temp1, *temp2, *t;
	int program_counter = 0;
	float average_wait = 0.0f;
	int number_of_process = process_counter(clone_header);
	bool is_first = true;
	bubble_sort(&clone_header, number_of_process, "AT");
	temp1 = clone_LL(clone_header);
	while (clone_header != NULL)
	{
		clone_header = delete_front(clone_header);
	}
	t = temp2 = temp1;

	while (temp1 != NULL)
	{
		if (temp1->arrival_time <= program_counter)
		{
			program_counter += temp1->burst_time;
			temp1->turnaround_time = program_counter;
			if (is_first)
			{
				if ((temp1->waiting_time = temp1->turnaround_time - temp1->burst_time) < 0)
					temp1->waiting_time = 0;
				is_first = false;
			}
			else
			{
				if ((temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time) < 0)
					temp1->waiting_time = 0;
			}
		}

		else
		{
			program_counter = temp1->arrival_time;
			program_counter += temp1->burst_time;
			temp1->turnaround_time = program_counter;
			if ((temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time) < 0)
				temp1->waiting_time = 0;
		}

		temp1 = temp1->next;
	}

	strcpy(buff, "");
	bubble_sort(&temp2, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: First Come First Served\n");
	strcat(buff, "Process Waiting Times:\n");
	while (temp2 != NULL)
	{
		int pid = temp2->process_id;
		int wait = temp2->waiting_time;
		average_wait += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		temp2 = temp2->next;
	}
	average_wait /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
	strcat(buff, buff_2);
	if (fcfs_first)
	{
		strcat(buffer_output, buff);
		fcfs_first = false;
	}
	printf("%s", buff);

	while (t != NULL)
	{
		t = delete_front(t);
	}
}

// Shortes-Job-First Non-Preemtive (Function)
void sjf_np()
{
	// I have first tried selection sort but could not figure it out...
	//...(There were complications regarding to non-adjacent nodes)
	// So I changed the sorting algorithm to bubble sort
	struct node *clone_header = clone_LL(header_original);
	struct node *temp, *temp1, *t;
	int program_counter = 0;
	float average_wait = 0.0f;
	int number_of_process = process_counter(clone_header);
	bubble_sort(&clone_header, number_of_process, "AT");
	bubble_sort(&clone_header, number_of_process, "SJF");
	temp = clone_LL(clone_header);
	while (clone_header != NULL)
	{
		clone_header = delete_front(clone_header);
	}
	t = temp1 = temp;

	bool is_first = true;
	while (temp != NULL)
	{
		if (temp->arrival_time <= program_counter)
		{
			program_counter += temp->burst_time;
			temp->turnaround_time = program_counter;
			if (is_first)
			{
				if ((temp->waiting_time = temp->turnaround_time - temp->burst_time) < 0)
					temp->waiting_time = 0;
				is_first = false;
			}
			else
			{
				if ((temp->waiting_time = temp->turnaround_time - temp->burst_time - temp->arrival_time) < 0)
					temp->waiting_time = 0;
			}
		}

		else
		{
			program_counter = temp->arrival_time;
			program_counter += temp->burst_time;
			temp->turnaround_time = program_counter;
			if ((temp->waiting_time = temp->turnaround_time - temp->burst_time - temp->arrival_time) < 0)
				temp->waiting_time = 0;
		}

		temp = temp->next;
	}

	strcpy(buff, "");
	bubble_sort(&temp1, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: Shortest Job First (Non-Preemtive)\n");
	strcat(buff, "Process Waiting Times:\n");
	while (temp1 != NULL)
	{
		int pid = temp1->process_id;
		int wait = temp1->waiting_time;
		average_wait += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		temp1 = temp1->next;
	}
	average_wait /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
	strcat(buff, buff_2);
	if (sjf_np_first)
	{
		strcat(buffer_output, buff);
		sjf_np_first = false;
	}
	printf("%s", buff);

	while (t != NULL)
	{
		t = delete_front(t);
	}
}

// Shortest-Job-First Preemtive (Function)
void sjf_p()
{
	struct node *clone_header = clone_LL(header_original);
	struct node *temp, *temp1, *temp2;
	int program_counter = 0;
	float average_wait = 0.0f;
	int number_of_process = process_counter(clone_header);
	bubble_sort(&clone_header, number_of_process, "AT");
	bubble_sort(&clone_header, number_of_process, "SJF");
	temp = temp1 = temp2 = clone_header;

	while (!is_all_done(clone_header))
	{
		struct node *in_cpu_node = find_least_left(clone_header, program_counter);
		bool is_found = true;
		if (in_cpu_node == NULL)
		{
			temp = clone_header;
			while (temp != NULL)
			{
				if (!temp->is_terminated)
				{
					if (temp->arrival_time > program_counter && is_found)
					{
						is_found = false;
						program_counter = temp->arrival_time;
						in_cpu_node = find_least_left(clone_header, program_counter);
						in_cpu_node->how_much_left--;
						program_counter++;
						if (in_cpu_node->how_much_left == 0)
						{
							in_cpu_node->turnaround_time = program_counter;
							in_cpu_node->is_terminated = true;
						}
					}
				}

				temp = temp->next;
			}
		}
		else
		{
			program_counter++;
			in_cpu_node->how_much_left--;

			if (in_cpu_node->how_much_left == 0)
			{
				in_cpu_node->turnaround_time = program_counter;
				in_cpu_node->is_terminated = true;
			}
		}
	}

	bool is_first = true;
	while (temp1 != NULL)
	{
		if (is_first)
		{
			temp1->waiting_time = temp1->turnaround_time - temp1->burst_time;
			if (temp1->waiting_time < 0)
				temp1->waiting_time = 0;

			is_first = false;
		}

		else
		{
			temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time;
			if (temp1->waiting_time < 0)
				temp1->waiting_time = 0;
		}

		temp1 = temp1->next;
	}

	strcpy(buff, "");
	bubble_sort(&temp2, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: Shortest Job First Scheduling (Preemtive)\n");
	strcat(buff, "Process Waiting Times:\n");
	while (temp2 != NULL)
	{
		int pid = temp2->process_id;
		int wait = temp2->waiting_time;
		average_wait += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		temp2 = temp2->next;
	}
	average_wait /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
	strcat(buff, buff_2);
	if (sjf_p_first)
	{
		strcat(buffer_output, buff);
		sjf_p_first = false;
	}
	printf("%s", buff);

	while (clone_header != NULL)
	{
		clone_header = delete_front(clone_header);
	}
}

// Priority Scheduling Non-Preemtive (Function)
void ps_np()
{
	struct node *clone_header = clone_LL(header_original);
	struct node *temp, *temp1, *t;
	int program_counter = 0;
	float average_wait = 0.0f;
	int number_of_process = process_counter(clone_header);
	bubble_sort(&clone_header, number_of_process, "AT");
	bubble_sort(&clone_header, number_of_process, "PS");
	temp = clone_LL(clone_header);
	while (clone_header != NULL)
	{
		clone_header = delete_front(clone_header);
	}
	t = temp1 = temp;

	bool is_first = true;
	while (temp != NULL)
	{
		if (temp->arrival_time <= program_counter)
		{
			program_counter += temp->burst_time;
			temp->turnaround_time = program_counter;
			if (is_first)
			{
				if ((temp->waiting_time = temp->turnaround_time - temp->burst_time) < 0)
					temp->waiting_time = 0;
				is_first = false;
			}
			else
			{
				if ((temp->waiting_time = temp->turnaround_time - temp->burst_time - temp->arrival_time) < 0)
					temp->waiting_time = 0;
			}
		}

		else
		{
			program_counter = temp->arrival_time;
			program_counter += temp->burst_time;
			temp->turnaround_time = program_counter;
			if ((temp->waiting_time = temp->turnaround_time - temp->burst_time - temp->arrival_time) < 0)
				temp->waiting_time = 0;
		}

		temp = temp->next;
	}

	strcpy(buff, "");
	bubble_sort(&temp1, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: Priority Scheduling (Non-Preemtive)\n");
	strcat(buff, "Process Waiting Times:\n");
	while (temp1 != NULL)
	{
		int pid = temp1->process_id;
		int wait = temp1->waiting_time;
		average_wait += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		temp1 = temp1->next;
	}
	average_wait /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
	strcat(buff, buff_2);
	if (ps_np_first)
	{
		strcat(buffer_output, buff);
		ps_np_first = false;
	}
	printf("%s", buff);

	while (t != NULL)
	{
		t = delete_front(t);
	}
}

// Priority Scheduling Preemtive (Function)
void ps_p()
{
	struct node *clone_header = clone_LL(header_original);
	struct node *temp, *temp1, *temp2;
	int program_counter = 0;
	float average_wait = 0.0f;
	int number_of_process = process_counter(clone_header);
	bubble_sort(&clone_header, number_of_process, "AT");
	bubble_sort(&clone_header, number_of_process, "PS");
	temp = temp1 = temp2 = clone_header;

	while (!is_all_done(clone_header))
	{
		struct node *in_cpu_node = find_least_priority(clone_header, program_counter);
		bool is_found = true;

		if (in_cpu_node == NULL)
		{
			temp = clone_header;
			while (temp != NULL)
			{
				if (!temp->is_terminated)
				{
					if (temp->arrival_time > program_counter && is_found)
					{
						is_found = false;
						program_counter = temp->arrival_time;
						in_cpu_node = find_least_priority(clone_header, program_counter);
						in_cpu_node->how_much_left--;
						program_counter++;
						if (in_cpu_node->how_much_left == 0)
						{
							in_cpu_node->turnaround_time = program_counter;
							in_cpu_node->is_terminated = true;
						}
					}
				}

				temp = temp->next;
			}
		}

		else
		{
			program_counter++;
			in_cpu_node->how_much_left--;

			if (in_cpu_node->how_much_left == 0)
			{
				in_cpu_node->turnaround_time = program_counter;
				in_cpu_node->is_terminated = true;
			}
		}
	}

	bool is_first = true;
	while (temp1 != NULL)
	{
		if (is_first)
		{
			temp1->waiting_time = temp1->turnaround_time - temp1->burst_time;
			if (temp1->waiting_time < 0)
				temp1->waiting_time = 0;

			is_first = false;
		}

		else
		{
			temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time;
			if (temp1->waiting_time < 0)
				temp1->waiting_time = 0;
		}
		temp1 = temp1->next;
	}

	strcpy(buff, "");
	bubble_sort(&temp2, number_of_process, "PID");
	system("clear");
	strcat(buff, "Scheduling Method: Priority Scheduling (Preemtive)\n");
	strcat(buff, "Process Waiting Times:\n");
	while (temp2 != NULL)
	{
		int pid = temp2->process_id;
		int wait = temp2->waiting_time;
		average_wait += wait;
		char buff_1[20] = "";
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		temp2 = temp2->next;
	}
	average_wait /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
	strcat(buff, buff_2);
	if (ps_p_first)
	{
		strcat(buffer_output, buff);
		ps_p_first = false;
	}
	printf("%s", buff);
	while (clone_header != NULL)
	{
		clone_header = delete_front(clone_header);
	}
}

// Round-Robin Scheduling (Function)
void rr()
{
	struct node *clone_header = clone_LL(header_original);
	struct node *temp1, *temp2, *temp3;
	int program_counter = 0;
	float average_wait = 0.0f;
	int number_of_process = process_counter(clone_header);
	bool is_first = true;
	bool previous_ones_done = false;
	bubble_sort(&clone_header, number_of_process, "AT");
	temp1 = temp2 = temp3 = clone_header;

	while (temp3 != NULL)
	{
		temp3->time_slices = temp3->burst_time / time_quantum;
		temp3->last_slice_burst = temp3->burst_time % time_quantum;
		temp3 = temp3->next;
	}

	while (!is_all_done(clone_header))
	{
		temp1 = clone_header;
		is_first = true;
		while (temp1 != NULL)
		{
			if (!temp1->is_terminated)
			{
				if (temp1->arrival_time <= program_counter)
				{
					if (is_first)
					{
						if (temp1->time_slices == 0)
						{
							program_counter += temp1->last_slice_burst;
							if (temp1->last_slice_burst != 0)
								temp1->turnaround_time = program_counter;
							temp1->waiting_time = temp1->turnaround_time - temp1->burst_time;
							if (temp1->waiting_time < 0)
								temp1->waiting_time = 0;
							temp1->is_terminated = true;
						}
						else
						{
							program_counter += time_quantum;
							temp1->turnaround_time = program_counter;
							temp1->time_slices--;
						}
						is_first = false;
					}

					else
					{
						if (temp1->time_slices == 0)
						{
							program_counter += temp1->last_slice_burst;
							if (temp1->last_slice_burst != 0)
								temp1->turnaround_time = program_counter;
							temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time;
							if (temp1->waiting_time < 0)
								temp1->waiting_time = 0;
							temp1->is_terminated = true;
						}
						else
						{
							program_counter += time_quantum;
							temp1->turnaround_time = program_counter;
							temp1->time_slices--;
						}
					}
				}

				else
				{
					previous_ones_done = is_previous_ones_done(clone_header, program_counter);
					if (previous_ones_done)
					{
						program_counter = temp1->arrival_time;
						if (temp1->time_slices == 0)
						{
							program_counter += temp1->last_slice_burst;
							if (temp1->last_slice_burst != 0)
								temp1->turnaround_time = program_counter;
							temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time;
							if (temp1->waiting_time < 0)
								temp1->waiting_time = 0;
							temp1->is_terminated = true;
						}
						else
						{
							program_counter += time_quantum;
							temp1->turnaround_time = program_counter;
							temp1->time_slices--;
						}
					}
				}
			}

			is_first = false;
			temp1 = temp1->next;
		}
	}

	strcpy(buff, "");
	bubble_sort(&clone_header, number_of_process, "PID");
	temp2 = clone_header;
	system("clear");
	snprintf(buff, 999, "Scheduling Method: Round-Robin (Time quantum: %d)\n", time_quantum);
	strcat(buff, "Process Waiting Times:\n");
	while (temp2 != NULL)
	{
		int pid = temp2->process_id;
		int wait = temp2->waiting_time;
		average_wait += wait;
		char buff_1[20];
		snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
		strcat(buff, buff_1);
		temp2 = temp2->next;
	}
	average_wait /= number_of_process;
	char buff_2[40];
	snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
	strcat(buff, buff_2);
	if (rr_first)
	{
		strcat(buffer_output, buff);
		rr_first = false;
	}
	printf("%s", buff);
	while (clone_header != NULL)
	{
		clone_header = delete_front(clone_header);
	}
}
