#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#include "sincere.h"


typedef struct {
	int id;
	long long time_ms;
} Test;

/* Get current time in milliseconds. */
long long get_time_ms(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}


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
		.id = 3,
		.time_ms = get_time_ms()
	};
	// Test *data = sincere_shared_memory_get(shm);
	//data->id += 2;

	sincere_shared_memory_set(shm, &data);

	sincere_finalize(shm);

	return 0;
}
