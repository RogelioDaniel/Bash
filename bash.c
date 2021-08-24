#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

char **obtener_entrada(char *); //Funcion para obtener comandos
int cd (char*);

int main (){
	char **comando;
	char *input;
	pid_t hijo_pid;
	int stat_loc;
	
	while (1){
		
		input= readline("Proyecto bash >"); //Mensaje bash y leemos el contenido ingresado por el usuario
		comando = obtener_entrada(input); //Nuestra funcion de obtener comando lo recibe
	
		if(!comando[0] ){  //  Si esta vacio liberamos y seguimos
			free(input);
			free(comando);
			continue;
		}
		if(strcmp (comando[0], "cd") == 0){ //Comparamos nuestro comando  si son iguales continuamos
			if(cd(comando[1])< 0){
				perror(comando[1]);
			}
			continue;
		}
		hijo_pid=fork(); //Proceso hijo creado, copia exacta del padre ( Menos por PID y memoria)
		
              	//if(hijo_pid<0){  //Si nos quedamos sin memoria
		//	perror("Fork fallo");
		//	exit(1);
		//}
		if(hijo_pid == 0){ // Si no hay ningun error  y nos encontramos en el proceso hijo todo esta bien
			if(execvp(comando[0], comando) <0){  //Recibe el comando y sus argumentos
				perror(comando[0]);
				exit(1);
			}
		}
		else{
			waitpid(hijo_pid,&stat_loc,WUNTRACED);
		}

		free(input);
		free(comando);
	}
	return 0;
}

char **obtener_entrada(char *input){ //Aceptamos el comando como entrada
char **comando = malloc (8 *sizeof(char*));  //
	//if(comando == NULL ){ //Dividimos la cadenar y se duelve una matriz 
	//	perror("malloc fallo");
	//	exit(1);
	//}
	char *separador=" ";
	char *parsed;
	int index=0;
	
	parsed =strtok(input,separador); //Terminamos la matriz
	while ( parsed != NULL){
		comando[index] = parsed;
		index++;

		parsed=strtok(NULL,separador);
	}
	comando[index] = NULL;
	return comando;
}

int cd (char *path){
	return chdir(path);
}
