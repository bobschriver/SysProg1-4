#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "mysh.h"

int main(char** argv , int argc)
{

	char buffer[MAX_CHARS];

	while(fgets(buffer , MAX_CHARS , stdin))
	{
		struct process * proc = process_input(buffer);

		printf("%d\n" , proc);

		struct process * exec = proc;

		while(exec->next_process != NULL && !exec->next_process->is_file)
		{
			printf("%s\n" , exec->name);
			
			for(int i = 0; i < exec->num_args; i++)
			{
				printf("Arg #%d: %s\n" , i , exec->args[i]);
			}

			exec = exec->next_process;
		}

		printf("%s\n" , exec->name);

		exec_processes( exec );
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

		while(arg != NULL && strcmp(arg , "|") != 0 && strcmp(arg , ">") != 0 && strcmp(arg , "<") != 0)
		{
			args[num_args] = arg;

			num_args++;
			args = realloc(args , sizeof(char *) * (num_args + 1));

			arg = strtok(NULL , " ");
		}

		printf("After args\n");

		for(int i = 0; i < num_args; i ++)
		{
			printf("%d: %s\n" , i , args[i]);
		}

		//If our parent is a program, and we've already got arguements, just skip this
		if(curr_parent->num_args == 0)
		{

			//If we don't have any arguements, deallocate the list and set it to null
			if(num_args == 0)
			{
				free(args);
				args = NULL;
			}
			
			curr_parent->num_args = num_args;
			curr_parent->args = args;
		}

		for(int i = 0; i < curr_parent->num_args; i++)
		{
			printf("%d: %s\n" , i , curr_parent->args[i]);
		}
		
		if(arg != NULL)
		{
			struct process * next_process = malloc(sizeof(struct process));

			if(strcmp(arg , "<") == 0)
			{
				struct process * input_file = malloc(sizeof(struct process));
				input_file->is_file = 1;
				
				char * name = strtok(NULL , " ");
				printf("Input name %s\n" , name);
				
				input_file->name = name;

				curr_parent->prev_process = input_file;

				curr_parent->redirect_state = INPUT;
			}
			else if(strcmp(arg , ">") == 0)
			{
				struct process * output_file = malloc(sizeof(struct process));
				output_file->is_file = 1;

				char * name = strtok(NULL , " ");
				printf("Output name %s\n" , name);

				output_file->name = name;

				curr_parent->next_process = output_file;

				if(curr_parent->redirect_state == INPUT)
				{
					curr_parent->redirect_state = INPUT_AND_OUTPUT;
				}
				else
				{
					curr_parent->redirect_state = OUTPUT;
				}
			}
			else if(strcmp(arg, "|") == 0)
			{
				struct process * next_process = malloc(sizeof(struct process));

				next_process->is_file = 0;

				char * name = strtok(NULL , " ");
				printf("Output processes %s\n" , name);

				next_process->name = name;

				curr_parent->next_process = next_process;
				next_process->prev_process = curr_parent;

				if(curr_parent->redirect_state == INPUT)
				{
					curr_parent->redirect_state = INPUT_AND_OUTPUT;
				}
				else
				{
					curr_parent->redirect_state = OUTPUT;
				}

				next_process->redirect_state = INPUT;

				curr_parent = next_process;
			}
		}
		else
		{
			break;
		}

		printf("Curr Parent %s\n" , curr_parent->name);
	 	
	}

	 return first_process;
}

void exec_processes( struct process *p )
{
	struct process *proc = p;
	int in_pipe[2];
	int out_pipe[2];

	// Find our final process
	while( proc->next_process != NULL && !proc->next_process->is_file )
	{
		proc = proc->next_process;
	}

	// For each process, fork and create any necessary pipes
	while( proc != NULL && !proc->is_file )
	{

		//the old in-pipe is the new out-pipe
		out_pipe[0] = in_pipe[0];
		out_pipe[1] = in_pipe[1];

		if( pipe( in_pipe ) < 0 )
		{
			perror( "PIPE" );
			exit( EXIT_FAILURE );
		}

		switch( fork() )
		{
			case -1:
				perror( "FORK" );
				exit( EXIT_FAILURE );
		
			case 0:
				do_child( proc, in_pipe, out_pipe );
				/* NOTREACHED */
		}

		proc = proc->prev_process;
	}

}

void do_child( struct process *p, int in_pipe[], int out_pipe[] )
{
	//remap FDs if necessary
	if( p->prev_process != NULL && !p->prev_process->is_file )
	{
		close( 0 );
		dup( in_pipe[ 0 ] );
		close( in_pipe[ 0 ] );
	}

	if( p->next_process != NULL && !p->next_process->is_file )
	{
		close( 1 );
		dup( out_pipe[ 1 ] );
		close( out_pipe[ 1 ] );
	}

	execvp( p->name, p->args );
	perror( "EXEC" );
	_exit( EXIT_FAILURE );
}











