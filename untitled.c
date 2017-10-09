/** EP2 -- Mac 0422 -- Sistemas Operacionais
 * 8030353 -- André Luiz Abdalla Silveira
 * 7158472 -- Cláudio Fenandes da Silva Filho
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

#define DELTA_TIME 60
#define DELTA_TIME_TUNING 20


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

typedef struct cyc {
    int id;
    int veloc;
    int lap;
    int position; //posicao na pista
    int track; //pista
    int broken; // sugestão minha colocar como bool, pode mudar se quiser
    int points;
    pthread_t c_thread;
} cyclist;
int ** pista;
cyclist* ciclistas;


int is_multiple_of(int number,int base);
int rand_velocity(int last_velocity);
cyclist* create_cyclist(int cics);
void relatorio();
void err_check (int track, int ciclistas, int voltas);
int ** criar_pista(int track);
void liberar_pista(int** pista,int track);
//cyclist procura_ciclista (int id);

// AUMENTA VELOCIDADE PARA 90?
int has_overtaking(int cyclist_index){
    return TRUE;
}

// POSSIBILIDADE DE QUEBRAR
int in_breaking(int cyclist_index){
    return TRUE;
}


void *Cyclist_Thread(void *args) {
    // código do ciclista
    int qual = (int)args, teste = 0;
    cyclist runner = ciclistas[qual]; 

    while(teste < 10){
        printf("Ciclista %d na posição[%d][%d]\n", runner.id, runner.position, runner.track);
        pthread_mutex_lock(&mutex);
        // sessão fucking critica
        pthread_mutex_unlock(&mutex);
        teste ++;
    }

    pthread_barrier_wait(&barrier); // espera as threads acabarem e acaba o role td 
}

//cyclist procura_ciclista (int id){
//    int i = 0
//}


// ========================================
// ========================================
// ========================================

int main (int argc, const char * argv) {
    int track, cics, laps;
    int r; // ative essa linha para 3 warnings a menos no  compilador
    r = scanf("%d", &track); // tamanho da pista
    r = scanf("%d", &cics);  // qtde de ciclistas
    r = scanf("%d", &laps);  // qtde de voltas

    // CRIANDO PISTA E CICLISTAS
    pista = criar_pista(track);
    r = pthread_barrier_init(&barrier, NULL, cics);
    ciclistas = create_cyclist(cics);

    // ESPERA OS CICLISTAS CHEGAREM E DEPOIS DESTROI AS THREADS
    //for (r = 0; r < cics; r++) comentei pq tava dando ruim
        //pthread_join(ciclistas[r].c_thread, NULL);
    
    relatorio();

    // ================= [ END ] ===============
    err_check(track, cics, laps);
    liberar_pista(pista,track);
    return 0;
}

void relatorio(){

    printf("Relatorio\n");
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
    int i,has_error;
    cyclist * aux;

    cyclist * ciclista = malloc(cics * sizeof(cyclist *));
    if (ciclista == NULL) {
        fprintf(stderr, "Erro na alocação dos ciclistas\n");
        exit(EXIT_FAILURE);        
    }

    for (i = 0; i < cics; i++) {
        pos = i / 10;
        tr = i % 10;
        aux = malloc((sizeof(cyclist)));
        aux.id = i;
        aux.veloc = 30;
        aux.lap = 1;
        aux.position = pos;
        aux.track = tr;
        pista[pos][tr] = i;
        aux.broken = 0;
        aux.points = 0;
        has_error = pthread_create(&aux.c_thread, NULL, Cyclist_Thread, (void *)i);
        
        if (has_error) {
            printf("ERROR; return code from pthread_create() is %d\n", has_error);
            exit(-1);
        }
        ciclista[i] = aux;
    }
    return ciclista;
}

void liberar_pista(int** pista,int track){
    int cont;
    for (cont = 0; cont < track; cont ++)
        free(pista[cont]);
    free(pista);
}

int rand_velocity(int last_velocity){
    int tmp = rand() % 10;
    int percent;

    if (last_velocity == 30)
        percent = 2;
    else
        percent = 4;

    if (tmp > percent)
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