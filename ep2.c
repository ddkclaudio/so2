/** EP2 -- Mac 0422 -- Sistemas Operacionais
 * 8030353 -- André Luiz Abdalla Silveira
 * 7158472 -- Cláudio Fenandes da Silva Filho
*/

// https://www.youtube.com/watch?v=QLOY6s53-5k

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define DELTA_TIME 60
#define DELTA_TIME_TUNING 20

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;
int track, cics, laps;

typedef struct cyc {
    int id;
    int veloc; 
    int lap;
    int position ;
    int track ;
    int broken;
    int points;
    pthread_t c_thread;
} cyclist;

int ** pista;
cyclist* ciclistas;

int is_multiple_of(int number,int base);
int rand_velocity(int last_velocity);
cyclist* create_cyclist(int cics);

// AUMENTA VELOCIDADE PARA 90?
int has_overtaking(int cyclist_index){
    return TRUE;
}

// POSSIBILIDADE DE QUEBRAR
int in_breaking(int cyclist_index){
    return TRUE;
}


void *Cyc_Thread(void *args){
    cyclist cyc = *((cyclist*) args);
    printf("%d\n", cyc.id);
    while(cyc.lap <= laps){
        pthread_mutex_lock(&mutex);

        

        pthread_mutex_unlock(&mutex);
    }
    pthread_barrier_wait(&barrier);
}

// Interfaces
void err_check (int track, int ciclistas, int voltas);
int ** criar_pista(int track);
void liberar_pista(int** pista,int track);

int main (int argc, const char * argv) {
    int r; // ative essa linha para 3 warnings a menos no  compilador
    r = scanf("%d", &track); // tamanho da pista
    r = scanf("%d", &cics);  // qtde de ciclistas
    r = scanf("%d", &laps);  // qtde de voltas
    
    pista       = criar_pista(track);
    r           = pthread_barrier_init(&barrier, NULL, cics);
    ciclistas   = create_cyclist(cics);

    for (r = 0; r < cics; r++)
        pthread_join(ciclistas[r].c_thread, NULL);

    // ================= [ END ] ===============
    err_check(track, cics, laps);
    liberar_pista(pista,track);
    return 0;
}


void err_check (int track, int ciclistas, int voltas){
    if (track <= 249) {
        //printf("%d", track);
        fprintf(stderr, "A pista deve ter pelo menos 250m\n");
        exit(EXIT_FAILURE);
    }
    if (ciclistas <= 5 || ciclistas > (track * 5)){
        //printf("%d", ciclistas);
        fprintf(stderr, "A qtde de ciclistas fora do intervalo (5 ; 5×track]\n");
        exit(EXIT_FAILURE);
    }
    if (voltas % 20 != 0) {
        //printf("%d", voltas);
        fprintf(stderr, "A qtde de voltas deve ser multipla de 20\n");
        exit(EXIT_FAILURE);
    }
    printf("Dados validados\n");
}

int ** criar_pista(int track){
    int cont = 0;
    int zero = 0;

    int ** pista = malloc(track * sizeof(int *));
    if (pista == NULL) {
        fprintf(stderr, "Erro na alocação da pista\n");
        exit(EXIT_FAILURE);        
    }

    for (cont = 0; cont < track; cont ++){
        pista[cont] = malloc (10 * sizeof(int));
        if (pista[cont] == NULL) {
            fprintf(stderr, "Erro na alocação da pista[%d]\n", cont);
            exit(EXIT_FAILURE);        
        }
        for (zero = 0; zero < 10;  zero ++)
            pista[cont][zero] = 0;
    }

    return pista;
}

cyclist* create_cyclist(int cics){
    int rc,i;
    cyclist* list_cyc = malloc(cics * sizeof(cyclist));

    for(i=0; i< cics;i++){
        pista[i/10][i%10] = i;
        list_cyc[i].id = i;
        list_cyc[i].veloc = 30;
        list_cyc[i].lap = 1;
        list_cyc[i].position = i/10;
        list_cyc[i].track = i%10;
        list_cyc[i].broken = 0;
        list_cyc[i].points = 0;
        rc = pthread_create(&list_cyc[i].c_thread, NULL, Cyc_Thread, (void *) &list_cyc[i]);
    }

    if (list_cyc == NULL) {
        fprintf(stderr, "Erro na alocação dos ciclistas\n");
        exit(EXIT_FAILURE);        
    }
    return list_cyc;
}

void liberar_pista(int** pista,int track){
    int cont;
    for (cont = 0; cont < track; cont ++)
        free(pista[cont]);
    free(pista);
}

int rand_velocity(int last_velocity){
    int percent;

    if (last_velocity == 30)
        percent = 2;
    else
        percent = 4;

    if (rand() % 10 > percent)
        return 60;
    else
        return 30;
}

int is_multiple_of(int number,int base){
    if (number % base == 0)
        return TRUE;
    else
        return FALSE;
}