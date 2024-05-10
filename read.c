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
		//.readonly = false,
		.unlink = false,
	};
	printf("Unlink: %d\n", config.unlink);
	shm = sincere_shared_memory_initialize_from_config(&config);
	
	Test *data = sincere_shared_memory_get(shm);
	printf("data->id = %d\n", data->id);
	while (1) {
		printf("s = %d\n", data->s);
		sleep(1);
	}

	sincere_finalize(shm);

	return 0;
}
