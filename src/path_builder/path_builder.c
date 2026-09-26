#include <stddef.h>
#include "path_builder.h"
#include "libft.h"

char *build_path(const char *directory_path, const char *child_path)
{
    if (!ft_is_valid_path(directory_path) || !ft_is_valid_path(child_path))
        return NULL;

    return ft_safe_strjoin(directory_path, child_path);
}
