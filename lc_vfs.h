#ifndef LC_LC_VFS_H
#define LC_LC_VFS_H

#ifndef LC_INCLUDE_H
#include "lc.h"
#endif

/// ::toc_begin
/// - Structures .................................................158. [LCV_XST]
/// - Implementation .............................................203. [LCV_XIM]
/// ::toc_end

////////////////////////////////////////////////////////////////////////////////
/// Structures                                                     [LCV_XST] ///
////////////////////////////////////////////////////////////////////////////////

enum LC_NODISCARD lc_vfs_error
{
	LC_VFS_ERROR_NONE,
	LC_VFS_ERROR_UNKNOWN,
	LC_VFS_ERROR_NOT_IMPLEMENTED,
	LC_VFS_ERROR_NOT_FOUND,
	LC_VFS_ERROR_ENCRYPTED,
	LC_VFS_ERROR_END_OF_DIR,
};

enum lc_vfs_cmd
{
	LC_VFS_CMD_OPEN,
	LC_VFS_CMD_CLOSE,

	LC_VFS_CMD_FILE_OPEN,
	LC_VFS_CMD_FILE_STAT,

	LC_VFS_CMD_CD,
	LC_VFS_CMD_CWD,

	LC_VFS_CMD_DIR_OPEN,
	LC_VFS_CMD_DIR_CREATE,
	LC_VFS_CMD_DIR_DELETE,
	LC_VFS_CMD_DIR_READ,
	LC_VFS_CMD_DIR_CLOSE,

	LC_VFS_CMD_ENC_SET_KEY,
};

enum lc_vfs_stat_mask
{
	LC_VFS_STAT_FILE_TYPE = 1 << 0,
	LC_VFS_STAT_NAME      = 1 << 1,
	LC_VFS_STAT_SIZE      = 1 << 2,
};

enum lc_vfs_file_type
{
	LC_VFS_FILE_TYPE_FILE,
	LC_VFS_FILE_TYPE_DIR,
};

struct lc_vfs_stat {
	enum lc_vfs_stat_mask field_mask;

	enum lc_vfs_file_type file_type;
	struct lc_sv name;
	size_t size;
};

struct lc_vfs_dir {
	void *data;
};

struct lc_vfs_entry {
	enum lc_vfs_file_type file_type;
	struct lc_sv name;
};

struct lc_vfs {
	enum lc_vfs_error (*proc)(LC_ARGS_DECL, void *restrict data, enum lc_vfs_cmd cmd, ...);
	void *restrict data;
};

static inline enum lc_vfs_error
lc_vfs_file_open_(LC_ARGS_DECL, struct lc_vfs fs, struct lc_sv filename, struct lc_io *file)
{
	return fs.proc(LC_ARGS_PASS, fs.data, LC_VFS_CMD_FILE_OPEN, filename, file);
}

static inline enum lc_vfs_error
lc_vfs_file_stat_(LC_ARGS_DECL, struct lc_vfs fs, struct lc_sv filename, struct lc_vfs_stat *stat)
{
	return fs.proc(LC_ARGS_PASS, fs.data, LC_VFS_CMD_FILE_STAT, filename, stat);
}

static inline enum lc_vfs_error
lc_vfs_cd_(LC_ARGS_DECL, struct lc_vfs fs, struct lc_sv dirname)
{
	return fs.proc(LC_ARGS_PASS, fs.data, LC_VFS_CMD_CD, dirname);
}

LC_NODISCARD
static inline struct lc_sv
lc_vfs_cwd_(LC_ARGS_DECL, struct lc_vfs fs)
{
	struct lc_sv cwd = sv(0);

	if (LC_VFS_ERROR_NONE != fs.proc(LC_ARGS_PASS, fs.data, LC_VFS_CMD_CWD, &cwd))
		return sv_c("/");

	return cwd;
}

LC_NODISCARD
static inline bool
lc_vfs_is_root_(LC_ARGS_DECL, struct lc_vfs fs)
{
	struct lc_sv cwd = lc_vfs_cwd_(LC_ARGS_PASS, fs);
	return 1 == cwd.length && '/' == cwd.s[0];
}


static inline enum lc_vfs_error
lc_vfs_dir_open_(LC_ARGS_DECL, struct lc_vfs fs, struct lc_sv dirname, struct lc_vfs_dir *dir)
{
	return fs.proc(LC_ARGS_PASS, fs.data, LC_VFS_CMD_DIR_OPEN, dirname, dir);
}


static inline enum lc_vfs_error
lc_vfs_dir_close_(LC_ARGS_DECL, struct lc_vfs fs, struct lc_vfs_dir dir)
{
	return fs.proc(LC_ARGS_PASS, fs.data, LC_VFS_CMD_DIR_CLOSE, dir);
}

static inline bool
lc_vfs_dir_read_(
        LC_ARGS_DECL,
        struct lc_vfs fs,
        struct lc_vfs_dir *dir,
        struct lc_vfs_entry *restrict file_entry,
        enum lc_vfs_error *restrict err
)
{
	return LC_VFS_ERROR_NONE == (*err = fs.proc(LC_ARGS_PASS, fs.data, LC_VFS_CMD_DIR_READ, dir, file_entry));
}

static inline enum lc_vfs_error
lc_vfs_close_(LC_ARGS_DECL, struct lc_vfs vfs)
{
	return vfs.proc(LC_ARGS_PASS, vfs.data, LC_VFS_CMD_CLOSE);
}

static inline enum lc_vfs_error
lc_vfs_encryption_key_set_(LC_ARGS_DECL, struct lc_vfs vfs, struct lc_sv password)
{
	return vfs.proc(LC_ARGS_PASS, vfs.data, LC_VFS_CMD_ENC_SET_KEY, password);
}




#define lc_vfs_file_stat(...) LC_ARGS_WARP(lc_vfs_file_stat_, __VA_ARGS__)
#define lc_vfs_cd(...) lc_vfs_cd_(LC_ARGS_CALL, __VA_ARGS__)
#define lc_vfs_cwd(fs) lc_vfs_cwd_(LC_ARGS_CALL, fs)
#define lc_vfs_is_root(fs) lc_vfs_is_root_(LC_ARGS_CALL, fs)
#define lc_vfs_dir_open(...) lc_vfs_dir_open_(LC_ARGS_CALL, __VA_ARGS__)
#define lc_vfs_file_open(...)  lc_vfs_file_open_(LC_ARGS_CALL, __VA_ARGS__)
#define lc_vfs_close(...) lc_vfs_close_(LC_ARGS_CALL, __VA_ARGS__)
#define lc_vfs_dir_read(...) lc_vfs_dir_read_(LC_ARGS_CALL, __VA_ARGS__)
#define lc_vfs_encryption_key_set(...) lc_vfs_encryption_key_set_(LC_ARGS_CALL, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////
/// Implementation                                                 [LCV_XIM] ///
////////////////////////////////////////////////////////////////////////////////

#include <stdarg.h>


#ifdef LC_PLATFORM_POSIX
#include <dirent.h>

extern enum lc_vfs_error
_lc_vfs_unistd_impl(LC_ARGS_DECL, void *restrict data, enum lc_vfs_cmd cmd, ...)
#ifndef LC_IMPLEMENTATION
	;
#else
{
	va_list args;
	va_start(args, cmd);

	lc_log_todo_once("buffer overflow check!");

	struct unistd_cwd {
		struct lc_allocator allocator;
		struct lc_sv view;
		char data[PATH_MAX];
	} *restrict p = data;

	switch (cmd) {
	case LC_VFS_CMD_OPEN:
	{
		struct lc_allocator allocator = va_arg(args, struct lc_allocator);
		struct unistd_cwd **p         = data;

		enum lc_allocator_error err;
		*p = lc_mem_alloc_(allocator, &err, sizeof **p);

		if (NULL == getcwd((*p)->data, sizeof (*p)->data))
			LC_ASSERT(false, "");

		LC_ASSERT('/' == (*p)->data[0], "This has to be absolute path");

		(*p)->allocator = allocator;
		(*p)->view      = sv_cstr((*p)->data);

		(*p)->data[(*p)->view.length] = 0;
		goto ret;
	}
	case LC_VFS_CMD_CLOSE:
	{
		enum lc_allocator_error err;
		struct lc_allocator allocator = p->allocator;
		lc_mem_free_sized_(allocator, &err, p, sizeof *p);

		goto ret;
	}
	case LC_VFS_CMD_FILE_OPEN:
	{
		struct lc_sv filename = va_arg(args, struct lc_sv);
		struct lc_io *io      = va_arg(args, struct lc_io *);

		char path[PATH_MAX];
		(void)lc_path_join(path, p->view, filename);

		*io = lc_io_from_stdfile(fopen(path, "rb"));

		goto ret;
	}
	case LC_VFS_CMD_FILE_STAT:
	{
		struct lc_sv filename = va_arg(args, typeof(filename));
		struct lc_vfs_stat *st   = va_arg(args, typeof(st));

		char path[PATH_MAX];

		(void)lc_path_join(path, p->view, filename);
		struct stat buf;
		if (0 != stat(path, &buf))
			goto unknown_err;

		*st = (struct lc_vfs_stat){
		        .name       = filename,
		        .size       = buf.st_size,
		        .file_type  = S_ISDIR(buf.st_mode) ? LC_VFS_FILE_TYPE_DIR : LC_VFS_FILE_TYPE_FILE,
		        .field_mask = ~0,
		};
		goto ret;
	}
	case LC_VFS_CMD_CD:
	{
		struct lc_sv filename = va_arg(args, typeof(filename));
		char path[PATH_MAX];

		const size_t len = lc_path_join(path, p->view, filename);
		lc_log_info("cd path: %.*s", (int)len, path);

		p->view = sv(len, memcpy(p->data, path, len));

		p->data[len] = 0;
		goto ret;
	}
	case LC_VFS_CMD_CWD:
	{
		struct lc_sv *cwd = va_arg(args, typeof(cwd));

		*cwd = p->view;
		goto ret;
	}
	case LC_VFS_CMD_DIR_OPEN:
	{
		struct lc_sv dirname = va_arg(args, typeof(dirname));
		struct lc_vfs_dir *dir  = va_arg(args, typeof(dir));

		char path[PATH_MAX];
		(void)lc_path_join(path, p->view, dirname);

		dir->data = opendir(path);
		if (nullptr == dir->data)
			goto unknown_err;

		goto ret;
	}
	case LC_VFS_CMD_DIR_CREATE: goto not_implemented;
	case LC_VFS_CMD_DIR_DELETE: goto not_implemented;
	case LC_VFS_CMD_DIR_READ:
	{
		struct lc_vfs_dir *dir     = va_arg(args, typeof(dir));
		struct lc_vfs_entry *entry = va_arg(args, typeof(entry));

		struct dirent *dir_entry = NULL;
		struct lc_sv name        = sv(0);

skip:
		errno = 0;
		dir_entry = readdir(dir->data);
		if (nullptr == dir_entry) {
			if (0 == errno)
				goto end_of_dir;

			goto unknown_err;
		}


		name = sv_cstr(dir_entry->d_name);
		if (0 == strncmp(".", name.s, name.length))
			goto skip;

		if (0 == strncmp("..", name.s, name.length))
			goto skip;

		bool is_dir = S_ISDIR(dir_entry->d_type << 12);


		*entry = (struct lc_vfs_entry){
		        .name      = name,
		        .file_type = is_dir ? LC_VFS_FILE_TYPE_DIR : LC_VFS_FILE_TYPE_FILE,
		};

		goto ret;
	}
	case LC_VFS_CMD_DIR_CLOSE:
	{
		struct lc_vfs_dir dir = va_arg(args, typeof(dir));
		if (0 != closedir(dir.data))
			goto unknown_err;

		goto ret;
	}
	case LC_VFS_CMD_ENC_SET_KEY: goto not_implemented;
	}


ret:
	va_end(args);
	return LC_VFS_ERROR_NONE;

not_implemented:
	va_end(args);
	return LC_VFS_ERROR_NOT_IMPLEMENTED;

unknown_err:
	lc_log_todo("FIXME: cmd = %d", cmd);

	va_end(args);
	return LC_VFS_ERROR_UNKNOWN;

end_of_dir:

	va_end(args);
	return LC_VFS_ERROR_END_OF_DIR;
}
#endif

static inline enum lc_vfs_error
lc_vfs_from_unistd(LC_ARGS_DECL, struct lc_vfs *vfs, struct lc_allocator allocator)
{
	return (vfs->proc = _lc_vfs_unistd_impl)(LC_ARGS_PASS, (void *)&vfs->data, LC_VFS_CMD_OPEN, allocator);
}




#endif

#endif
