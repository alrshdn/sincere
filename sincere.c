#include "sincere.h"


void
sincere_handle_initialize(SincereHandle *handle, SincereHandleType type)
{
	handle->type = type;
	handle->data = NULL;
}


SincereHandle *
sincere_shared_memory_initialize(const char *name, off_t size, bool readonly, bool create, bool unlink)
{
	SincereHandle *handle;
	int oflag;
	mode_t mode;
	int prot;
	int fd;
	void *ptr;

	/* permissions */
	if (readonly) {
		oflag = O_RDONLY;
		prot = PROT_READ;
	} else {
		oflag = O_RDWR;
		prot = PROT_READ | PROT_WRITE;
	}

	/* behavior */
	if (create) {
		oflag |= O_CREAT;
		/* TODO(alrshdn): configure `mode` from `config` */
		/*	   (user)              | (group)   | (others) */
		mode = (S_IRUSR | S_IWUSR) | (S_IRGRP) | (S_IROTH);
	} else {
		mode = 0;
	}
	
	/* opening a shared memory object, and returning a file descriptor */
	fd = shm_open(
			name,
			oflag,
			mode
			);
	if (fd <= 0) {
		/* TODO(alrshdn): Handle error. */
		printf("[ERROR]: Failed to open shared memory object!\n");
		exit(EXIT_FAILURE);
	}

	/* truncating the shared memory object to the size/length sepcified */
	if (!readonly && ftruncate(fd, size) == -1) {
		/* TODO(alrshdn): Handle error. */
		printf("[ERROR]: Failed to truncate shared memory object to the size/length specified (size = %d).\n", size);
		exit(EXIT_FAILURE);
	}

	/* handle allocation and initialization */
	handle = malloc(sizeof *handle);
	sincere_handle_initialize(handle, S_HANDLE_TYPE_SHARED_MEMORY_POSIX);

	/* mapping shared memory object */
	/* TODO(alrshdn): add support for `MAP_ANONYMOUS` if possible/relevant */
	ptr = mmap(
			NULL,				/* address hint */
			size,				/* size/length of mapping  */
			prot,				/* memory protection flag */
			MAP_SHARED,			/* updates visibility flag */
			fd,					/* file descriptor */
			0					/* offset */
			);
	if (ptr == MAP_FAILED) {
		/* TODO(alrshdn): Handle error. */
		printf("[ERROR]: Mapping shared memory object failed!\n");
		exit(EXIT_FAILURE);
	}

	/* setting the handle fields */
	handle->name = name;
	handle->size = size;
	handle->readonly = readonly;
	handle->unlink = unlink;
	handle->data = ptr;

	/* file descriptor is not needed anymore, and can be closed */
	close(fd);

	return handle;
}

SincereHandle *
sincere_shared_memory_initialize_from_config(SincereSharedMemoryConfig *config)
{
	return sincere_shared_memory_initialize(
			config->name,
			config->size,
			config->readonly,
			config->create,
			config->unlink
			);
}

void
sincere_finalize(SincereHandle *handle)
{
	if (handle->type == S_HANDLE_TYPE_SHARED_MEMORY_POSIX) {
		/* unmapping */
		munmap(handle->data, handle->size);

		/* unlinking if configured to do so */
		if (handle->unlink) shm_unlink(handle->name);
	}

	/* freeing the handle */
	free(handle);
}

void *
sincere_shared_memory_get(SincereHandle* handle)
{
	return handle->data;
}

void
sincere_shared_memory_set(SincereHandle* handle, void *data)
{
	if (!handle->readonly) {
		memcpy(handle->data, data, handle->size);
	} else {
		/* TODO(alrshdn): Handle error. */
		printf("[ERROR]: Failed to set shared memory data! Reason: read-only memory.\n");
		exit(EXIT_FAILURE);
	}
}
