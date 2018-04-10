#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<pthread.h>

typedef struct{
	int burst;
	int tasa;
} Param;

//hice un cambio
//hilos
void* automatico(void*);
void* mamual(void*);
void* generador(void*);
void* generadorManual(void*);

 
int main(int argc , char *argv[])
{
	int var;
	char param[256];
	char *param2;
	if(!strcmp("automatico",argv[1])){
		
		puts("pase auto");
		var = atoi(argv[2]);	
		printf("%d\n",var);
		Param *param = malloc(sizeof(Param));
		param->burst = atoi(argv[2]);
		param->tasa = atoi(argv[3]);
		printf("param burst> %d\nparam tsa>%d\n",param->burst,param->tasa);

		pthread_t main;
		pthread_create(&main,NULL,generador,(void*)param);
		pthread_join(main,NULL);
	}
	else if(!strcmp("manual",argv[1])){
		
		pthread_t main_thread;
		pthread_create(&main_thread, NULL,generadorManual, (void*) var);
		pthread_join(main_thread,NULL);
	}
	else{
		printf("Debe escribir: ./multiclient2 manual/atumatico\n");
	}
	
}

//hilo para generar burst y prioridad automaticamente
void* automatico(void* var){
	int sock, burst, prioridad, i;
	time_t t;
	srand((unsigned) time(&t));
	
	char burst_s[20];
	char pri_s[20];
	char* linea;
    	struct sockaddr_in server;
    	char message[1000] , server_reply[2000];
	char msg[2000], pri[2000];

	Param* param = (Param*) var;
	burst=(rand()%param->burst)+1;
	prioridad=(rand()%5)+1;
	
	sprintf(msg,"%d",burst);
    	//Create socket
    	sock = socket(AF_INET , SOCK_STREAM , 0);
    	if (sock == -1)
    	{
        	printf("Could not create socket");
    	}
    	puts("Socket created");
     	
    	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    	server.sin_family = AF_INET;
    	server.sin_port = htons( 8888 );
 
    	//Connect to remote server
    	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	perror("connect failed. Error");
        	return 1;
    	}
     
    	puts("Connected\n");
     
    	//keep communicating with server

        
	puts("...Inicio de proceso...\n");
         
        //Receive a reply from the server
	do{
		//memset(pri,'\0',sizeof(pri));
		sprintf(burst_s,"%d",burst);
		sprintf(pri_s,"%d",prioridad);
		strcat(burst_s," ");
		strcat(burst_s,pri_s);
		
		strcpy(msg,burst_s);
		
		send(sock,msg,strlen(msg),0);
		
	}while(++i<1);

	if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            return 1;
        }
	puts(" --- Envie al servidor ---");
	
        puts("Server reply :");
        puts(server_reply);
	
     
    close(sock);
    return 0;
}

void* manual(void* var){
	
	int sock, burst, prioridad, i;
	time_t t;
   	struct sockaddr_in server;
   	char message[1000] , server_reply[2000];
	char msg[2000], pri[2000];
	char* buff = (char *) var;
	
	sprintf(msg,"%d",burst);
    //Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
	        printf("Could not create socket");
	}
	puts("Socket created");
     
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    	server.sin_family = AF_INET;
    	server.sin_port = htons( 8888 );
 
    //Connect to remote server
    	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	perror("connect failed. Error");
        	return 1;
    	}
     
    	puts("Connected\n");
     
    //keep communicating with server

        printf("Enter message : ");
	puts("...Envio mshg...\n");
         
        //Receive a reply from the server
	do{
		//memset(buff,'\0',sizeof(buff));
		puts(buff);
		send(sock,buff,strlen(buff),0);
	}while(++i<1);

	if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            return 1;
        }
	puts(" --- asdasdad 0000");
	
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
	//char* nbrArch = var;
	//printf(nbrArch);
	FILE* archivo = fopen("datos","r");
	char line[256];
	char* burst;
	char* priority;
	char* data[2];
	
	int i=0;
	while(fgets(line,sizeof(line),archivo)){
		char* buff=line;
		//printf("%s",line);
		//puts("antes");
		pthread_t generarM;
		pthread_create(&generarM,NULL,manual,(void*) buff);
		pthread_join(generarM,NULL);
		//burst = strtok(buff," ");
		//priority = strtok(NULL," ");
		//puts(burst);
		//puts(priority);
		
		sleep(2);
	}

	fclose(archivo);
	
}




