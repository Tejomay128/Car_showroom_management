#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define NUM_SH 3
#define T 2
typedef enum{PETROL,DIESEL} FuelType;
typedef enum{HATCHBACK,SEDAN,SUV} CarType;
typedef enum{FALSE,TRUE} boolean;
typedef enum{LOAN,CASH} PaymentType;

typedef struct Customer_tag
{
    char name[50];
    char mobNo[11];
    char addr[50];
    int soldVIN;
    int regNo;
    PaymentType pType;
    struct Customer_tag *left;
    struct Customer_tag *right;
    int ht;
    //loan details
    int months;
    float rate;
    float downPayment;
    float emi;
}Customer;

typedef struct Car_tag_data
{
    int salesmanID;
    int VIN;
    int regNo;
    char name[20];
    char color[10];
    int price;                                               
    FuelType fType;             
    CarType cType;
}Car_data;

typedef struct DLL_tag
{
    Car_data carData[5*T];
    int n;        //current no. of keys
    struct DLL_tag *next;
    struct DLL_tag *prev;
}DLL;

typedef struct Car_tag_node
{
    boolean isLeaf;
    int keys[2*T-1];
    int n;
    union
    {
        DLL *cars[2*T];
        struct Car_tag_node *C[2*T];
    }child;
}Car_node;

Car_node* initCarNode()
{
    Car_node *c = (Car_node*)malloc(sizeof(Car_node));
    c->n = 0;
    int i;
    for(i=0;i<2*T;i++)
    {
        c->child.cars[i] = (DLL*)malloc(sizeof(DLL));
        c->child.cars[i]->n = 0;
    }    
    return c;
}

Car_node* splitDataNode(Car_node *root,int i,Car_data c)
{
    int newkey;
    int div = 5*T/2;
    newkey = root->child.cars[i]->carData[div].VIN; 
    if(root->n == 2*T-1)
    {
        root = split(root,i,c);
    }
    else
    {
        int j = root->n;
        while (j>0 && root->keys[j-1] > newkey)
        {
            root->keys[j] = root->keys[j-1];
            j--;
        }
        root->keys[j] = newkey;
        int k = root->n + 1;
        while(k>j+1)
        {
            root->child.cars[k] = root->child.cars[k-1];
            k--;
        }
        for(j=0;j<div;j++)
        {
            root->child.cars[k]->carData[j] = root->child.cars[i]->carData[j+div];
        }
        root->child.cars[k]->n = root->child.cars[i]->n = div;
        root->n += 1;
        if(k!=0 && k!=root->n - 1)
        {
            root->child.cars[k]->prev = root->child.cars[k-1];
            root->child.cars[k]->next = root->child.cars[k+1];
            root->child.cars[k-1]->next = root->child.cars[k];
            root->child.cars[k+1]->prev = root->child.cars[k];
        }
        else if(k!=0)
        {
            root->child.cars[k]->prev = root->child.cars[k-1];
            root->child.cars[k]->next = NULL;
            root->child.cars[k-1]->next = root->child.cars[k];
        }
        else
        {
            root->child.cars[k]->prev = NULL;
            root->child.cars[k]->next = root->child.cars[k+1];
            root->child.cars[k+1]->prev = root->child.cars[k];
        }
        root->isLeaf = TRUE;
    }
    return root;
}

Car_node *insertCar(Car_node *root,Car_data c)
{
    if(root == NULL)
    {
        root = initCarNode();
        root->child.cars[0]->carData[0] = c;
        root->child.cars[0]->n = 1;
        root->n = 0;
        root->isLeaf = TRUE;
    }
    else 
    {
        int i;
        i=0;
        while(i<root->n && c.VIN > root->keys[i])
            i++;
        if(root->isLeaf == FALSE)
        {
            root->child.C[i] = insertCar(root->child.C[i],c);
        }
        else
        {
            if(root->child.cars[i]->n == 5*T)
            {
                root = splitDataNode(root,i,c);
            }
            else
            {
                int j = root->child.cars[i]->n;
                while(j>0 && root->child.cars[i]->carData[j-1].VIN > c.VIN)
                {
                    root->child.cars[i]->carData[j] = root->child.cars[i]->carData[j-1];
                    j--;
                }
                root->child.cars[i]->carData[j] = c;
            }
        }
        
    }
}

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
    Car_node *carData;
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

Customer* createNode(char name[],char mobNo[],char addr[],int VIN,int regNo,PaymentType pType)
{
    Customer *c = (Customer*)malloc(sizeof(Customer)); 
    strcpy(c->name,name);
    strcpy(c->mobNo,mobNo);
    strcpy(c->addr,addr);
    c->soldVIN = VIN;
    c->regNo = regNo;
    c->pType = pType;
    c->right = c->left = NULL;
    c->ht = 1;
    c->months = -1;
    c->rate = -1.0;
    c->downPayment = -1.0;
    c->emi = -1.0; 
    return c;
}

Customer* readCustomer()
{
    FILE *fp;
    Customer *root = NULL;
    fp = fopen("customer.txt","r");
    if(fp == NULL)
        printf("\nError in opening file\n");
    else
    {
        char name[50],mobNo[11],addr[50];
        int VIN,regNo;
        int pType;
        while(fscanf(fp,"%s %s %s %d %d %d",name,mobNo,addr,&VIN,&regNo,&pType) != EOF)
        {
            PaymentType p = pType;
            Customer *c = createNode(name,mobNo,addr,VIN,regNo,p);
            root = insert_c(root,c);
        }
    }
    fclose(fp);
    return root;
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

Salesman* createSalesman(int ID,char name[],char passwd[])
{
    Salesman *s = (Salesman*)malloc(sizeof(Salesman)); 
    strcpy(s->name,name);
    s->ID = ID;
    strcpy(s->passwd,passwd);
    s->salesAchieved = 0;
    s->salesTarget = 5000000;
    s->custData = NULL;
    s->right = s->left = NULL;
    s->ht = 1;
    return s;
}

Salesman* readSalesman()
{
    FILE *fp;
    Salesman *root = NULL;
    fp = fopen("salesman.txt","r");
    if(fp == NULL)
    {
        printf("\nError in opening file\n");
    }
    else
    {
        char name[50],passwd[50];
        int ID;
        while(fscanf(fp,"%d %s %s",&ID,name,passwd) != EOF);
        {
            Salesman *s = createSalesman(ID,name,passwd);
            root = insert_s(root,s);
        }
    }
    return root;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/



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
