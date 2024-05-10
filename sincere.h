#ifndef SINCERE_H
#define SINCERE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>  /* for file access modes (O_* flags) */
#include <sys/stat.h>  /* for symbolic constants for `shm_open`'s `mode` */
#include <sys/mman.h>

/* TODO(alrshdn): add explicit `typedef`s for `real32`, `real64`, `int64`, and `uint64` */
/* TODO(alrshdn): move these common `typedef`s to their own library with other common code */
typedef signed char		int8;
typedef unsigned char	uint8;
typedef signed short	int16;
typedef unsigned short	uint16;
typedef signed int		int32;
typedef unsigned int	uint32;

/* TODO(alrshdn): check all bool cases ("stdbool.h" availability, C++, etc.) */
#if !defined(bool)
	typedef enum {
		false = 0,
		true = !false
	} bool;
#endif


/* ---------- Handle ---------- */
/* enums */
typedef enum {
	S_HANDLE_TYPE_UNASSIGNED = 0,
	S_HANDLE_TYPE_SHARED_MEMORY_POSIX
} SincereHandleType;

/* structs */
typedef struct {
	SincereHandleType type;
	const char *name;
	off_t size;

	/* POSIX shared memory */
	bool readonly;
	bool unlink;

	void *data;
} SincereHandle;

/* management functions */
static void sincere_handle_initialize(SincereHandle* handle, SincereHandleType type);  /* configures a handle into a preset configuration based on the handle's type */
void sincere_finalize(SincereHandle* handle);  /* cleans up and frees the memory related/handled by a handle */


/* ---------- POSIX Shared Memory ---------- */
/* structs */
typedef struct {
	const char *name;
	off_t		size;
	bool		readonly;
	bool		create;
	bool		unlink;
	/* unimplemented:
	mode_t		user_mode;
	mode_t		group_mode;
	mode_t		others_mode;
	*/
} SincereSharedMemoryConfig;

/* management functions */
SincereHandle *sincere_shared_memory_initialize(const char *name, off_t size, bool readonly, bool create, bool unlink);
SincereHandle *sincere_shared_memory_initialize_from_config(SincereSharedMemoryConfig *config);
/* usage/utility functions */
void *sincere_shared_memory_get(SincereHandle* handle);
void  sincere_shared_memory_set(SincereHandle* handle, void *data);

#endif
