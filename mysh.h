
struct process * process_input(char * buffer);

struct process
{
	struct process * prev_process;
	struct process * next_process;
	char * process_name;
	enum io_redirect redirect_state;
	int is_file;
	int num_args;
	char ** args;
}

enum io_redirect
{
	NONE,
	INPUT,
	OUTPUT,
	INPUT_AND_OUTPUT
};

