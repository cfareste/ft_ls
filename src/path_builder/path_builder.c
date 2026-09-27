#include <stddef.h>
#include "path_builder.h"
#include "libft.h"

#define CURRENT_DIRECTORY "."

char *build_full_path(const char *directory_path, const char *child_path)
{
    if (!ft_is_valid_path(directory_path) || !ft_is_valid_path(child_path))
        return NULL;

    const unsigned int directory_path_size = ft_strlen(directory_path);
    const unsigned int child_path_size = ft_strlen(child_path);
    const unsigned int needs_slash = directory_path[directory_path_size - 1] != '/';
    const unsigned int new_path_size = directory_path_size + child_path_size + needs_slash;
    char *new_path = ft_safe_calloc(new_path_size + 1, sizeof(char));

    ft_strlcat(new_path, directory_path, new_path_size + 1);
    if (needs_slash)
        ft_strlcat(new_path, "/", new_path_size + 1);
    ft_strlcat(new_path, child_path, new_path_size + 1);

    return new_path;
}

char *build_path(const char *directory_path, const char *child_path)
{
    if (!ft_is_valid_path(directory_path) || !ft_is_valid_path(child_path))
        return NULL;

    if (ft_are_string_equals(directory_path, CURRENT_DIRECTORY))
        return ft_safe_strdup(child_path);

    const unsigned int directory_path_size = ft_strlen(directory_path);
    const unsigned int child_path_size = ft_strlen(child_path);
    const unsigned int needs_slash = directory_path[directory_path_size - 1] != '/';
    const unsigned int new_path_size = directory_path_size + child_path_size + needs_slash;
    char *new_path = ft_safe_calloc(new_path_size + 1, sizeof(char));

    ft_strlcat(new_path, directory_path, new_path_size + 1);
    if (needs_slash)
        ft_strlcat(new_path, "/", new_path_size + 1);
    ft_strlcat(new_path, child_path, new_path_size + 1);

    return new_path;
}
