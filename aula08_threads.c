
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int SIZE=5000;
const int NTHREAD=2;

struct vetor_t { //define o vetor
    double *dados; //ponteiro significa dizer que tem o endereço de uma estrutura de memória que está no heap
    int length;
};

struct work_t {
    struct vetor_t *v;  //fazer a criação de várias estruturas work mudando o valor de Star e End
    int start;          //parametro
    int end;            //parametro
    double soma;        //area reservada para o resultado
};

void
aloca_vetor(struct vetor_t *v, int size);

double
soma_vetor(struct vetor_t *v, int start, int end);

struct work_t*
aloca_work(struct vetor_t *v, int start, int end);

void* thr_soma(void* param); //um ponteiro para qualquer tipo de dado e retorna um ponteiro para qualquer tipo de dado

int main(void) {
    struct vetor_t v;
    struct work_t *trab_thr[2]; //alocação estática = um array de 2 posição que cada uma das posição é um ponteiro.
    pthread_t idthread[NTHREAD];
    int i;
    aloca_vetor(&v, SIZE); // ao completar esta operação aloca o vetor
    trab_thr[0] = aloca_work(&v, 0, v.length/2);
    trab_thr[1] = aloca_work(&v, v.length/2+1, v.length);

    //thr_soma(trab_thr[0]);
    //thr_soma(trab_thr[1]);

    for(i=0; i<NTHREAD; i++){
       // pthread_attr_t att;
       // pthread_attr_init(&att); //pode alocar memória
       // pthread_attr_setdetachstate((&att, PTHREAD_CREATE_JOINABLE)); //nao é necessário ser feito pois é o padrão

        pthread_create(&idthread[i],//var para receber id da thread cruiada
                       &att,//atributos
                       &thr_soma,//função ponto-de-entrada da thread
                       &trab_thr[i]//o parametro da thread
                       )

       // pthread_attr_destroy(&att); não é necessário ter, a menos que seja necessário
    }

    if (err){
        perror("phtread creat() falhou");
        exit(EXIT_FAILURE);
    }

    //struct res_thread* res;
    for(i=0; i<NTHREAD; i++){
            pthread_join(idthread[i], NULL); //Operação de sincronização.
    }

    printf("media= %f\n",
           trab_thr[0]->soma+trab_thr[1]->soma)/SIZE);

    printf("v.length=%d\n", v.length);


    pthread_exit(NULL);// colocar para o compilador retirar apenas a thread

    //return (EXIT_SUCCESS);

}

void* thr_soma(void* param){
    struct work_t* w = (struct work_t*) param;
    w->soma = soma_vetor(w->v, w->start, w->end);
    pthread_exit(NULL);
    //return NULL;
}

void aloca_vetor(struct vetor_t *v, int size) {
    v->dados = calloc(size, sizeof(double));
    v->length = size;
}

double soma_vetor(struct vetor_t *v, int start, int end) {
    double total = 0.0;
    for (int i=start; i<end; i++) {
        total += v->dados[i];
    }
    return total;
}

struct work_t* aloca_work(struct vetor_t *v, int start, int end) {
    struct work_t *r = malloc(sizeof(struct work_t));
    r->v = v;
    r->start = start;
    r->end = end;
    return r;
}
