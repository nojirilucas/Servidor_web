#include "server.h"

void ajuda (){

    printf ("Ajuda -h.\n-s: opção de servidor.\n");
    printf ("-s 1: servidor sequencial.\n");
    printf ("-s 2: servidor paralelo.\n");
    printf ("-s 3: produtor consumidor.\n");
    printf ("-s 4: servidor com select.\n");
}

int main (int argc, char **argv){

    if (argc < 2){
        printf ("Por favor. informe os parâmetros.\n");
        return 1;
    }
    int opcao;
    char *servidor;
	opterr = 0;
	while ((opcao = getopt (argc, argv, "hs:")) != -1){
		switch (opcao) {
			case 's':
				servidor = optarg;
				break;
			case 'h':
                ajuda ();
				exit(0);
            default:
                ajuda ();
				break;
		}
    }
    if (servidor[0] < 49 || servidor[0] >   52){
        printf ("Valor %c está fora do espaço de opções.\nDigite novamente outro valor.\n",servidor[0]);
        return 1;
    }
    if (servidor[0] == '1'){
        iterativo ();
    }else if (servidor[0] == '2'){
        paralelo (); 
    }else if (servidor[0] == '3'){
        produtor();
    }else {
        concorrente ();
    }
    return 0;
}