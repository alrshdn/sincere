#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#include "sincere.h"

typedef struct {
	time_t s;
	int id;
} Test;

int
main(void)
{
	SincereHandle *shm;
	SincereSharedMemoryConfig config = {
		.name = "/shm-test-1",
		.size = sizeof(Test),
		.readonly = false,
		.create = true,
		.unlink = false
	};
	shm = sincere_shared_memory_initialize_from_config(&config);
	
	Test data = {
		.s = time(NULL),
		.id = 3
	};
	// Test *data = sincere_shared_memory_get(shm);
	//data->id += 2;

	sincere_shared_memory_set(shm, &data);

	sincere_finalize(shm);

	return 0;
}
