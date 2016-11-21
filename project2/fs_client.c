#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	int ssock, csock;
	struct sockaddr_in saddr;
	
	char buf[10000];
	char filename[255];
	char path[255];
	int filesize;
	char filesizee[255];
	int readed=0;
	int state=0;
	int total=0;
	int len=0;
    FILE *fp;

	csock=socket(AF_INET, SOCK_STREAM, 0); //make client socket
	if (csock<0){
		printf("Socket Error\n");
		exit(0);
	}
	bzero((char *)&saddr, sizeof(saddr));
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(20003);
	if ( connect(csock, (struct sockaddr *)&saddr, sizeof(saddr)) <0 )  //connet to server socket
		exit(0);
	write(csock, argv[3], strlen(argv[3])); // send command .. ls or upload or download

	if(strncmp(argv[3], "-l", 2)==0){
		printf("============This is the list function\n");
		printf("Sent command: 1\n");

		while(1){
			memset(buf, 0, 10000);
			state=recv(csock, buf, sizeof(buf),0); //receive names of elements
			if (strncmp(buf, "end\n", 4)==0)       // it means the directory doesn't exist
				break;  
			if (state==0)                    //it means all the elements were read
				break;
			printf("\n");
			printf("%s\n", buf);
		}
		printf("============End List!\n");

	}

	else if(strncmp(argv[3], "-u", 2)==0){
		write(csock, strcat(argv[4],"\0"),strlen(argv[4])+1); // receive name of upload file
		printf("============This is the upload function\n");
		printf("Sent command: 2\n");
		printf("Sent filename: %s\n",argv[4]);
		memset(buf, 0, 10000);
		memset(filename, 0, 30);
		if (send(csock, argv[4], strlen(argv[4]), 0)<0)
			exit(0);


		sprintf(filename, "./");
		strncat(filename,argv[4], strlen(argv[4]));
		
		fp=fopen(filename, "rb");


		fseek(fp, 0, SEEK_END);   //save size of file
		filesize=ftell(fp);		
		
		if (send(csock, (char *)&filesize, sizeof(filesize),0)<0)
			exit(0);

		rewind(fp); //rewind file pointer to start
		while(1){
			readed = fread(buf, 1, filesize-total, fp); //read the file and send the characters

			if (readed>0){
				if(send(csock, buf, readed, 0)<0)
					exit(0);
				memset(buf, 0, 10000);
				total+=readed;
			}
			else if(total==filesize){
				fclose(fp);
				printf("============End Upload!\n");
				exit(0);
			}

		}
		printf("============End upload!\n");

	}
	else if(strncmp(argv[3], "-d", 2)==0){
		write(csock, strcat(argv[4], "\0"), strlen(argv[4])+1);
		printf("============This is the download function\n");
		printf("Sent command: 3\n");
		printf("Sent filename: %s\n",argv[4]);

		total=0;
		memset(buf, 0, 255);
		memset(path, 0, 255);
		memset(filename, 0,255);
		if (send(csock, argv[4], strlen(argv[4]), 0)<0){
			close(csock);
			exit(0);
		}
		
		sprintf(filename, "./");
		strncat(filename,argv[4], strlen(argv[4]));


		fp=fopen(filename, "wb");
		if(fp==NULL){
			close(csock);
			exit(0);
		}
		if (recv(csock, (char *)&filesize, sizeof(filesize),0)<0){
			close(csock);
			exit(0);
		}

		while(filesize>0){
			readed=recv(csock, buf, filesize, 0); //receive characters and write to new file
			if(readed<0){
				close(csock);
				exit(0);
			}
			else{
				fwrite(buf, 1, readed, fp);
			}
			filesize-=readed;
		}
		fclose(fp);
		printf("============End Download!\n");

	}

	else
		printf("Unproper Command\n");

	close(csock);
	exit(0);
}
