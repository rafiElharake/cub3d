#include "cub3d.h"

void error_exit(const char *msg) {
    write(2, msg, strlen(msg));
    exit(1);
}

void free_map(char **map, int height) {
    if (!map) return;
    int i = 0;
    while (i < height) {
        free(map[i]);
        i++;
    }
    free(map);
}

int load_map(const char *filename, t_game *game) {
    int fd = -1;
    game->map_height = 0;
    game->map_width = 0;
    int len = 0;
    while (filename[len]) len++;
    if (len < 4 || !(filename[len-4] == '.' && filename[len-3] == 'c' && filename[len-2] == 'u' && filename[len-1] == 'b')) {
        write(2, "Error: scene file must have .cub extension\n", 40);
        return 0;
    }
    fd = open(filename, 0);
    if (fd < 0) {
        perror("Error opening map file");
        return 0;
    }
    char **lines = NULL;
    int cap = 16;
    lines = malloc(sizeof(char*) * cap);
    if (!lines) { close(fd); return 0; }
    char *line;
    int maxw = 0;
    while ((line = get_next_line(fd))) {
        int l = 0; while (line[l]) l++;
        if (l > maxw) maxw = l;
        if (game->map_height + 1 > cap) {
            cap *= 2;
            char **tmp = malloc(sizeof(char*) * cap);
            if (!tmp) { free(line); break; }
            int i = 0;
            while (i < game->map_height) {
                tmp[i] = lines[i];
                i++;
            }
            free(lines);
            lines = tmp;
        }
        lines[game->map_height++] = line;
    }
    close(fd);
    if (game->map_height == 0) {
        write(2, "Error: empty map file\n", 23);
        free(lines);
        return 0;
    }
    game->map_width = maxw;
    game->map = malloc(sizeof(char*) * game->map_height);
    if (!game->map) { int i = 0; while (i < game->map_height) { free(lines[i]); i++; } free(lines); return 0; }
    int i = 0;
    while (i < game->map_height) {
        char *src = lines[i];
        int l = 0;
        while (src[l])
            l++;
        if (l > 0 && src[l-1] == '\n')
            l--;
        int j = 0;
        while (j < l) {
            char c = src[j];
            if (!(c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W')) {
                write(2, "Error: invalid character '", 27);
                write(2, &c, 1);
                write(2, "' in map. Allowed: 0,1,N,S,E,W\n", 32);
                int k = 0;
                while (k < game->map_height) {
                    free(lines[k]);
                    k++;
                }
                free(lines);
                free(game->map);
                return 0;
            }
            j++;
        }
        char *dst = malloc(game->map_width + 1);
        j = 0;
        while (j < game->map_width) {
            dst[j] = '1';
            j++;
        }
        j = 0;
        while (j < l) {
            dst[j] = src[j];
            j++;
        }
        dst[game->map_width] = 0;
        game->map[i] = dst;
        free(src);
        i++;
    }
    free(lines);
    return 1;
}
