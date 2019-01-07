#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<stdbool.h>
#include <string.h>

#define FILE_USER "/home/ankit/Desktop/Term1/System Software/mini_project/users.txt"

struct user{
	char name[40];
	char user_name[20];
	char pass[20];
	int account_no;
	int balance;
	bool isactive;
	int type;    //0-normal,1-joint,2-admin
};
struct trans{
	int account_no;
	char trans_type[20];
	int old_balance;
	int new_balance;
};
int create_new_users(char *name,char *username,char *pass,int balance,int type,int account_no)
{
	struct user new_user;
	//int user_file;
	//int account_no = 0;
	strcpy(new_user.name,name);
	strcpy(new_user.user_name,username);
	strcpy(new_user.pass,pass);
	new_user.account_no = account_no;
	new_user.balance = balance;
	new_user.isactive = true;
	new_user.type = type;

	int user_file;
	user_file = open(FILE_USER,O_CREAT|O_WRONLY|O_APPEND,0744);
	if(user_file<2)
	{
		perror("error in file opening");
		close(user_file);
		return -1;
	}
	int seek_val = lseek(user_file,0,SEEK_END);
	if((write(user_file,&new_user,sizeof(struct user)))<=0)
	{
		printf("write error");
		close(user_file);
		return -1;
	}
	//fseek(user_file,0,SEEK_SET);
	close(user_file);
	return 0;
}

bool check_username_exist(char *user_name)
{
	int user_file;
	struct user user_entry;
	user_file = open(FILE_USER,O_RDONLY,0744);
	if(user_file<2)
	{
		perror("error in file opening please enter again\n");
		close(user_file);
		return true;
	}
	//lseek(user_file,0,SEEK_SET);
	//printf("%d",(read(user_file,user_entry.user_name,sizeof(struct user))));
	while((read(user_file,&user_entry,sizeof(struct user)))>0)
	{
		//printf("inside loop");
		if((strcmp(user_name,user_entry.user_name))==0 && user_entry.isactive)
		{
			printf("Username already exist,enter the diffrent user_name\n");
			close(user_file);
			return true;
		}
	}
	//printf("its working");
	close(user_file);
	return false;
}

bool modify_user(char *name,char *user_name,char *pass,int account_no)
{
	int user_file;
	int size;
	size = sizeof(struct user);
	struct user *user_entry = (struct user *)malloc(size);
	user_file = open(FILE_USER,O_RDWR,0744);
	if(user_file<2)
	{
		printf("error in file opening\n");
		return false;
	}
	while((read(user_file,user_entry,sizeof(struct user)))>0)
	{
		//printf("inside loop");
		if(user_entry->account_no == account_no && user_entry->isactive)
		{
			strcpy(user_entry->name,name);
			strcpy(user_entry->user_name,user_name);
			strcpy(user_entry->pass,pass);
			lseek(user_file,-size,SEEK_CUR);
			write(user_file,user_entry,size);
			return true;
		}

	}
	return false;
}

struct user *search_user(int account_no)
{
	int user_file;
	int size;
	size = sizeof(struct user);
	//struct user user_entry;
	struct user *user_entry = (struct user *)malloc(size);
	user_file = open(FILE_USER,O_RDONLY,0744);
	if(user_file<2)
	{
		//perror("error in file opening please enter again\n");
		printf("Error in file opening\n");
		return NULL;
	}
	//printf("%d\n",(read(user_file,&user_entry,sizeof(struct user))));
	while((read(user_file,user_entry,sizeof(struct user)))>0)
	{
		if(user_entry->account_no == account_no && user_entry->isactive)
		{
			//*user_entry2 = user_entry;
			return user_entry;
		}
	}
	printf("Account no. not found\n");
	return NULL;
}

bool delete_user(int account_no)
{
	struct user *del_user;
	int user_file;
	int size;
	size = sizeof(struct user);
	struct user *user_entry = (struct user *)malloc(size);
	user_file = open(FILE_USER,O_RDWR,0744);
	del_user = search_user(account_no);
	if(del_user==NULL || del_user->isactive == false)
	{
		return false;
	}
	del_user->isactive = false;
	while((read(user_file,user_entry,sizeof(struct user)))>0)
	{
		//printf("inside loop");
		if(user_entry->account_no == del_user->account_no && user_entry->isactive)
		{
			lseek(user_file,-size,SEEK_CUR);
			write(user_file,del_user,size);
			return true;
		}
	}
	return false;

}

char* print_user_details(int account_no)
{
	struct user *srch_user = search_user(account_no);
	char *c = (char*)malloc(1024);
    char acnt[50],bal[20];
    if(srch_user==NULL)
    	strcat(c,"Error in finding the user");
    else
    {
		if(srch_user->isactive==true)
		{
	        strcat(c,"Name: ");
			strcat(c,srch_user->name);
			strcat(c,"\nUsername: ");
			strcat(c,srch_user->user_name);
			strcat(c,"\nAccount No.: ");
			sprintf(acnt,"%d",srch_user->account_no);
			strcat(c,acnt);
			strcat(c,"\nBalance: ");
			sprintf(bal,"%d",srch_user->balance);
			strcat(c,bal);
			if(srch_user->type==0)
			{
				strcat(c,"\nAccount type is normal");
			}
			if(srch_user->type==1)
			{
				strcat(c,"\nAccount type is joint");
			}
			strcat(c,"\n");
			//printf("Account No.: %d\n", srch_user->account_no);
			//printf("Balance: %d\n", srch_user->balance);
			//printf("pass: %s\n",srch_user->pass);
		}
		else
			//printf("User doesn't exist\n");
			strcat(c,"User doesn't exist");
	}
    return c;
}
