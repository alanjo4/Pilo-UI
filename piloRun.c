#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void createDirectoryIfNotExists(const char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        mkdir(dir, 0700);
    }
}

int endsWith(const char *str, const char *suffix) {
    if (!str || !suffix) return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr) return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int main() {
    const char *componentsDir = "src/components";
    const char *storiesDir = "src/stories";

    createDirectoryIfNotExists(storiesDir);

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(componentsDir)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (endsWith(ent->d_name, ".pilo.jsx")) {
                char componentName[256];
                strcpy(componentName, ent->d_name);
                componentName[strlen(componentName) - 9] = '\0'; // Remove ".pilo.jsx"

                char storyFilePath[512];
                sprintf(storyFilePath, "%s/%s.js", storiesDir, componentName);

                FILE *f = fopen(storyFilePath, "w");
                if (f == NULL) {
                    perror("Error al abrir el archivo");
                    continue;
                }

                fprintf(f, "import React from 'react';\n");
                fprintf(f, "import %s from '../components/%s';\n", componentName, ent->d_name);
                fprintf(f, "import MySBForReact from '../components/MySBForReact.jsx';\n\n");
                fprintf(f, "const %sStory = () => (\n", componentName);
                fprintf(f, "\t<MySBForReact>\n");
                fprintf(f, "\t\t<%s />\n", componentName);
                fprintf(f, "\t</MySBForReact>\n");
                fprintf(f, ");\n\n");
                fprintf(f, "export default %sStory;\n", componentName);

                fclose(f);

                printf("Historia generada: %s\n", storyFilePath);
            }
        }
        closedir(dir);
    } else {
        perror("No se pudo abrir el directorio");
    }

    return 0;
}