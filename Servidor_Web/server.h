
/* Bibliotecas de manipulação de IO e variáveis */
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>
/* Biblitecas de erros */
#include <errno.h>
/* Bibliotecas para o uso threads */
#include <pthread.h> 
/*Biblioteca de manipulaçao de diretorios*/
#include <dirent.h>
#include <libgen.h>
/* Bibliotecas para o uso do select() */
#include <sys/select.h>
/* Bibliotecas de manipulação de Sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
/* Bibliotecas de manipulação de Strings */
#include <string.h>

#define PORT 40000 //Porta para os sockets
#define QTDE_CONEXOES 32
#define BUFFER_SIZE 128
#define HEADER_SIZE 2048
#define URL_SIZE 256

// ESTRUTURAS PTHREAD
pthread_t threads[QTDE_CONEXOES];
u_int32_t qtde_requisicoes;
pthread_mutex_t qtde_requisicoes_protect;
pthread_mutex_t requisicoes_protect[BUFFER_SIZE];

// Modelo cliente servidor.
u_int32_t requisicoes[BUFFER_SIZE];

typedef struct {
    int bytes_lidos;
    char cabecalho[HEADER_SIZE];
} Requisicao;

typedef struct{
    u_int32_t tamMsg;
    u_int8_t status;
    char *bufferResp;
} Resposta;

// Estrutura de retorno quando se aceita uma conexão com usuários
typedef struct{
    int sockfd;
    struct sockaddr_in sock_addr;
    socklen_t socklen;
}clientent;

int criaSocket (int qtde_con);

Requisicao *readHeader (int clienteSocket);

Resposta *carregaArq (char *url);

Resposta *httpHeader (Requisicao *req);

void iterativo (void);

void clienteResp (int clienteSocket);

void clienteRespThread (void *args);

void paralelo (void);

void concorrente (void);

void produtor (void);

void consumidor (void);
