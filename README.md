# Sincere
Sincere is a C library for facilitating inter-process communication (IPC).
It is written in pure ANSI C code in order to prioritize portability.

Sincere is still in very early development, and its API is subject to change at anytime. However, **features** presented below are fully functional.

## Features
Currently, Sincere is in its early development, and only implements a **POSIX shared memory** interface.

### Usage
A process can write to and read from a POSIX shared memory object.

- Writing:
```c
typedef struct {
	/* your data structure fields here */
	int field;
} Data;

int
main(void)
{
	/* initializing a POSIX shared memoey object with configuration */
	SincereHandle *shm = sincere_shared_memory_initialize(
		"/my-shared-memory",		/* name (must start with "/") */
		sizeof(Data),				/* size of memory allocation */
		false,						/* readonly mode */
		true,						/* create if it does not exist */
		false						/* unlink after finalizing */
		);

	Data data = {42};
	/* setting the whole data of the shared memory object to the input (must be of the same size) */
	sincere_shared_memory_set(shm, &data);

	/* finalizing (cleaning up and freeing) a memory object, unlinks it if configured to do so */
	sincere_finalize(shm);
}
```

- Reading:
```c
typedef struct {
	/* your data structure fields here */
	int field;
} Data;

int
main(void)
{
	/* initializing a POSIX shared memoey object with configuration */
	SincereHandle *shm = sincere_shared_memory_initialize(
		"/my-shared-memory",		/* name (must start with "/") */
		sizeof(Data),				/* size of memory allocation */
		true,						/* readonly mode */
		false,						/* create if it does not exist */
		true						/* unlink after finalizing */
		);

	/* accessing the data in the shared memory object via a direct pointer */
	Data *data = sincere_shared_memory_get(shm);  /* can access fields with `data->field` */

	/* finalizing (cleaning up and freeing) a memory object, unlinks it if configured to do so */
	sincere_finalize(shm);
}
```

Compile:
```bash
cc -lrt sincere.c your_source_code.c
```

