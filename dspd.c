#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define NUM_SH 3
typedef enum{PETROL,DIESEL} FuelType;
typedef enum{HATCHBACK,SEDAN,SUV} CarType;

typedef struct Customer_tag
{
    char name[50];
    char mobNo[11];
    char addr[50];
    int soldVIN;
    int regNo;
    struct Customer_tag *left;
    struct Customer_tag *right;
    int ht;
}Customer;

typedef struct Car_tag
{
    int VIN;
    char name[20];
    char color[10];
    int price;
    FuelType fType;
    CarType cType;
    struct Car_tag *left;
    struct Car_tag *right;
}Car;

typedef struct Salesman_tag
{
    int ID;
    char passwd[50];
    char name[50];
    int salesTarget;
    int salesAchieved;
    Customer *custData;
    struct Salesman_tag *left;
    struct Salesman_tag *right;
    int ht;
}Salesman;


typedef struct Showroom_tag
{
    Car *carData;
    Salesman *salesmanData;
    int soldCars;
    int availableCars;
}Showroom;

int max(int a,int b)
{
    return (a>b)?a:b;
} 

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/

int height_c(Customer *root) 
{ 
    int retval;
    if (root == NULL) 
        retval = 0;
    else     
        retval = root->ht;
    return retval;     
}

Customer* createNode(char name[],char mobNo[],char addr[],int VIN,int regNo)
{
    Customer *c = (Customer*)malloc(sizeof(Customer)); 
    strcpy(c->name,name);
    strcpy(c->mobNo,mobNo);
    strcpy(c->addr,addr);
    c->soldVIN = VIN;
    c->regNo = regNo;
    c->right = c->left = NULL;
    c->ht = 1;
    return c;
}

Customer* rightRotate_c(Customer *p)
{
    Customer *q;
    if(p!=NULL && (p->left != NULL))
    {
        q = p->left;
        p->left = q->right;
        q->right = p;
    }
    p->ht = max(height_c(p->left), height_c(p->right))+1; 
    q->ht = max(height_c(q->left), height_c(q->right))+1;
    return q;
}

Customer* leftRotate_c(Customer *p)
{
    Customer *q;
    if(p!=NULL && (p->right != NULL))
    {
        q = p->right;
        p->right = q->left;
        q->left = p;
    }
    p->ht = max(height_c(p->left), height_c(p->right))+1;
    q->ht = max(height_c(q->left), height_c(q->right))+1;
    return q;
}

int getBalance_c(Customer *N) 
{ 
    if (N == NULL) 
        return 0; 
    return (height_c(N->left) - height_c(N->right)); 
}

Customer* insert_c(Customer *root, Customer *c)
{
    Customer *retval;
    int bal;
    if(root == NULL)
        retval = c;
	else
	{
		
	    if(c->soldVIN < root->soldVIN)
	        root->left = insert_c(root->left,c);
	    else if(c->soldVIN > root->soldVIN)
	        root->right = insert_c(root->right,c);
	    else 
	        retval = root;
	    
		root->ht = 1 + max(height_c(root->left),height_c(root->right));    
	
	    bal = getBalance_c(root);
	
	    if(bal>1 && (c->soldVIN < root->left->soldVIN))
	        retval = rightRotate_c(root);
	    else if(bal<-1 && (c->soldVIN > root->right->soldVIN))
	    	retval = leftRotate_c(root);
	    else if(bal>1 && (c->soldVIN > root->left->soldVIN))  
	    {
	        root->left = leftRotate_c(root->left);
	        retval = rightRotate_c(root);
	    }     
	    else if(bal<-1 && (c->soldVIN < root->left->soldVIN))
	    {
	        root->right = rightRotate_c(root->right);
	        retval = leftRotate_c(root);
	    }
        else 
        {
            retval = root;
        }
	}
    return retval;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/

int height_s(Salesman *root) 
{ 
    int retval;
    if (root == NULL) 
        retval = 0;
    else     
        retval = root->ht;
    return retval;     
}

Salesman* createSalesman(int ID,char name[])
{
    Salesman *s = (Salesman*)malloc(sizeof(Salesman)); 
    strcpy(s->name,name);
    s->ID = ID;
    s->salesAchieved = 0;
    s->salesTarget = 5000000;
    s->custData = NULL;
    s->right = s->left = NULL;
    s->ht = 1;
    return s;
}

Salesman* rightRotate_s(Salesman *p)
{
    Salesman *q;
    if(p!=NULL && (p->left != NULL))
    {
        q = p->left;
        p->left = q->right;
        q->right = p;
    }
    p->ht = max(height_s(p->left), height_s(p->right))+1; 
    q->ht = max(height_s(q->left), height_s(q->right))+1;
    return q;
}

Salesman* leftRotate_s(Salesman *p)
{
    Salesman *q;
    if(p!=NULL && (p->right != NULL))
    {
        q = p->right;
        p->right = q->left;
        q->left = p;
    }
    p->ht = max(height_s(p->left), height_s(p->right))+1;
    q->ht = max(height_s(q->left), height_s(q->right))+1;
    return q;
}

int getBalance_s(Salesman *N) 
{ 
    if (N == NULL) 
        return 0; 
    return (height_s(N->left) - height_s(N->right)); 
}

Salesman* insert_s(Salesman *root, Salesman *c)
{
    Salesman *retval;
    int bal;
    if(root == NULL)
        retval = c;
	else
	{
		
	    if(c->ID < root->ID)
	        root->left = insert_s(root->left,c);
	    else if(c->ID > root->ID)
	        root->right = insert_s(root->right,c);
	    else 
	        retval = root;
	    
		root->ht = 1 + max(height_s(root->left),height_s(root->right));    
	
	    bal = getBalance_s(root);
	
	    if(bal>1 && (c->ID < root->left->ID))
	        retval = rightRotate_s(root);
	    else if(bal<-1 && (c->ID > root->right->ID))
	    	retval = leftRotate_s(root);
	    else if(bal>1 && (c->ID > root->left->ID))  
	    {
	        root->left = leftRotate_s(root->left);
	        retval = rightRotate_s(root);
	    }     
	    else if(bal<-1 && (c->ID < root->left->ID))
	    {
	        root->right = rightRotate_s(root->right);
	        retval = leftRotate_s(root);
	    }
        else 
        {
            retval = root;
        }
	}
    return retval;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/


void preOrder(Salesman *root) 
{
    if(root != NULL) 
    { 
        printf("%d ", root->ID); 
        preOrder(root->left); 
        preOrder(root->right); 
    } 
} 

int main()
{
    int shID;
    Showroom sh[NUM_SH];
    char yn;
    int ch;
    printf("\n----------------------WELCOME TO CAR SHOWROOMS MANAGEMENT--------------------\n");
    do
    {
        printf("Enter showroom ID:(1,2,3): ");
        scanf("%d",&shID);
        if(shID!=1 && shID!=2 && shID!=3)
            printf("\nNOT A VALID SHOWRROM!");
    }while(shID!=1 && shID!=2 && shID!=3);
    do
    {
        printf("1.Salesman\n2.Customer");
        printf("\nEnter Your choice:(1,2): ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
            {
                int salesID;
                char passwd[50];
                printf("\nEnter Your Login Details:\nLogin ID: ");
                scanf("%d",&salesID);
                printf("\nPassword: ");
                scanf("%s",passwd);
                
                break;
            }
            case 2:
            {
                printf("\n2");
                break;
            }
            default: 
            {
                printf("\nYou entered wrong choice.");
            }
        }    
        printf("\nDo you wish to choose again?(Y/N): ");
        while((getchar())!='\n');
        scanf("%c",&yn);
    } while (yn=='y' || yn=='Y');
    
    return 0;
}
