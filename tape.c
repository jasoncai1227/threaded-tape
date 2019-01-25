#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "pthread.h"
#include "tape.h"

//uint8_t* tape;

struct HEAD{
	pthread_t thread;
	int base;
	FILE* fp;
};

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
struct HEAD* head;
int count=0;
FILE* fp;
int file_length;
int cursor;

void* tape_reader(void* args) {
	int i=*(int*)args;
	int cur=cursor;
	pthread_mutex_lock(&lock);
		//fseek(fp,head[i].base,SEEK_SET);
			uint8_t tape[abs(cur)];
		//fread(tape,cur,1,fp);
		//fwrite(tape,cur,1,head[i].fp);
		if(cur>0){
			if(head[i].base+cur<=file_length){
				fseek(fp,head[i].base,SEEK_SET);
				fread(tape,cur,1,fp);
				//printf("%s\n",tape);
				head[i].base=head[i].base+cur;
				fwrite((char*)tape,abs(cur),1,head[i].fp);
			}else{
				
				int len_1=file_length-head[i].base;
				int len_2=cur-len_1;
				uint8_t temp[len_1];
				uint8_t temp_2[len_2];
				fseek(fp,head[i].base,SEEK_SET);
				fread((char*)temp,len_1,1,fp);
				//fgets((char*)temp,len_1,fp);
				rewind(fp);
				fread((char*)temp_2,len_2,1,fp);
				//fgets((char*)temp_2,len_2,fp);
				head[i].base=len_2;
				// int result_len=strlen(strcat((char*)temp,(char*)temp_2));
				// // fwrite(strcat((char*)temp,(char*)temp_2),result_len,1,head[i].fp);
				//fputs(strcat((char*)temp,(char*)temp_2),head[i].fp);
				fwrite((char*)temp,len_1,1,head[i].fp);
				fwrite((char*)temp_2,len_2,1,head[i].fp);
			}
		}else if(cur<0){
			uint8_t temp_3[1];
			if(head[i].base+cur>=0){
			int current=head[i].base;
			for(int k=0;k<abs(cur);k++){
				current=current-1;
				fseek(fp,current,SEEK_SET);
				fread((void*)temp_3,1,1,fp);
				fwrite((void*)temp_3,1,1,head[i].fp);
			}
			head[i].base=head[i].base+cur;
			}else{
				int left=abs(cur)-head[i].base;
				int current_2=head[i].base;
				for(int j=0;j<head[i].base;j++){
					current_2=current_2-1;
					fseek(fp,current_2,SEEK_SET);
					fread(tape,1,1,fp);
				}
				fseek(fp,0,SEEK_END);
				int back=0;
				for(int q=0;q<left;q++){
					back=back-1;
					fseek(fp,back,SEEK_CUR);
					fread(tape,1,1,fp);
				}
				head[i].base=file_length-abs(cur)-head[i].base;
			}
		}else head[i].base=head[i].base;
	
	
	pthread_mutex_unlock(&lock);
	return NULL;
}


int main(int argc,char** argv) {
	if(argv[1]==NULL){
		printf("Tape Not Inserted\n");
		return 0;
	}
	fp=fopen(argv[1],"rb");
	if(fp==NULL){
		printf("Cannot Read Tape\n");
		return 0;
	}
	fseek(fp,0,SEEK_END);
	file_length=ftell(fp);
	rewind(fp);
	//printf("%d\n",file_length);
	//tape=(uint8_t*)malloc(sizeof(uint8_t)*file_length);
	
	while(1){
		char input[1024];
		char* command_1;
		char* command_2;
		char* command_3;
		int base;
		pthread_t thread=0;
		char file_name[20];
		FILE* file_p;
		fgets(input,1024,stdin);
		command_1=strtok(input," \n");
		if(strcmp(command_1,"QUIT")==0){
			break;
		}
		if(strcmp(command_1,"HEAD")==0){
			command_2=strtok(NULL," \n");
			base=atoi(command_2);
			count++;
			if(count==1){head=(struct HEAD*)malloc(sizeof(struct HEAD)*count);}
			else {head=(struct HEAD*)realloc(head,sizeof(struct HEAD)*count);}
			if(base>=0){
			printf("HEAD %d at +%d\n",count,base);
			printf("\n");
			}else 
			{
				printf("HEAD %d at %d\n",count,base);
				printf("\n");
			}
			
			if(base>=0){
				base=base%file_length;
				head[count-1].base=base;
			}else {
				base=file_length-abs(base);
				head[count-1].base=base;
			}
			head[count-1].thread=thread;
			snprintf(file_name,20,"head%d",count);
			file_p=fopen(file_name,"wb");
			head[count-1].fp=file_p;
			
		}
		if(strcmp(command_1,"READ")==0){
			command_3=strtok(NULL," \n");
			cursor=atoi(command_3);
			pthread_mutex_init(&lock,NULL);
			int arg[count];
			for(int i=0;i<count;i++){
				arg[i]=i;
				if(pthread_create(&(head[i].thread),NULL,tape_reader,arg+i)!=0){
					perror("Unable to create thread\n");
					return 1;
				}
			}
			for(int k=0;k<count;k++){
				if (pthread_join(head[k].thread, NULL) != 0) {
					perror("unable to join thread");
					return 1; }
			}
			printf("Finished Reading\n");
			printf("\n");
		}
	}
	return 0;
}
