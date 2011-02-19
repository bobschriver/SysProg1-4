
int main(char** argv , int argc)
{
	char buffer[MAX_COMMANDS];

	while(fgets(buffer , MAX_COMMANDS , stdin))
	{
		struct process * proc = process_input(buffer);

		struct process * exec = proc;

		while(exec->next_process != NULL && exec->next_process->is_file)
		{
			exec = exec->next_process;
		}
	}
}

struct process * process_input(char * buffer)
{
	struct process * first_process = malloc(sizeof(struct process));
	struct process * curr_parent = first_process;

	char * program_name = strtok(buffer  , " ");
	curr_parent->name = program_name;
	curr_parent->num_args = 0;


	while(program_name != NULL)
	{
		//malloc the arguement list
		char ** args = malloc(sizeof(char*));

		//Read in the first arguement
		char * arg = strtok(NULL , " ");

		int num_args = 0;

		while(arg != NULL
			|| strcmp(arg , "|") != 0 || strcmp(arg , ">") != 0 || strcmp(arg , "<") != 0)
		{
			args[num_args] = arg;

			num_args++;
			realloc(args , sizeof(char *) * (num_args + 1));

			arg = strtok(NULL , " ");
		}

		//If our parent is a program, and we've already got arguements, just skip this
		if(curr->num_args == 0)
		{

			//If we don't have any arguements, deallocate the list and set it to null
			if(num_args == 0)
			{
				dealloc(args);
				args = NULL;
			}

			curr_parent->num_args = num_args;
			curr_parent->args = args;
		}
		
		if(arg != NULL)
		{
			struct process * next_process = malloc(sizeof(struct process));

			if(strcmp(arg , "<") == 0)
			{
				struct process * input_file = malloc(sizeof(struct process));
				input_file->is_file = 1;
				
				char * name = strtok(NULL , " ");
				input_file->name = name;

				curr_parent->prev_process = input_file;

				curr_parent->redirect_state = INPUT;
			}
			else if(strcmp(arg , ">") == 0)
			{
				struct process * output_file = malloc(sizeof(struct process));
				output_file->is_file = 1;

				char * name = strtok(NULL , " ");
				input_file->name = name;

				curr_parent->next_process = output_file;

				if(curr_parent->redirect_state == INPUT)
				{
					curr_parent->redirect_state = INPUT_AND_OUTPUT;
				}
				else
				{
					curr_parent->redirect_state = OUTPUT
				}
			}
			else if(strcmp(arg "|") == 0)
			{
				struct process * next_process = malloc(sizeof(struct patron));

				next_process->is_file = 0;

				char * name = strtok(NULL , " ");

				next_process->name = name;

				curr_parent->next_process = next_process;
				next_process->prev_process = curr_parent;

				if(curr_parent->redirect_state == INPUT)
				{
					curr_parent->io_redirect = INPUT_AND_OUTPUT;
				}
				else
				{
					curr_parent->io_redirect = OUTPUT:
				}

				next_process->redirect_state = INPUT;

				curr_parent = next_process;
			}
		}
	 }

	 return first_process;
}
