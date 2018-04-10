#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<pthread.h>

//definir estructura pequena para agarrar los parametros por linea
typedef struct{
	int burst;
	int tasa;
} Param;


//hilos
void* automatico(void*);
void* mamual(void*);
void* generador(void*);
void* generadorManual(void*);

 
int main(int argc , char *argv[])
{
	int var;
	if(!strcmp("automatico",argv[1])){
		
		//recollection de parametros para automatico
		Param *param = malloc(sizeof(Param));
		param->burst = atoi(argv[2]);
		param->tasa = atoi(argv[3]);
		printf("param burst> %d\nparam tsa>%d\n",param->burst,param->tasa);

		pthread_t main;
		pthread_create(&main,NULL,generador,(void*)param);
		pthread_join(main,NULL);
	}
	else if(!strcmp("manual",argv[1])){
		
		char* nombre = argv[2];
		printf("%s\n", nombre);
		pthread_t main_thread;
		pthread_create(&main_thread, NULL,generadorManual, (void*) nombre);
		pthread_join(main_thread,NULL);
	}
	else{
		printf("Debe escribir: ./multiclient2 manual/atumatico\n");
	}
	
}


/*
 * Hilo para generar burst y prioridad automaticamente
 * Entradas: Estructura Param
 * Salidas: no tiene.
 * Restricciones: no tiene.
 */
void* automatico(void* var){
	int sock, burst, prioridad, i;
	//generador random
	time_t t;
	srand((unsigned) time(&t));
	
	char burst_s[20];
	char pri_s[20];
	char* linea;
    	struct sockaddr_in server;
    	char message[1000] , server_reply[2000];
	char msg[2000], pri[2000];

	//generando random segun el parametro escogdo
	Param* param = (Param*) var;
	burst=(rand()%param->burst)+1;
	prioridad=(rand()%5)+1;
	
	sprintf(msg,"%d",burst);
    	//Creacion de Socket
    	sock = socket(AF_INET , SOCK_STREAM , 0);
    	if (sock == -1)
    	{
        	printf("No se pudo crear el socket");
    	}
    	puts("==Socket creado==");
     	
	//configuracion del socket
    	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    	server.sin_family = AF_INET;
    	server.sin_port = htons( 8888 );
 
    	//Conectando con socket
    	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	perror("connect failed. Error");
        	return 1;
    	}
     
    	puts("Conectado\n");
     
    	//Comunicacion con socket

        
	puts("...Inicio de proceso...\n");
         
        
	do{
		//memset(pri,'\0',sizeof(pri));
		sprintf(burst_s,"%d",burst);
		sprintf(pri_s,"%d",prioridad);
		strcat(burst_s," ");
		strcat(burst_s,pri_s);
		
		strcpy(msg,burst_s);
		
		send(sock,msg,strlen(msg),0);
		
	}while(++i<1);

	//Recibiendo respuestas por parte del servidor
	if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            return 1;
        }
	puts(" --- Envio al servidor ---\n");
	puts(" --------------------------");
        puts("Server reply :");
        puts(server_reply);
	
     //Cierre de conexion y socket
    close(sock);
    return 0;
}

/*
 * Hilo para generar burst y prioridad por un archivo
 * Entradas: No tiene
 * Salidas: no tiene.
 * Restricciones: no tiene.
 */
void* manual(void* var){
	
	int sock, burst, prioridad, i;
	time_t t;
   	struct sockaddr_in server;
   	char message[1000] , server_reply[2000];
	char msg[2000], pri[2000];
	char* buff = (char *) var;
	
	sprintf(msg,"%d",burst);
    //Creando socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
	        printf("Could not create socket");
	}
	puts("Socket creado");
     
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    	server.sin_family = AF_INET;
    	server.sin_port = htons( 8888 );
 
    //Conexion con servidor
    	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	perror("connect failed. Error");
        	return 1;
    	}
     
    	puts("Conectado\n");
     
    //Comunicacion con servidor

	puts("...Inicio de proceso...\n");
         
        //Envio de burst y prioridad
	do{
		//memset(buff,'\0',sizeof(buff));
		puts("Enviando burst y prioridad");
		send(sock,buff,strlen(buff),0);
	}while(++i<1);

	//Recibe respuesta del servidor
	if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            return 1;
        }
	puts(" --- Envio al servidor ...");
	puts(" --------------------------");
        puts("Server reply :");
        puts(server_reply);

     
    close(sock);
    return 0;
}


void* generador(void* var){
	
	time_t t;
	srand((unsigned) time(&t));
	Param* param = (Param*) var;
	int tasa_sleep;
	
	while(1){
		tasa_sleep = (rand()%param->tasa)+1;
		 
		pthread_t generar;
		pthread_create(&generar,NULL,automatico,(void*) param);
		pthread_join(generar,NULL);
		printf("Tasa de creacion = %d segundos\n",tasa_sleep);
		printf("===========================================\n");
		sleep(tasa_sleep);

	}
}

void* generadorManual(void* var){
	char* nbrArch = var;
	printf("Generador> %s",nbrArch);
	FILE* archivo = fopen(nbrArch,"r");
	char line[256];
	char* burst;
	char* priority;
	char* data[2];
	time_t t;
	srand((unsigned) time(&t));
	int tasa_sleep;
	
	int i=0;
	while(fgets(line,sizeof(line),archivo)){
		char* buff=line;
		tasa_sleep=(rand()%5)+3;
		pthread_t generarM;
		pthread_create(&generarM,NULL,manual,(void*) buff);
		pthread_join(generarM,NULL);
		
		printf("Tasa de creacion = %d segundos\n",tasa_sleep);
		printf("===========================================\n");
		sleep(tasa_sleep);
	}

	fclose(archivo);
	
}




