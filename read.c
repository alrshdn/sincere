#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>    

#include "sincere.h"


typedef struct {
	int id;
	long long time_ms;
} Test;


/* Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}


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

	long long last_time_ms = 0;
	while (1) {
		if (data->time_ms > last_time_ms) {
			printf("s = %lld\n", data->time_ms);
			last_time_ms = data->time_ms;
		}
		msleep(20);
	}

	sincere_finalize(shm);

	return 0;
}
