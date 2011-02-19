struct process * process_input(char * buffer);

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
