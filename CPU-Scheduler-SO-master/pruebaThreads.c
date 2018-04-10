#include "estructuras.c"
#include <time.h>

#define TRUE   1 
#define FALSE  0 
#define PORT 8888 

void * consola(void *lts){
	ListasArgs * listas = lts;
	ListaColaReady * colaReady = listas->colaReady;
	bool * seguir = listas->seguir;
	int input;
	while (*seguir){
		printf("\nComandos:\n\n -> 'd' para detener la ejecucion.\n\n -> 'l' para ver cola del Ready. \n");
		input = getchar();
		//*input = putchar(tolower(*input));		
		if (input == 'd'){
			*seguir = false;
			printf("\nEJECUCION DETENIDA\n");
		} else {
			if (input == 'l'){
				imprimirLista(colaReady);
			} else {
				printf("\nCOMANDO INCORRECTO\n");
			}
		}
	}
}

void * jobScheduling(void *lts){
	ListasArgs * listas = lts;
	ListaColaReady * colaReady = listas->colaReady;
	bool * seguir = listas->seguir;
	
	int opt = TRUE;  
    	int master_socket , addrlen , new_socket , client_socket[30] ,  max_clients = 30 , activity, i , valread , sd;  
    	int max_sd;  
    	struct sockaddr_in address;  
        
    	char buffer[2000];  //data buffer of 1K 
        
    	//set of socket descriptors 
    	fd_set readfds;  
        
    	//a message 
    	char *message = "ECHO Daemon v1.0 \r\n";  
    
    	//initialise all client_socket[] to 0 so not checked 
    	for (i = 0; i < max_clients; i++)  
    	{  
        	client_socket[i] = 0;  
    	}  
        
    	//create a master socket 
    	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    	{  
        	perror("socket failed");  
        	exit(EXIT_FAILURE);  
    	}  
    
    	//set master socket to allow multiple connections , 
    	//this is just a good habit, it will work without this 
    	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          	sizeof(opt)) < 0 )  
    	{  
        	perror("setsockopt");  
        	exit(EXIT_FAILURE);  
    	}  
    
    	//type of socket created 
    	address.sin_family = AF_INET;  
    	address.sin_addr.s_addr = INADDR_ANY;  
    	address.sin_port = htons( PORT );  
        
    	//bind the socket to localhost port 8888 
    	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    	{  
        	perror("bind failed");  
        	exit(EXIT_FAILURE);  
    	}  
    	printf("Listener on port %d \n", PORT);  
        
    	//try to specify maximum of 3 pending connections for the master socket 
    	if (listen(master_socket, 3) < 0)  
    	{  
        	perror("listen");  
        	exit(EXIT_FAILURE);  
    	}  
        
    	//accept the incoming connection 
    	addrlen = sizeof(address);  
    	puts("Waiting for connections ...");  
        
    	while(*seguir)  
    	{  
		puts("===== valor seguir");
		printf("%d",*seguir);
        	//clear the socket set 
        	FD_ZERO(&readfds);  
    
        	//add master socket to set 
        	FD_SET(master_socket, &readfds);  
        	max_sd = master_socket;  
            
        	//add child sockets to set 
        	for ( i = 0 ; i < max_clients ; i++)  
        	{  
		    	//socket descriptor 
		    	sd = client_socket[i];  
		        
		    	//if valid socket descriptor then add to read list 
		    	if(sd > 0)  
		        FD_SET( sd , &readfds);  
                
		    	//highest file descriptor number, need it for the select function 
		    	if(sd > max_sd)  
		        	max_sd = sd;  
        	}  
    
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
	      
		if ((activity < 0) && (errno!=EINTR))  
		{  
		    printf("select error");  
		}  
		    
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds))  
		{  
			
		    if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
		    {  
			
		        perror("accept");  
		        exit(EXIT_FAILURE);  
		    }  
		    
		    //inform user of socket number - used in send and receive commands 
		    printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , 
ntohs(address.sin_port));  
          
                
		    //add new socket to array of sockets 
		    for (i = 0; i < max_clients; i++)  
		    {  
		        //if position is empty 
		        if( client_socket[i] == 0 )  
		        {  
		            client_socket[i] = new_socket;  
		            printf("Adding to list of sockets as %d\n" , i);  
		                
		            break;  
		        }  
           	    }  
        	}
	
            
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];  
                
		    if (FD_ISSET( sd , &readfds))  
		    {  
                //Check if it was for closing , and also read the 
                //incoming message 
			puts("espero read");
		        if ((valread = read( sd , buffer, 2000)) == 0)  
		        {  
				puts("despues de read");
		            //Somebody disconnected , get his details and print 
		            getpeername(sd , (struct sockaddr*)&address , \
		                (socklen_t*)&addrlen);  
		            printf("Host disconnected , ip %s , port %d \n" , 
		                  inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
		                
		            //Close the socket and mark as 0 in list for reuse 
		            close( sd );  
		            client_socket[i] = 0;  
		        }  
                    
                //Echo back the message that came in 
		        else
		        {  
		            //set the string terminating NULL byte on the end 
		            //of the data read 
		            buffer[valread] = '\0';  
				puts(buffer);
				char* prioridad_buffer;
				char* burst_buffer;
				burst_buffer = strtok(buffer," ");
				prioridad_buffer = strtok(NULL," ");
				int burst_num = atoi(burst_buffer);
				int prio_num = atoi(prioridad_buffer);
				insertar(burst_num, prio_num, colaReady->contPID, time(NULL) - colaReady->tiempoInicial, colaReady);
				printf("\nProceso insertado a la cola\n");
				char server_reply[2000];
				char num_pid[2000];
				sprintf(num_pid,"%d",colaReady->contPID);
				strcpy(server_reply,"Proceso insertado, PID= ");
				strcat(server_reply, num_pid);			
				colaReady->contPID ++;
			
		            send(sd , server_reply , strlen(server_reply) , 0 );  
		        }  
            	}  
		
        }  
	
    }  
        
	
	return 0;
}

void * cpuScheduling(void *lts){
	ListasArgs * listas = lts;
	ListaColaReady * colaReady = listas->colaReady;
	ListaColaReady * hist = listas->hist;
	bool * seguir = listas->seguir;
	int burst;
	int cont = 0;
	while (*seguir){
		if (!(isListaEmpty(colaReady))){
			printf("\nREADY\n");
			
			switch (listas->tipoAlgoritmo){ 
				case 0:
					burst = extraerFIFO(colaReady, hist);
					break;
				case 1:
					burst = extraerSJF(colaReady, hist);
					break;
				case 2:
					burst = extraerHPF(colaReady, hist);
					break;
				case 3:
					burst = extraerRR(listas->quantum,colaReady, hist);
					break;
			}
			
			sleep(burst);
		} else{
			sleep(1);
			cont ++;
		}
		
	}
	hist->tiempoFinal = time(NULL) - hist->tiempoInicial;
	
	return 0;
}

int  main(int argc, char * argv []) {  // Anadi (no tengo idioma espanol xd) el argV.
	if (argc == 1){
		printf("\nFalta indicar el tipo de algoritmo a utilizar (FIFO, SJF, HPF o RR), si es RR, indicar el quantum. Por ejemplo:\n./programa RR 4\n");
		return 0;
	}
	int i = 0;
	while( argv[1][i] ) {
		argv[1][i] = putchar(tolower(argv[1][i]));
		i++;
	}
	
	char inFifo[4] = "fifo";
	char inSjf[3] = "sjf";
	char inHpf[3] = "hpf";
	char inRr[2] = "rr";
	int algoritmo;
	int quantum = 0;
	if (cmpString(argv[1],inFifo)){
		algoritmo = 0;
	} else {
		if (cmpString(argv[1],inSjf)){
			algoritmo = 1;
		} else {
			if (cmpString(argv[1],inHpf)){
				algoritmo = 2;
			} else {
				if (cmpString(argv[1],inRr)){
					algoritmo = 3;
					quantum = atoi(argv[2]);
					
					if (argc < 3){
						printf("\nFalta el parametro del quantum, intente nuevamente.\n");
						return 0;
					}
				} else {
					printf("\nComando incorrecto. Intente nuevamente\n");
						return 0;
				}
			}
		}
	
	}
	
	printf("\nCont Arg: %d\n\nAlgoritmo: %d\n",argc,algoritmo);  // Hasta aqui acaba la parte de leer los parametros de entrada, se agrego funcion a estructuras.c
	
	bool flag = true;
	bool * seguir = &flag;
	srand(time(NULL));
	
	ListasArgs * listas = malloc(sizeof(ListasArgs));
	
	ListaColaReady * ptr_colaReady = malloc(sizeof(ListaColaReady));
    ptr_colaReady->primerNodo = 0;
	ptr_colaReady->tiempoInicial = time(NULL);
	ptr_colaReady->contPID = 0;
	
	ListaColaReady * hist = malloc(sizeof(ListaColaReady));
    hist->primerNodo = 0;
	hist->tiempoInicial = time(NULL);
	
	listas->colaReady = ptr_colaReady;
	listas->hist = hist;
	listas->seguir = seguir;
	listas->quantum = quantum;		// Se agrega a la lista de parametros el quantum y el tipo de algoritmo.
	listas->tipoAlgoritmo = algoritmo;
	
	pthread_t jobSch;
	pthread_t cpuSch;
	pthread_t console;
	pthread_create(&console, 0, consola,(void *)listas);
	pthread_create(&jobSch, 0, jobScheduling,(void *)listas);
	pthread_create(&cpuSch, 0, cpuScheduling,(void *)listas);
	//pthread_join(jobSch, NULL);
	pthread_join(cpuSch, NULL);
	pthread_join(console, NULL);
	
	reporteCPU(hist);
	
	return(0);
}
