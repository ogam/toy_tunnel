#include "assets/assets.h"

char* mount_get_directory_path()
{
    //  mounting vfs
    char *path = cf_path_normalize(cf_fs_get_base_directory());
    char *dir = cf_path_directory_of(path);
    s32 directory_depth = 0;
    //  running from debugger
    if (cf_string_equ(dir, "/build"))
    {
        directory_depth = 2;
    }
    //  running from debug build
    else if (cf_string_equ(dir, "/Debug") || cf_string_equ(dir, "/Release"))
    {
        directory_depth = 2;
    }
    cf_string_free(dir);
    path = cf_path_pop_n(path, directory_depth);
    
    return path;
}

void mount_root_read_directory()
{
    char *path = mount_get_directory_path();
    cf_fs_mount(path, "/", false);
    cf_string_free(path);
}

void mount_root_write_directory()
{
    char *path = mount_get_directory_path();
    cf_fs_set_write_directory(path);
    cf_string_free(path);
}

void dismount_root_directory()
{
    char *path = mount_get_directory_path();
    cf_fs_dismount(path);
    cf_string_free(path);
}

void mount_data_read_directory()
{
    char *path = mount_get_directory_path();
    cf_string_append(path, "/data");
    cf_fs_mount(path, "/", false);
    cf_string_free(path);
}

void mount_data_write_directory()
{
    char *path = mount_get_directory_path();
    cf_string_append(path, "/data");
    cf_fs_set_write_directory(path);
    cf_string_free(path);
}

void dismount_data_directory()
{
    char *path = mount_get_directory_path();
    cf_string_append(path, "/data");
    cf_fs_dismount(path);
    cf_string_free(path);
}

void mount_shaders_read_directory()
{
    char *path = mount_get_directory_path();
    cf_string_append(path, "/data/shaders");
    cf_fs_mount(path, "/", false);
    cf_string_free(path);
}