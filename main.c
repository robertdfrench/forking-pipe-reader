#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int worker(int altout) {
	/* Change this process's stdout to "altout" (the write end of our pipe)
	 * so that we can use printf as normal but have it flow over the pipe.
	 */
	dup2(altout, STDOUT_FILENO);
	close(altout);

	/* Print some text so we can see how it is captured by the parent */
	printf("Write programs that do one thing and do it well.");
	printf("Write programs that work together.");
	printf("Write programs that manipulate text streams, ");
	printf("for that is a universal interface.");

	/* Guarantee that any buffered output has been written to our pipe.*/
	fflush(stdout);
	close(STDOUT_FILENO); // Send EOF
	_exit(0);
	return 0; // This is never reached, just here to make the compiler happy
}

int main() {
	/* Create a pipe to allow the worker to send text to the parent */
	int p[2];
	pipe(p);
	/* p[1] is the "write end" of the pipe. Anything written to it will be
	 * available on p[0], the "read end". Our goal is to make p[1] available
	 * in the child process, and p[0] available in the parent, so that we
	 * have a one-way communication pattern. */

	pid_t worker_pid = fork();
	if (worker_pid == 0) return worker(p[1]);
	/* At this point, the parent will continue executing the remainder of
	 * 'main()' but the child will have moved on to the 'worker()' function.
	 */

	close(p[1]); /* The parent will not write to itself, so we must close
			the write end of the pipe. Failure to do so will cause
			'read()' to hang after the child is done, because the
			operating system is still waiting on the parent's copy
			of the write end of the pipe */

	const char* header = "0123456789ABCDEF0123456789ABCDE";
	size_t buffer_size = strlen(header);
	char body[buffer_size + 1];
	memset(body, 0, buffer_size + 1); // Start off with the empty string.

	printf("   [%s]\n", header);
	int num_reads = 0;
	for(size_t n = 1; n > 0;) {
		n = read(p[0], body, buffer_size);
		memset(body + n, 0, buffer_size + 1 - n);
		/* These two lines ^ copy as many bytes as will fit into 'body'
		 * and then write zeros over the remainder (if any space
		 * remains). This ensures that we don't have any residual data
		 * from previous reads, since we are reusing the same 'body'
		 * buffer each time. */
		num_reads++;
		printf("%2d [%s]\n", num_reads, body);
	}

	int status;
	waitpid(worker_pid, &status, 0);
	return status;
}
