struct process * process_input(char * buffer);
void exec_processes( struct process *p );
void do_child( struct process *p, int in_pipe[], int out_pipe[] );

enum IO_REDIRECT
{
	NONE,
	INPUT,
	OUTPUT,
	INPUT_AND_OUTPUT
};

struct process
{
	struct process * prev_process;
	struct process * next_process;
	char * name;
	enum IO_REDIRECT redirect_state;
	int is_file;
	int num_args;
	char ** args;
};

int MAX_CHARS = 5000;
