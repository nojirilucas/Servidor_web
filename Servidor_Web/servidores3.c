#include "server.h"

//Funçao de criacao do socket para o servidor TCP
// AF_INET tipo de conexão sobre IP para redes.
// SOCK_STREAM protocolo com controle de erros.
// 0 seleção do protocolo TCP
int criaSocket (int qtde_con){
    
    int iniciaSocket, yes = 1;
    struct sockaddr_in servidor;
    iniciaSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (iniciaSocket == -1){
        perror("erro");
        exit (1);
    }
    if (setsockopt (iniciaSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror ("setsocket");
        exit (1);
    }
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY); 
    servidor.sin_port = htons(PORT); 
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    if(bind(iniciaSocket, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){ //Ligando as estruturas do servidor com o ID do socket 
        close(iniciaSocket);
        perror("bind");
        exit(1);
    }
    if (listen(iniciaSocket, qtde_con) < 0){ //Limita o numero de conexoes, uma de cada vez
        perror ("listen");
        exit(1);
    }
    return iniciaSocket;
}

Requisicao *readHeader (int clienteSocket){

    Requisicao *req = malloc (sizeof(Requisicao));
    req->bytes_lidos = 0;
    req->bytes_lidos = read (clienteSocket, req->cabecalho, HEADER_SIZE);
    return req;
}

Resposta *carregaArq (char *url){

    Resposta *arquivo = malloc (sizeof (Resposta));
    FILE *arqLer = fopen (url, "r");
    if (arqLer == NULL){
        perror ("Not found");
        arquivo->status = -1; //Primeiro status de rro
        arqLer = fopen ("www/not_found.html", "r"); //Lendo arquivo do erro 404
        fseek (arqLer, 0, SEEK_END);
        arquivo->tamMsg = ftell(arqLer);
        arquivo->bufferResp = malloc (arquivo->tamMsg);
        rewind (arqLer);
        fread (arquivo->bufferResp, 1, arquivo->tamMsg, arqLer);
        fclose (arqLer);
        return arquivo;
    }
    fseek (arqLer, 0, SEEK_END);
    arquivo->tamMsg = ftell(arqLer);
    arquivo->bufferResp = malloc (arquivo->tamMsg);
    
    if (arquivo->bufferResp == NULL){ //Se o buffer n possuir memoria suficiente para o arq
        perror ("Payload");
        arquivo->status = -2; //Segundo status de erro
        fclose (arqLer);  
        arqLer = fopen ("www/payload.html", "r"); // Lendo o arquivo do erro 413.
        fseek (arqLer, 0, SEEK_END);
        arquivo->tamMsg = ftell(arqLer);
        arquivo->bufferResp = malloc (arquivo->tamMsg);
        rewind (arqLer);
        fread (arquivo->bufferResp, 1, arquivo->tamMsg, arqLer);
        fclose (arqLer);
        return arquivo;
    }
    rewind (arqLer);
    fread (arquivo->bufferResp, 1, arquivo->tamMsg, arqLer);
    fclose (arqLer);
    arquivo->status = 0;
    return arquivo;
}

Resposta *httpHeader (Requisicao *req){

   
    Resposta *arquivo, *res = malloc (sizeof(Resposta));
    char url[URL_SIZE] = {0};
    char cabecalho[100] = {0};
    int i, j = 0;
    int cabecalhoTAM;
    while(req->cabecalho[i] != ' ')
    i++;
    while(req->cabecalho[i] != ' '){
        if (j > 0 || req->cabecalho[i] != '/'){
            url[j] = req->cabecalho[i];
            j++;
        }
        i++;
    }

    DIR *d = opendir(url);
    if ((url[0] == '/' && url[1] == 0) || url[0] == 0 || d != NULL){ //Verifica se o arquivo esta em um diretorio ou eh um arq normal
        closedir (d);
        arquivo = carregaArq ("www/index.html");
    }else{      
        if(strncmp ("www/", url, 4)){// Se a url não tiver o prefixo www/.
            char temp[URL_SIZE] = {0};
            sprintf (temp, "www/%s", url);
            arquivo = carregaArq (temp);
        }else {
            arquivo = carregaArq (url);
        }
    }
    
    if (arquivo->status == -1){ //ERRO 404 - Se n for encontrado
        sprintf (cabecalho, "HTTP/1.0 404 Not Found\nContent-Lenght: %d\r\n\r\n", arquivo->tamMsg);
        
    }
    else if (arquivo->status == -2){// RRRO 413 - Se o buffer n possuir memoria suficiente
        sprintf (cabecalho, "HTTP/1.0 413 Payload Too Large\nContent-Lenght: %d\r\n\r\n", arquivo->tamMsg);
    }// Se foi possível carregar o arquivo.
    else {
        sprintf (cabecalho, "HTTP/1.0 200 OK\nContent-Lenght: %d\r\n\r\n", arquivo->tamMsg);
    }
    cabecalhoTAM = strlen (cabecalho);
    res->bufferResp = malloc (arquivo->tamMsg + cabecalhoTAM);
    memcpy (res->bufferResp, cabecalho, cabecalhoTAM);
    memcpy (res->bufferResp + cabecalhoTAM, arquivo->bufferResp, arquivo->tamMsg);
    res->tamMsg = cabecalhoTAM + arquivo->tamMsg;
    free (arquivo->bufferResp);
    free (arquivo);
    return res;
}

void clienteResp (int clienteSocket){ // Funcao que recebe e libera as estruturas para a conexao requisitada

    Requisicao *req = readHeader (clienteSocket);
    Resposta *res = httpHeader (req);
    write (clienteSocket, res->bufferResp, res->tamMsg);
    close (clienteSocket);
    free (req);
    free (res->bufferResp);
    free (res);
}

void clienteRespThread (void *args){

    pthread_mutex_lock (&qtde_requisicoes_protect);
    qtde_requisicoes++;
    pthread_mutex_unlock (&qtde_requisicoes_protect);
    int clienteSocket = *(int *)args;
    clienteResp (clienteSocket);
    pthread_mutex_lock (&qtde_requisicoes_protect);
    qtde_requisicoes--;
    pthread_mutex_unlock (&qtde_requisicoes_protect);
    free (args);
}

// void sckt_accept(int sockfd, clientent *client){
// 	client->socklen = sizeof(client->sock_addr);
// 	client->sockfd = accept(sockfd, (struct sockaddr*)&(client->sock_addr),&(client->socklen));

// 	if(client->sockfd < 0){
// 		sckt_error("<Connect::sckt_connect> "
// "Erro ao aceitar a conexao", errno);
// 	}
// }

/////////////////////////////////////////////////////////////////
/////////////////////////Servidores//////////////////////////////
/////////////////////////////////////////////////////////////////

void iterativo (void){ //Servidor Iterativo

    int iniciaSocket, clienteSocket, sizeSockaddr = sizeof(struct sockaddr_in);
    struct sockaddr_in cliente;
    iniciaSocket = criaSocket (1);
    while (1){
        clienteSocket = accept(iniciaSocket, (struct sockaddr *) &cliente, (socklen_t *) &sizeSockaddr);
        clienteResp (clienteSocket);
    }    
    close (iniciaSocket);
}

void paralelo (void){ //Servidor utilizando thread
    
    pthread_mutex_init (&qtde_requisicoes_protect, NULL);
    int iniciaSocket, *clienteSocket;
    int sizeSockaddr = sizeof(struct sockaddr_in);
    struct sockaddr_in cliente;
    iniciaSocket = criaSocket (QTDE_CONEXOES);
    qtde_requisicoes = 0;
    while (1){
        clienteSocket = malloc (sizeof(int));
        clienteSocket[0] = accept(iniciaSocket, (struct sockaddr *) &cliente, (socklen_t *) &sizeSockaddr);
        pthread_t t_local;
        if (pthread_create (&t_local, NULL, (void *)&clienteRespThread, (void *)&clienteSocket[0]) < 0){
                perror("create thread error");
            exit(1);
        }
    }
    close (iniciaSocket);
    pthread_mutex_destroy (&qtde_requisicoes_protect);
}

void concorrente (void){ //Servidor usando o select

    int iniciaSocket, clienteSocket, i, nfds;
    int size_sockaddr = sizeof (struct sockaddr_in);
    struct sockaddr_in cliente;
    fd_set read_fds, master;
    FD_ZERO (&master);
    FD_ZERO (&read_fds);

    iniciaSocket = criaSocket (QTDE_CONEXOES);
    FD_SET(iniciaSocket, &master);
    
    while (1){
        read_fds = master;
        nfds = select (iniciaSocket + 1, &read_fds, NULL, NULL, NULL);
        if (nfds == -1){
            perror ("select");
            exit(1);
        }
        while(i <= iniciaSocket){
            if (FD_ISSET (i, &read_fds)){ //Verificando se existe algum dado no socket
                if (i == iniciaSocket){
                    clienteSocket = accept (iniciaSocket, (struct sockaddr *)&cliente, (socklen_t *)&size_sockaddr);
                    if (clienteSocket == -1){
                        perror ("accept");
                        exit (1);
                    }else{ // Se a conexão for aceita sem erros.
                        FD_SET (clienteSocket, &master);
                        if (clienteSocket > iniciaSocket){
                            iniciaSocket = clienteSocket;
                        }
                    }
                }else {
                    clienteResp (i);
                    FD_CLR (i, &master);
                }
            }
        i++;
        }
    }
}

void produtor (void){

    int iniciaSocket, clienteSocket;
    int sizeSockaddr = sizeof(struct sockaddr_in);
    struct sockaddr_in cliente;
    iniciaSocket = criaSocket (QTDE_CONEXOES);

    int i = 0;
    while(i < BUFFER_SIZE){
        requisicoes[i] = -1;
        i++;
    }
    
    pthread_mutex_init (&qtde_requisicoes_protect, NULL);
    qtde_requisicoes = 0;
    
    while(i < QTDE_CONEXOES){
        pthread_mutex_init (&requisicoes_protect[i], NULL);
        // if (pthread_create (&threads[i], NULL, (void *)&consumidor, NULL) < 0){
        //         perror("create thread error");
        //     exit(1);
        // }
        i++;
    }
    
    while (1){
        clienteSocket = accept(iniciaSocket, (struct sockaddr *) &cliente, (socklen_t *) &sizeSockaddr);
        for (i = 0; i < BUFFER_SIZE; ++i){
            if (requisicoes[i] == -1){ // Vetifica a posicao do buffer esta aberta ou fechada
                if (!pthread_mutex_trylock(&requisicoes_protect[i])){
                    requisicoes[i] = clienteSocket;
                    pthread_mutex_unlock(&requisicoes_protect[i]);
                    break;
                }
            }else if (requisicoes[BUFFER_SIZE - i - 1] == -1){
                if (!pthread_mutex_trylock(&requisicoes_protect[BUFFER_SIZE - i - 1])){
                    requisicoes[BUFFER_SIZE - i - 1] = clienteSocket;
                    pthread_mutex_unlock(&requisicoes_protect[BUFFER_SIZE - i - 1]);
                    break;
                }
            }
        }
    }
    pthread_mutex_destroy (&qtde_requisicoes_protect);
    for (i = 0; i < QTDE_CONEXOES; ++i){
        pthread_mutex_destroy (&requisicoes_protect[i]);
    }
    close (iniciaSocket);
}

void consumidor (void){
    int i;
    while (1){
        for (i = 0; i < QTDE_CONEXOES; ++i){
            // Se estiver alguma requisição na fila para ser atendida.            
            if (!pthread_mutex_trylock (&requisicoes_protect[i])){
                if (requisicoes[i] != -1){
                    clienteResp (requisicoes[i]);
                  
                    requisicoes[i] = -1;
                }
                pthread_mutex_unlock (&requisicoes_protect[i]);
            }else if (!pthread_mutex_trylock (&requisicoes_protect[BUFFER_SIZE - i - 1])){
                if (requisicoes[BUFFER_SIZE - i - 1] != -1){
                    clienteResp (requisicoes[BUFFER_SIZE - i - 1]);
                    requisicoes[BUFFER_SIZE - i - 1] = -1;
                }
                pthread_mutex_unlock (&requisicoes_protect[BUFFER_SIZE - i - 1]);
            }
        }
        exit (1);
    }
}

