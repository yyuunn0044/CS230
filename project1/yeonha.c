#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int main (void){
	char com[1000]={};       //com: save each command
	char me[]="20140235>";   //me: prompt (studEnt ID)
	char* newp;              //newp: each word(token) of a command is saved in 'newp'
	char* path;              //path: for express name of directory path, it just include "./"
	DIR* adir;               //arir: for information of directory
	struct dirent* adirent;  //adirent: for directory structure
	int regi;			 	 //regi&regi2: for saving simple return of fuction
	int regi2;
	printf("%s ",me);
	fgets(com, sizeof(com),stdin);  //save input command
	newp=strtok(com,"\n"); //remove 'enter'
	newp=strtok(com, " "); //save first token, 'ls','mkdir','cd',.....it indicates what command wants


	while( strcmp(newp, "exit")!=0 ){ //until meet 'exit', get commands


		while( newp!=NULL ){             //until each word in a command is used
			///////////start of 'ls'//////////
			if ( strcmp(newp,"ls")==0){
				adir=opendir(".");                         //open directory and read all directories
				while(((adirent)=readdir(adir))!=NULL){    //until meet NULL
					printf("%s\n", adirent->d_name);
				}
				closedir(adir);
				break;
			}

			/////start of 'mkdir'///////////
			else if ( strcmp(newp, "mkdir")==0 ){
				newp=strtok(NULL, " ");
				if (newp==NULL){     //'mkdir' needs directory name, but if it isn't exist?
					printf("%s ERROR: YOU NEED TO TYPE DIRECTORY NAME\n",me);
				}
				while( newp!=NULL){      //until all input directories are made
					mkdir(newp, 0777);
					newp=strtok(NULL," ");
				}
			}
			///////start of 'rmdir'/////////
			else if ( strcmp(newp, "rmdir")==0 ){
				newp=strtok(NULL, " ");
				if (newp==NULL){
					printf("%s ERROR: YOU NEED TO TYPE DERECTORY NAME\n",me); //'rmdir' needs directory name, but if it isn't exist?
					break;
				}
				while( newp!=NULL ){   				//until all input directories are removed
					regi=0;
					adir=opendir(".");
					if ( NULL!= adir){
						while( adirent=readdir(adir)){
							if (strcmp(newp, adirent->d_name)==0);{ //before remove directory, i have to check existence of the directory
								regi=1;
								break;
							}
						}
						closedir(adir);	
					}
					if ( regi==0 ){
						printf("%s ERROR: THE DIRECTORY '%s' DON'T EXIST, REMOVE WAS STOPPED\n",me, newp);  //NOT EXIST
						break;
					}
					else                                                                                    //EXIST	
						rmdir(newp); 
					newp=strtok(NULL," ");

				}
			
			}

			///////////start of 'cd'/////////////

			else if( strcmp(newp, "cd")==0 ){
				newp=strtok(NULL, " ");
				if(newp==NULL){   //'cd' command needs directory name or '..', but?
					printf("%s ERROR: YOU NEED TO TYPE DIRECTORY NAME\n",me);
					break;
				}
				else if( strcmp(newp, "..")==0){  // move to upper directory
					chdir("../");
					break;
				}

				else{
					path=strdup("./");
					strcat(path, newp);
					regi2=chdir(path);
					if(regi2==-1){
						printf("%s ERROR: DIRECTORY '%s' DON'T EXIST\n", me, newp); //target directory don't exist
						path=strdup("./");
						break;
					}
					if(regi2==0)
						path=strdup("./");
						break;
				}
			}
			else{ 
				printf("%s ERROR: UNEXPECTED COMMAND\n",me); //any unexpected command
				break;
			}

		}
		newp=strtok(NULL," ");     //get next command....
		
		printf("%s ",me);
		fgets(com, sizeof(com),stdin);
		newp=strtok(com,"\n");
		newp=strtok(com, " ");
	}
}
