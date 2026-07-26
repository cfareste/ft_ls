#include "libft.h"
#include "ft_printf.h"
#include "mocks.h"
#include "os_adapter.h"

struct dir_entry
{
    char d_name[256];
};

struct dir_stream
{
    t_dir_entry entry;
};

t_failure_type g_opendir_fail = NO_FAILURE;
t_failure_type g_readdir_fail = NO_FAILURE;
t_failure_type g_closedir_fail = NO_FAILURE;
static char g_dirent_name[256] = "";
static unsigned int g_dirent_entry_num = 0;
static unsigned int g_total_dirent_entries = 0;

static char *get_file_name()
{
    char *file_num = "";

    if (g_dirent_entry_num != 1)
        file_num = ft_uitoa(g_dirent_entry_num);

    char *file_name = ft_strjoin(g_dirent_name, file_num);

    if (g_dirent_entry_num != 1)
        free(file_num);

    return file_name;
}

t_dir_stream *opendir_adapter(const char *path)
{
    (void) path;
    if (g_opendir_fail != NO_FAILURE)
    {
        return NULL;
    }

    t_dir_stream *dir = ft_calloc(1, sizeof(t_dir_stream));

    return dir;
}

t_dir_entry *readdir_adapter(t_dir_stream *dir)
{
    if (g_readdir_fail != NO_FAILURE)
    {
        return NULL;
    }

    if (g_total_dirent_entries == 0 || g_dirent_entry_num > g_total_dirent_entries)
    {
        return NULL;
    }

    char *file_name = get_file_name();
    ft_strlcpy(dir->entry.d_name, file_name, sizeof(dir->entry.d_name));
    g_dirent_entry_num++;
    free(file_name);

    return &dir->entry;
}

int closedir_adapter(t_dir_stream **dir_stream)
{
    if (g_closedir_fail != NO_FAILURE)
    {
        return -1;
    }

    free(*dir_stream);
    return 0;
}


void dir_entry_destroy(t_dir_entry **dir_entry)
{
    (void) dir_entry;
}

const char *dir_entry_get_name(const t_dir_entry *dir_entry)
{
    return dir_entry->d_name;
}

short dir_entry_is_null(const t_dir_entry *dir_entry)
{
    return (short) (dir_entry == NULL);
}


//TODO: Change how the name file name is computed as it makes the tests fragile (we depend on fileX)
void affirm_readdir_will_return_N_files_named(const char *file_name, const unsigned int num)
{
    g_dirent_entry_num = 1;
    g_total_dirent_entries = num;
    ft_strlcpy(g_dirent_name, file_name, sizeof(g_dirent_name));
}

void affirm_readdir_will_return_a_file_named(const char *file_name)
{
    affirm_readdir_will_return_N_files_named(file_name, 1);
}

//TODO: Fix mocking coupling to this function; find another solution to not depend on calling this function whenever I need to reset readdir
void reset_readdir_affirmations()
{
    g_dirent_name[0] = '\0';
    g_dirent_entry_num = 0;
    g_total_dirent_entries = 0;
}
