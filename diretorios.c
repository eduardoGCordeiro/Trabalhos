//
// DIRETORIOS.C
//
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

static int pesquisa(const char *arg, const char *caminho){
    DIR *dirpont, *dirptraux;
    struct dirent *dp, *listDir;
    char *proximo;
    struct stat filestat;

    if ((dirpont = opendir(caminho)) == NULL) {
        (void) printf( "Nao pode abrir %s \n\n", caminho);
        return 0;
    }
    (void) printf("\nProcurando...");
    do {
        errno = 0;
        if ((dp = readdir(dirpont)) != NULL) {
            if (strcmp(dp->d_name, arg) == 0){
              (void) printf("\n\n'%s' encontrado \nCaminho: %s/%s\n", dp->d_name, caminho, dp->d_name);
              (void) printf("\nInformações: ");
              if((dp->d_type & DT_DIR) == DT_DIR)
                (void) printf("\nTipo: Pasta\n");
              else
                (void) printf("\nTipo: Arquivo\n");

              stat(dp->d_name, &filestat);
              (void) printf("Tamanho: %d bytes\n", filestat.st_size);
              (void) printf("Modo: %d\n", filestat.st_mode);
              (void) printf("Numero de blocos: %d\n", filestat.st_blocks);
              (void) printf("Tamanho do em disco: %d\n", filestat.st_blocks*512);
              (void) closedir(dirpont);
              return 1;
            }else{
              if(dp->d_type == 4 && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
                proximo = (char *)malloc(strlen(caminho)+strlen(dp->d_name)+2); //Caminho para proxima pasta
                strcat(proximo, caminho);
                strcat(proximo,"/");
                strcat(proximo,dp->d_name);
                if(pesquisa(arg, proximo) == 1){
                  return 1;
                }
              }
            }
        }
    } while (dp != NULL);

    if (errno != 0)
        perror("\nErro lendo o diretorio\n\n");
    else
        (void) printf("\nFalha ao procurar: %s\n", arg);
    (void) closedir(dirpont);
    return 0;
}

int main(int argc, char *argv[])
{
    int i;

    if( argc < 2 ) {
       printf( "USO: %s nome_arquivo ...\n\n", argv[ 0 ] );
       return( -1 );
    }
    for (i = 1; i < argc; i++){
        pesquisa(argv[i], ".");
    }
    return (0);
}
