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
    int map_started = 0;
    
    while ((line = get_next_line(fd))) {
        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\0') {
            free(line);
            continue;
        }
        
        // Check if this is a configuration line (before map starts)
        if (!map_started && (line[0] == 'N' || line[0] == 'S' || line[0] == 'W' || 
                            line[0] == 'E' || line[0] == 'F' || line[0] == 'C')) {
            free(line);
            continue;
        }
        
        // Mark that map has started
        map_started = 1;
        
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
            if (!(c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ')) {
                write(2, "Error: invalid character '", 27);
                write(2, &c, 1);
                write(2, "' in map. Allowed: 0,1,N,S,E,W,space\n", 38);
                int k = 0;
                while(k<i){
                    free(game->map[k]);
                    k++;
                }
                free(game->map);
                k = 0;
                while (k < game->map_height) {
                    free(lines[k]);
                    k++;
                }
                free(lines);
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
        lines[i] = NULL;
        i++;
    }
    free(lines);
    return 1;
}

// Helper function to extract path from line
static char *extract_path(char *line, int start) {
    int i = start;
    
    // Skip spaces
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    
    int start_pos = i;
    // Find end of line (before newline)
    while (line[i] && line[i] != '\n')
        i++;
    
    int len = i - start_pos;
    if (len <= 0)
        return NULL;
    
    char *path = malloc(len + 1);
    if (!path)
        return NULL;
    
    int j = 0;
    while (j < len) {
        path[j] = line[start_pos + j];
        j++;
    }
    path[len] = '\0';
    
    // Remove trailing spaces
    while (len > 0 && (path[len - 1] == ' ' || path[len - 1] == '\t')) {
        path[len - 1] = '\0';
        len--;
    }
    
    return path;
}

// Helper function to parse RGB color
static int parse_rgb(char *line, int start) {
    int r = 0, g = 0, b = 0;
    int i = start;
    
    // Skip spaces
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    
    // Parse R
    while (line[i] >= '0' && line[i] <= '9') {
        r = r * 10 + (line[i] - '0');
        i++;
    }
    if (line[i] != ',') return -1;
    i++;
    
    // Parse G
    while (line[i] >= '0' && line[i] <= '9') {
        g = g * 10 + (line[i] - '0');
        i++;
    }
    if (line[i] != ',') return -1;
    i++;
    
    // Parse B
    while (line[i] >= '0' && line[i] <= '9') {
        b = b * 10 + (line[i] - '0');
        i++;
    }
    
    if (r > 255 || g > 255 || b > 255)
        return -1;
    
    return (r << 16) | (g << 8) | b;
}

void parse_config_data(t_game *game, const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return;
    
    // Initialize texture paths to NULL
    game->north_path = NULL;
    game->south_path = NULL;
    game->east_path = NULL;
    game->west_path = NULL;
    
    char *line;
    while ((line = get_next_line(fd))) {
        if (line[0] == '\n' || line[0] == '\0') {
            free(line);
            continue;
        }
        if (line[0] == '0' || line[0] == '1' || line[0] == ' ') {
            free(line);
            break;
        }
        
        if (line[0] == 'N' && line[1] == 'O') {
            game->north_path = extract_path(line, 2);
        }
        else if (line[0] == 'S' && line[1] == 'O') {
            game->south_path = extract_path(line, 2);
        }
        else if (line[0] == 'W' && line[1] == 'E') {
            game->west_path = extract_path(line, 2);
        }
        else if (line[0] == 'E' && line[1] == 'A') {
            game->east_path = extract_path(line, 2);
        }
        else if (line[0] == 'F') {
            game->floor_color = parse_rgb(line, 1);
        }
        else if (line[0] == 'C') {
            game->ceiling_color = parse_rgb(line, 1);
        }
        free(line);
    }
    close(fd);
}

void print_config_data(t_game *game) {
    printf("North texture: %s\n", game->north_path ? game->north_path : "NULL");
    printf("South texture: %s\n", game->south_path ? game->south_path : "NULL");
    printf("West texture: %s\n", game->west_path ? game->west_path : "NULL");
    printf("East texture: %s\n", game->east_path ? game->east_path : "NULL");
    
    if (game->floor_color >= 0) {
        printf("Floor color: %d,%d,%d\n", 
               (game->floor_color >> 16) & 0xFF,
               (game->floor_color >> 8) & 0xFF,
               game->floor_color & 0xFF);
    } else {
        printf("Floor color: NULL\n");
    }
    
    if (game->ceiling_color >= 0) {
        printf("Ceiling color: %d,%d,%d\n", 
               (game->ceiling_color >> 16) & 0xFF,
               (game->ceiling_color >> 8) & 0xFF,
               game->ceiling_color & 0xFF);
    } else {
        printf("Ceiling color: NULL\n");
    }
}
