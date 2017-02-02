



/*ordenação topologica em ordem parcial em ingrafos 

- Inicialmente considera-se um objeto que  não é precedido por nenhum outro na ordem parcial.
Esse objeto é o primeiro na saída, isto é na ordem final.
-Agora remova o objeto do conjunto S.
-O conjunto Resultante obedece novamente a ordem parcial.
-Repita todo processo até que todos elementos sejam removidos para saida. 
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct vetorString{
	char nome[50];
}VetorString;


typedef struct grafo{
	VetorString* tarefas;
	int** matriz; //ponteiro apontando para outro ponteiro que aponta para o valor final, 
	int* predecessor;
	int* resultados;
	int* cj; // conjunto disjunto.	

	int numeroDeTarefas;
}Grafo;



Grafo* inicializarGrafo(int);
Grafo* lerArquivo();
void gravarArquivo(Grafo*);
void adicionarNaMatriz(Grafo*,int,int );
void atualizarConjuntos(Grafo*,int,int);
void imprimirInformacao(Grafo*);
void atualizarPredecessores(Grafo*);
void inicializarPredecessores(Grafo*);
int contElementosNaMatriz(Grafo*);
void atualizarGrafo(Grafo*); // função principal


int main(){

	Grafo* s;
	s = lerArquivo();	
	inicializarPredecessores(s);	
	
	imprimirInformacao(s);
	
	while( contElementosNaMatriz(s) > 0  ){
		printf("Elementos na matriz: %d\n",contElementosNaMatriz(s) );		
		atualizarGrafo(s);
		imprimirInformacao(s);
	}	
	

	atualizarGrafo(s);
	imprimirInformacao(s);
	gravarArquivo(s);	

	printf("\n");
	printf("Fim\n");	
	return 0;
}






Grafo* inicializarGrafo(int n){
	Grafo* grafo = malloc(sizeof(Grafo));	
	grafo->tarefas = (VetorString*) calloc(n,sizeof(VetorString));		
	grafo->numeroDeTarefas = n;

	int* teste = calloc(n,sizeof(int));
	

	grafo->resultados = (int*) calloc(n + 1,sizeof(int)); 
	grafo->cj = (int*) calloc( n + 1, sizeof(int));	
	grafo->predecessor = (int*) calloc(n + 1,sizeof(int)); 

	//aloca as linhas da matriz
	grafo->matriz = (int**) calloc(n + 1,sizeof(int)); //aloca 0 a n - 1 linhas
	
	if( grafo->matriz == NULL){
		printf("Erro: Memoria Insuficiente!\n");
		return NULL;	
	}	
	
	//aloca as colunas da matriz
	int i;
	for( i = 0; i < n + 1; i++){
		grafo->matriz[i] = (int*) calloc( n + 1,sizeof(int));// para cada linha alocada, n clolunas colunas	 	
		if( grafo->matriz[i] == NULL ){
			printf("Erro: Memoria Insuficiente!\n");
			return NULL;		
		}	
	}	
	
	
	// inicializando conjuntos disjuntos com valores de 1 até n.

	
	for( i = 1; i < grafo->numeroDeTarefas + 1; i++){
		grafo->cj[i] = i;	
	}

	return grafo;
}


Grafo* lerArquivo(){

	Grafo* s;	
	FILE* arquivo = fopen("ordtopo.in","r"); //arquivo de leitura
	int n = 0;
	int i = 0;	
	int x,y = 1;

	if( arquivo == NULL){
		printf("Erro:ao carregar arquivo.\n");	
		exit(1);	
	}
		
		while ( n  == 0){
			fscanf(arquivo, "%d", &n); // recebe inteiro n		
			s = inicializarGrafo(n);
			
						
			while( i <= n){
				fgets(s->tarefas[i].nome, 50 , arquivo ); // recebe n tarefas
				i++;			
			}
			
			/*adicionar valores na matriz */	
			while( fscanf(arquivo,"%d %d",&x,&y) != EOF ){
				if( x && y > 0 && ( x && y <= n ))	{					
					adicionarNaMatriz(s,x,y);					
			
				 }					
				
			}
			
		}

	

	fclose(arquivo);
	return s;	
}

void gravarArquivo(Grafo* s){
     
     FILE* arquivo =  fopen("ordtopo.out","w"); // criar arquivo para gravação    
     int i = 1;
     
      if( arquivo == NULL){
          printf("Erro: ao criar arquivo\n");
          exit(1);
      }   
     
     fprintf(arquivo,"Saida:\n");

     for( s->resultados[i]; i <= s->numeroDeTarefas; i++ ){
          fprintf(arquivo,"Tarefa %d : %s",s->resultados[i],s->tarefas[s->resultados[i]].nome);
          
     }
     
     fclose(arquivo);
}    











void adicionarNaMatriz(Grafo* s,int x,int y){
	int conjuntoAntigo;
	int conjuntoNovo;
	
	if( s->cj[x] != s->cj[y] ){
		conjuntoNovo = s->cj[x];
		conjuntoAntigo =  s->cj[y];
		s->matriz[x][y] = 1;
		atualizarConjuntos(s,conjuntoAntigo,conjuntoNovo);
	}	
}

void atualizarConjuntos( Grafo* s, int conjuntoAntigo, int conjuntoNovo){
	int i = 0;

	for( i = 1; i <= s->numeroDeTarefas; i++){
		if( s->cj[i] == conjuntoAntigo){
			s->cj[i] = conjuntoNovo;
		}
	}
}



void inicializarPredecessores(Grafo*s){
	int i,j;
	int cont;	

	for( i = 1; i <= s->numeroDeTarefas; i++){
		cont = 0;		
				
		 for( j = 1; j <= s->numeroDeTarefas; j++){
			if ( s->matriz[j][i] == 1 ){
				cont++;
			}		
		  }
		
		 if( cont >= 1 ){
			s->predecessor[i] = cont;
		 }
					 
				
	}

}




int contElementosNaMatriz(Grafo* s){
	int i,j = 0;
	int cont = 0;
	
	for( i = 1; i <= s->numeroDeTarefas;i++ ){
		for( j = 1; j <= s->numeroDeTarefas;j++ ){
			if( s->matriz[i][j] == 1){
				cont++;	
			}			
		}	

	}
	

	


	return cont; 
}

void atualizarPredecessores(Grafo*s){
	int i,j;
	int cont;	

	for( i = 1; i <= s->numeroDeTarefas; i++){
		cont = 0;		
		if(  s->predecessor[ i] != - 1   ){		
		 	for( j = 1; j <= s->numeroDeTarefas; j++){
				if ( s->matriz[j][i] == 1 ){
					cont++;
				}		
			}
		
			s->predecessor[i] = cont;
			
				 
		}		
	}
}





void atualizarGrafo(Grafo* s){
	int i = 1;
	
	while( i <= s->numeroDeTarefas){
	
		if( s->predecessor[i] == 0  ){
			
			int j = 1;
			while( s->resultados[j] != 0 && s->resultados[j] <= s->numeroDeTarefas  ){
				j++;
			}
			if(  s->resultados[j]  == 0){
				int k = 1;				
				for( k = 1; k <= s->numeroDeTarefas; k++){
					if( s->matriz[i][k] == 1  ){
						s->matriz[i][k] = 0;
			 		}		
				}
			
				s->resultados[j] = i;
				s->predecessor[i] = -1;
			}
		}


		i++;
	}

	atualizarPredecessores(s);

}








void imprimirInformacao(Grafo* s){ // precedimento que imprimir estado atual do grafo.

	int i = 0;
	int j = 0;	
	
	printf("Estado Atual do Grafo\n");
	printf("\n");
	printf("Matriz \n");

	/*imprimi matriz */
	
	for( i = 1; i <= s->numeroDeTarefas ; i++){
		for( j = 1; j <= s->numeroDeTarefas ; j++ ){
			printf("%d",s->matriz[i][j]);
		}			   	
		printf("\n");
	}

	printf("\n");

	
	
	/*imprimi verificador*/	

	printf("Predecessores\n");
	for( i = 1; i <= s->numeroDeTarefas; i++){
		printf(" %d ", i);	
	}
	printf("\n");
	for( i = 1; i <= s->numeroDeTarefas; i++ ){
		printf("[%d]",s->predecessor[i]);
	}	
	
	printf("\n");
	printf("\n");


	/*imprimi conjunto disjunto */	

	printf("Conjunto Disjunto\n");
	
	
	
	for( i = 1; i < s->numeroDeTarefas + 1; i++){
		printf(" %d ", i);	
	}
	printf("\n");
	for( i = 1; i < s->numeroDeTarefas + 1; i++ ){
		printf("[%d]",s->cj[i]);
	}	
	
	printf("\n");
	printf("\n");
	
	
	
	/*imprimi Resultados */	

	printf("Resultados\n");
	
	
	
	for( i = 1; i <= s->numeroDeTarefas; i++){
		printf(" %d ", i );	
	}
	printf("\n");
	for( i = 1; i <= s->numeroDeTarefas; i++ ){
		printf("[%d]",s->resultados[i]);
	}	
	
	printf("\n");

}





