#include <stdio.h>              //2015115907
#include <unistd.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <wait.h>

#define max_size 8192
#define end_work 9
int op_cnt = 0;
int nae_cnt = 1;
char *token_string = NULL;
char temp[200];
char temp_to[200];
char *to[200][200];
char *op[10];
char *nae[10];
int grep_counter = 1;
typedef enum {false, true} bool;

struct textarr {
    char text[max_size];    
    char text_out[max_size];
    int full;
    int full_n;
    int end_flag;
};
struct textarr *ta;
int n;
bool flag_end = false;
bool flag_end_all = false;
pthread_mutex_t *mutex;
pthread_mutex_t mutex_n;

void* thread_work(void *data)
{
    int status;

    int i = *((int*)data);
    int worker_pid[100] = {-1,};
    int pid_cnt = 0;
    int pd[100][2];    
    int flags = 0;
    char test3[max_size];  
    int temp;
    memset(worker_pid,-1, sizeof(worker_pid));
    memset(pd,0, sizeof(pd));
     for(pid_cnt = 0 ; pid_cnt<=op_cnt ; pid_cnt++)
    {
    pipe(pd[pid_cnt]);
    }
    fcntl(pd[op_cnt][0], F_SETFL, O_NONBLOCK);
    for(pid_cnt = 0 ; pid_cnt <op_cnt ; pid_cnt++)
    {
        worker_pid[pid_cnt] = fork();

        if(worker_pid[pid_cnt] == 0)
        {
            for(int j = 0; j<=op_cnt; j++)
            {
            if(j == pid_cnt) 
            {
               dup2(pd[j][0],STDIN_FILENO);
               close(pd[j][0]);
               close(pd[j][1]);
            }
            else if(j == pid_cnt+1)
            {
               if(pid_cnt != op_cnt -1)
               {
               dup2(pd[j][1],STDOUT_FILENO);
               close(pd[j][1]);
               close(pd[j][0]);
               }
               else close(pd[j][0]);
            }
            else
            {
               close(pd[j][0]);
               close(pd[j][1]);
                }
            }
            execvp(to[pid_cnt][0], to[pid_cnt]);       
         }
    }

    pid_t p_pid;
    int a;
    int j;
    for(j = 0; j<=op_cnt; j++)
    {
        if(j == 0)
            close(pd[j][0]);
        else if(j == op_cnt)
            close(pd[j][1]);
        else
        {
        close(pd[j][0]);
        close(pd[j][1]);
        }            
    }
    int re_size = 0;

    while(1)
    {
    while(ta[i].full == false)
    {
    if(flag_end == true)
        {
        close(pd[0][1]);
        close(pd[op_cnt][0]);
//        waitpid(-1,NULL,0);
        return 0;
        }
    }
    pthread_mutex_lock(&mutex[i]);
    write(pd[0][1],ta[i].text,max_size);
    pthread_mutex_unlock(&mutex[i]);
    memset(ta[i].text,'\0',sizeof(ta[i].text));
    while(re_size != 0)
    re_size = read(pd[op_cnt][0],test3,max_size);
    ta[i].text[0] = '\0';
    ta[i].full = false;
 
    if(strlen(test3)> 0)
    {
    while(ta[i].full_n);
    pthread_mutex_lock(&mutex_n);
    strncpy(ta[i].text_out,test3,strlen(test3));
    pthread_mutex_unlock(&mutex_n);   
    memset(test3,0,sizeof(test3));
    test3[0]='\0';
    ta[i].full_n = true;
    }
   }
    ta[i].end_flag = end_work;
//    p_pid = wait(&status);
    // while(!ta[i].full)
    // {
    //     if(flag_end == true && !ta[i].full)
    //         return 0;
    // }
//pid_child = wait(&status);
//     a = waitpid(worker_pid[op_cnt-1],&status,0);
return 0;
}

void* thread_work2(void *data)
{
  int i = *((int*)data);
    int r_w_chk = 0;
    int turn = 0;
    bool temp_flag = true;
    do{
    while(ta[turn].full_n == false)
    {
        if(flag_end_all == false)
        {
//        turn = (turn+1)%n; 
            continue;
        }
        if(flag_end_all == true)   
        {
        temp_flag = true;
            for(r_w_chk = 0 ; r_w_chk < n ; r_w_chk++)
            {
                if(ta[r_w_chk].full_n == true)
                {
                turn = r_w_chk;
                temp_flag = false;
                break;
                }
            }
            if(temp_flag == false)
                break;
            return 0;
        }  
    }
    // while(!ta[turn].full_n)
    //     turn = (turn+1)%n; 

    pthread_mutex_lock(&mutex_n);
    printf("%s\n", ta[turn].text_out);
    memset(ta[turn].text_out,0,sizeof(ta[turn].text_out));
    ta[turn].text_out[0] = '\0';
    ta[turn].full_n = false;
pthread_mutex_unlock(&mutex_n);
//    turn = (turn+1)%n; 
    }while(true);
}
int main(int argc, char* argv[])
{
    int j = 0;
    int i = 0;
    int t = 0;
    int *thread_id;
    char temp[100];
    if (strcmp(argv[1],"-n") != 0)
    {
        printf("error1\n");
        return 1;
    }
    n = atoi(argv[2]);

    if ( n <=0  )
    {
        printf("error2\n");
        return 1;
    }

    strcpy(temp,argv[4]);
    token_string = strtok(temp," ");

	to[op_cnt][0] = (char*)malloc(sizeof(char) * 100);
	strcpy(to[op_cnt][0], token_string);
	token_string = strtok(NULL, " >");
	strcpy(temp_to, token_string);
	while (1)
	{
		if (temp_to[strlen(temp_to) - 1] == '-')
		{
			temp_to[strlen(temp_to) - 1] = '\0';
			if (temp_to == NULL)
				break;
			to[op_cnt][nae_cnt] = (char*)malloc(sizeof(char) * 100);
			strcpy(to[op_cnt][nae_cnt++], temp_to);

			break;
		}
		if (temp_to[0] == '\'')
		{
			temp_to[0] = ' ';
			for (t = 1; temp_to[t] != '\0'; t++)
			{
				temp_to[t - 1] = temp_to[t];
			}
			t = t - 1;
			temp_to[t] = ' ';

			token_string = strtok(NULL, " >");
			strcat(temp_to, token_string);
			temp_to[(strlen(temp_to)) - 1] = '\0';

			to[op_cnt][nae_cnt] = (char*)malloc(sizeof(char) * 100);
			strcpy(to[op_cnt][nae_cnt++], temp_to);
			break;
		}
		else
		{
			to[op_cnt][nae_cnt] = (char*)malloc(sizeof(char) * 100);
			strcpy(to[op_cnt][nae_cnt++], temp_to);
		}
		token_string = strtok(NULL, " >");
		if (token_string == NULL || ((token_string[0] == '-')&&(strlen(token_string) == 1))) break;
		strcpy(temp_to, token_string);
	}
	if (strcmp(to[op_cnt][0], "grep") == 0)
	{
		to[op_cnt][nae_cnt] = (char*)malloc(sizeof(char) * 100);
		strcpy(to[op_cnt][nae_cnt], "-a");
		nae_cnt++;
	}

	op_cnt++;
	nae_cnt = 1;

	while (1)
	{
		token_string = strtok(NULL, " >");
		if (token_string == NULL)
			break;
		to[op_cnt][0] = (char*)malloc(sizeof(char) * 100);
		strcpy(to[op_cnt][0], token_string);

		token_string = strtok(NULL, " >");
		strcpy(temp_to, token_string);
		while (1)
		{
			if (temp_to[strlen(temp_to) - 1] == '-')
			{
				temp_to[strlen(temp_to) - 1] = '\0';
				if (temp_to == NULL)
					break;

				to[op_cnt][nae_cnt] = (char*)malloc(sizeof(char) * 100);
				strcpy(to[op_cnt][nae_cnt++], temp_to);
				break;
			}

			if (temp_to[0] == '\'')
			{
				temp_to[0] = ' ';
				for (t = 1; temp_to[t] != '\0'; t++)
				{
					temp_to[t - 1] = temp_to[t];
				}
				t = t - 1;
				temp_to[t] = ' ';

				token_string = strtok(NULL, " ->");
				strcat(temp_to, token_string);
				temp_to[(strlen(temp_to)) - 1] = '\0';
				to[op_cnt][nae_cnt] = (char*)malloc(sizeof(char) * 100);
				strcpy(to[op_cnt][nae_cnt++], temp_to);
				break;
			}
			else
			{
				to[op_cnt][nae_cnt] = (char*)malloc(sizeof(char) * 100);
				strcpy(to[op_cnt][nae_cnt++], temp_to);
			}
			token_string = strtok(NULL, " >");
			if (token_string == NULL || ((token_string[0] == '-') && (strlen(token_string) == 1)))
				break;
			strcpy(temp_to, token_string);
		}

		if (strcmp(to[op_cnt][0], "grep") == 0)
		{
			to[op_cnt][nae_cnt] = (char*)malloc(sizeof(char) * 100);
			strcpy(to[op_cnt][nae_cnt], "-a");
			nae_cnt++;
		}
		op_cnt++;
		nae_cnt = 1;
	}
    ta= malloc(sizeof(struct textarr)*(n+1));
    mutex = malloc(sizeof(pthread_mutex_t)*(n));
    thread_id = malloc(sizeof(int)*(n+1));
    pthread_t *p_thread = malloc(sizeof(pthread_t)*(n+1));

    FILE* fp;
    fp = fdopen(0,"r");

    for(i = 0 ; i < n+1; i ++)
    {
        ta[i].text[0] = '\0';
        ta[i].text_out[0] = '\0';
        ta[i].full = false;
        thread_id[i] = i;
        if(i != n)
        {
        pthread_mutex_init(&mutex[i],NULL); 
        pthread_create(&p_thread[i], NULL, thread_work, (void*)&thread_id[i]);
        }
    }   
    pthread_create(&p_thread[n], NULL, thread_work2, (void*)&thread_id[n]);
    pthread_mutex_init(&mutex_n,NULL);  

    while(!feof(fp))
    {
        while(ta[j].full);
      pthread_mutex_lock(&mutex[j]);  
        fgets(ta[j].text,max_size,fp);
//        printf("main %d - %s",j,ta[j].text);
      pthread_mutex_unlock(&mutex[j]);
        ta[j].full = true;
        j = (j+1)%n;
    }
 //   while(1);    
    flag_end = true;
    for(int i = 0 ; i < n; i ++)
    {
        pthread_join(p_thread[i],NULL);
        pthread_mutex_destroy (&mutex[i]);
    }
    flag_end_all = true;
    pthread_join(p_thread[n],NULL);
    pthread_mutex_destroy (&mutex_n);

    free(ta);
    free(mutex);
    free(thread_id);
    free(p_thread);
    sleep(1);
    return 0;
}