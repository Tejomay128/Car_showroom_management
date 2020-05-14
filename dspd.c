#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define NUM_SH 3
#define T 2
#define NUM_CARS 9

typedef enum{PETROL,DIESEL} FuelType;
typedef enum{HATCHBACK=-1,SEDAN=0,SUV=1} CarType;
typedef enum{FALSE,TRUE} boolean;
typedef enum{LOAN,CASH} PaymentType;


typedef struct Customer_tag               //customer data along with his loan details(AVL tree)
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

typedef struct Car_tag_data                //car info(AVL tree)
{
    int salesmanID;
    int VIN;
    int regNo;
    char name[20];
    char color[10];
    int price;                                               
    FuelType fType;             
    CarType cType;
    struct Car_tag_data *left;
    struct Car_tag_data *right;
    int ht;
}Car_node;

typedef struct Salesman_tag              //salesman info(AVL tree)  
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


typedef struct Showroom_tag            //showroom info 
{
    Car_node *carData;
    Salesman *salesmanData;
    int soldCars;
    int availableCars;
}Showroom;

typedef struct PopCar                  //data of cars along with their nos. sold
{
    char name[50];
    int no;
}PopCar;

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

void displaySalesman(Salesman *s)
{
    printf("ID: %d\nName: %s\nSales Achieved: %d\n",s->ID,s->name,s->salesAchieved);
}

Car_node* searchCar(Car_node *root,int VIN)              //given car database and VIN, it returns pointer to car data corresponding to the VIN 
{
    Car_node *retval = NULL;
    if(root!=NULL)
    {
        if(VIN > root->VIN)
            retval = searchCar(root->right,VIN);
        else if(VIN < root->VIN)
            retval = searchCar(root->left,VIN);
        else
            retval = root;
    }        
    return retval;            
}

void readSalesman(Showroom sh[],int size)                //reads salesman data from a file and distributes it in three showrooms
{
    FILE *fp;
    fp = fopen("salesman.txt","r");
    if(fp == NULL)
        printf("\nError opening file!");
    else
    {
        int i = 0;
        int index = 0;
        int ID;
        char name[50],passwd[50];
        while(fscanf(fp,"%d %s %s",&ID,name,passwd)!=EOF)
        {
            if(i==4 || i==8)                              //initializing 4 salesman every showroom
                index++;
            Salesman *s = createSalesman(ID,name,passwd);
            sh[index].salesmanData = insert_s(sh[index].salesmanData,s);
            i++;
        }
    }
    fclose(fp);
}

void fillLoanDetails(Customer *c,Showroom showroom,int count,Salesman *s,float *mon)  //utility function loan detail initializer with the first type
{
    Car_node *car = searchCar(showroom.carData,c->soldVIN);
    if(count%36 == 0)
    {
        c->downPayment = (float)car->price * 0.2;
        c->months = 84;
        c->rate = 9.0;
        float rem = (float)car->price * 0.8;
        c->emi = (rem + (c->rate * rem))/((float)c->months);
    }
    car->salesmanID = s->ID;
    s->salesAchieved += car->price;
    (*mon) += (float)car->price;
}

Salesman* fillCustomer(Showroom showroom,Salesman *root,FILE **fp,float *mon)    //utility function to fill customer data with respective salesmen  
{
    static int count = 0;
    int i,VIN,regNo,pay;
    char name[50],mobNo[11],addr[50];
    PaymentType p;
    if(root!=NULL)
    {
        root->left = fillCustomer(showroom,root->left,fp,mon);
        // Customer *cust = NULL;
        for(i=0;i<NUM_SH;i++)
        {
            fscanf(*fp,"%s %s %s %d %d %d",name,mobNo,addr,&VIN,&regNo,&pay);
            p = pay;
            Customer *c = createNode(name,mobNo,addr,VIN,regNo,p);
                fillLoanDetails(c,showroom,count,root,mon);
            root->custData = insert_c(root->custData,c);
            count++;
        }
        root->right = fillCustomer(showroom,root->right,fp,mon);
    }
    return root;
}

void readCustomer(Showroom *sh,int size,float month[])        //reads a file, creates salesmen in showrooms as well as fills them with customers using utility functions
{
    FILE *fp;
    float mon;
    fp = fopen("customer.txt","r");
    if(fp == NULL)
        printf("\nError in opening file\n");
    else
    {
        int cust_count = 3,i = 0,j;
        for(j=0;j<NUM_SH;j++)
        {
            mon = 0.0;
            for(i=0;i<size;i++)
            {
                sh[i].salesmanData = fillCustomer(sh[i],sh[i].salesmanData,&fp,&mon);
            }
            month[j] = mon;
        }
    }   
    fclose(fp);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/

int height_Car(Car_node *root) 
{ 
    int retval;
    if (root == NULL) 
        retval = 0;
    else     
        retval = root->ht;
    return retval;     
}

Car_node* rightRotate_Car(Car_node *p)
{
    Car_node *q;
    if(p!=NULL && (p->left != NULL))
    {
        q = p->left;
        p->left = q->right;
        q->right = p;
    }
    p->ht = max(height_Car(p->left), height_Car(p->right))+1; 
    q->ht = max(height_Car(q->left), height_Car(q->right))+1;
    return q;
}

Car_node* leftRotate_Car(Car_node *p)
{
    Car_node *q;
    if(p!=NULL && (p->right != NULL))
    {
        q = p->right;
        p->right = q->left;
        q->left = p;
    }
    p->ht = max(height_Car(p->left), height_Car(p->right))+1;
    q->ht = max(height_Car(q->left), height_Car(q->right))+1;
    return q;
}

int getBalance_Car(Car_node *N) 
{ 
    if (N == NULL) 
        return 0; 
    return (height_Car(N->left) - height_Car(N->right)); 
}

Car_node* insert_Car(Car_node *root, Car_node *c)
{
    Car_node *retval;
    int bal;
    if(root == NULL)
        retval = c;
	else
	{
		
	    if(c->VIN < root->VIN)
	        root->left = insert_Car(root->left,c);
	    else if(c->VIN > root->VIN)
	        root->right = insert_Car(root->right,c);
	    else 
	        retval = root;
	    
		root->ht = 1 + max(height_Car(root->left),height_Car(root->right));    
	
	    bal = getBalance_Car(root);
	
	    if(bal>1 && (c->VIN < root->left->VIN))
	        retval = rightRotate_Car(root);
	    else if(bal<-1 && (c->VIN > root->right->VIN))
	    	retval = leftRotate_Car(root);
	    else if(bal>1 && (c->VIN > root->left->VIN))  
	    {
	        root->left = leftRotate_Car(root->left);
	        retval = rightRotate_Car(root);
	    }     
	    else if(bal<-1 && (c->VIN < root->left->VIN))
	    {
	        root->right = rightRotate_Car(root->right);
	        retval = leftRotate_Car(root);
	    }
        else 
        {
            retval = root;
        }
	}
    return retval;
}

Car_node* createCar(int ID,int VIN,int regNo,char name[],char color[],int price,FuelType f,CarType c)
{
    Car_node *car = (Car_node*)malloc(sizeof(Car_node));
    car->salesmanID = ID;
    car->VIN = VIN;
    car->regNo = regNo;
    strcpy(car->name,name);
    strcpy(car->color,color);
    car->price = price;
    car->fType = f;
    car->cType = c;
    car->right = car->left = NULL;
    car->ht = 1;
    return car;
}

Salesman* searchSalesman(Salesman *root,int ID)      //searches a salesman with the given ID 
{
    Salesman *retval = NULL;
    if(root!=NULL)
    {
        if(ID > root->ID)
            retval = searchSalesman(root->right,ID);
        else if(ID < root->ID)
            retval = searchSalesman(root->left,ID);
        else
            retval = root;
    }        
    return retval;
}

Customer* searchCust(Customer *root,int VIN)      //searches a customer with a given VIN of the car sold to that customer
{
    Customer *retval = NULL;
    if(root!=NULL)
    {
        if(VIN > root->soldVIN)
            retval = searchCust(root->right,VIN);
        else if(VIN < root->soldVIN)
            retval = searchCust(root->left,VIN);
        else
            retval = root;
    }        
    return retval;
}

void displayCar(Car_node *c,Showroom sh)
{
    printf("Name: %s\nVIN: %d\nColor: %s\nPrice: %d\n",c->name,c->VIN,c->color,c->price);
    if(c->salesmanID != -1)
    {
        printf("ID: %d\n",c->salesmanID);
        Salesman *s = searchSalesman(sh.salesmanData,c->salesmanID);
        Customer *cust = searchCust(s->custData,c->VIN);
        printf("Salesman Name :%s\nCustomer Name:%s",s->name,cust->name);
    }
    printf("\n-------------------------------\n");
}

void readCar(Showroom sh[],int size)          // reads car data from a file and fills them up in showrooms
{
    FILE *fp;
    fp = fopen("car.txt","r");
    if(fp == NULL)
        printf("Error!!");
    else
    {
        int count = 0,i = 0;
        int ID,VIN,regNo,price,ctype,ftype;
        char name[20],color[10];
        FuelType f;
        CarType c;
        while(fscanf(fp,"%d %d %d %s %s %d %d %d",&ID,&VIN,&regNo,name,color,&price,&ctype,&ftype) != EOF)
        {
            if(count == 12 || count == 24 || count == 36 || count == 48 || count == 60 || count == 72 || count == 84 || count == 96)
            {
                i++;
                i = i%3;
            }    
            f = ftype;
            c = ctype;
            Car_node *temp = createCar(ID,VIN,regNo,name,color,price,f,c);
            sh[i].carData = insert_Car(sh[i].carData,temp);
            sh[i].soldCars += 1;
            count++;
        }
    }   
    fclose(fp);
}

void readAVCar(Showroom sh[],int size)         //reads available cars from a file and fills them up in showrooms
{
    FILE *fp;
    fp = fopen("AVcar.txt","r");
    if(fp == NULL)
        printf("Error!!");
    else
    {
        int count = 0,i = 0;
        int ID,VIN,regNo,price,ctype,ftype;
        char name[20],color[10];
        FuelType f;
        CarType c;
        while(fscanf(fp,"%d %d %d %s %s %d %d %d",&ID,&VIN,&regNo,name,color,&price,&ctype,&ftype) != EOF)
        {
            if(count == 6 || count == 12)
                i++;
            f = ftype;
            c = ctype;
            Car_node *temp = createCar(ID,VIN,regNo,name,color,price,f,c);
            sh[i].carData = insert_Car(sh[i].carData,temp);
            sh[i].availableCars += 1;
            count++;
        }
    }   
    fclose(fp);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/

Salesman* LoginSalesman(Showroom sh,int size,int ID,char passwd[])    //Finds the salesman and verifies its login credentials
{
    Salesman *retval = NULL;
    retval = searchSalesman(sh.salesmanData, ID);
    if(retval!=NULL)
    {
        if(strcmp(passwd,retval->passwd) != 0)
        retval = NULL;
    }
    return retval;
}

void addSalesman(Showroom sh)              //que B
{
    Salesman *s;
    printf("\nEnter salesman data:-");
    char name[50],passwd[50];
    int ID;
    printf("\nName:");
    scanf("%s",name);
    printf("\nID:");
    scanf("%d",&ID);
    printf("\nPassword:");
    scanf("%s",passwd);
    s = createSalesman(ID,name,passwd);
    sh.salesmanData = insert_s(sh.salesmanData,s);
    printf("\nAdded Successfully!!!");
}

void createPopCarDatabase(PopCar p[],int size)        //initializes the popular car data 
{
    int i,count;
    char ch[9][20] = {"Swift","Alto","Ritz","Dzire","Innova","Hexa","Civic","Santro","Tavera"};
    for(i=0;i<size;i++)
    {
        strcpy(p[i].name,ch[i]);
        p[i].no = 12;
    }
}

void showPopCar(PopCar p[],int size)                 //que C
{
    int i,max;
    max = p[0].no;
    for(i=1;i<size;i++)
    {
        if(max<p[i].no)
            max = p[i].no;        
    }
    printf("\nMost popular car(s):");
    for(i=0;i<size;i++)
    {
        if(p[i].no == max)
            printf("%s\n",p[i].name);
    }
}

void maxValue(Salesman *root,int *max,Salesman **s)   //utility function for finding the most successful salesperson
{
    if(root!=NULL)
    {
        maxValue(root->right,max,s);
        if((*max) < root->salesAchieved)
        {
            *s = root; 
            *max = root->salesAchieved;
        }
        maxValue(root->left,max,s);
    }
}

void mostSuccSalesman(Showroom sh[],int size)        //Que D
{
    int i,max = 0;
    Salesman *s;
    for(i=0;i<size;i++)
    {
        maxValue(sh[i].salesmanData,&max,&s);
    }
    printf("Most Successful Salesman is: \n");
    displaySalesman(s);
    printf("Total commission achieved along with additional 1%% incentive is : %f",0.03 * (float)s->salesAchieved);
}

void sell(Showroom sh,Salesman *s,float *month,PopCar popCars[])    // Que E
{
    char name[50],mobNo[11],addr[50];
    int VIN,regNo,p;
    PaymentType pType;
    printf("\nEnter customer details:");
    printf("\nName: ");
    scanf("%s",name);
    printf("\nMobNo: ");
    scanf("%s",mobNo);
    printf("\nAddr: ");
    scanf("%s",addr);
    printf("\nVIN: ");
    scanf("%d",&VIN);
    printf("\nPayment Type(type 0 for loan, 1 for cash): ");
    do
    {
        scanf("%d",&p);
        if(p!=0 && p!=1)
            printf("\nWrong choice, enter again:");
    }while(p!=0 && p!=1);
    pType = p;
    Customer *c = createNode(name,mobNo,addr,VIN,regNo,pType);
    Car_node *car = searchCar(sh.carData,VIN);
    if(car == NULL)
        printf("\nNo such car found!!!");
    else if(car->salesmanID != -1)
        printf("\nCar sold out!!!");    
    else
    {
        c->regNo = car->regNo = 1000+VIN;
        sh.soldCars += 1;
        sh.availableCars -= 1;
        car->salesmanID = s->ID;
        if(pType == LOAN)
        {
            int choice;
            printf("\n1)9.00%% rate of interest for 84 months EMI.\n2)8.75%% rate of interest for 60 months EMI.\n3)8.50%% rate of interest for 36 months EMI.");
            printf("\nEnter Loan choice(1,2,3): ");
            do
            {
                scanf("%d",&choice);
                if(choice!=1 && choice!=2 && choice!=3)
                    printf("\nWrong Choice,enter again: ");
            }while(choice!=1 && choice!=2 && choice!=3);    
            if(choice == 1)
            {
                c->rate = 9.0;
                c->downPayment = 0.2 * (float)car->price;
                c->months = 84;
                float rem = (float)car->price * 0.8;
                c->emi = (rem + (c->rate * rem))/((float)c->months);
            }
            else if(choice == 2)
            {
                c->rate = 8.75;
                c->downPayment = 0.2 * (float)car->price;
                c->months = 60;
                float rem = (float)car->price * 0.8;
                c->emi = (rem + (c->rate * rem))/((float)c->months);
            }
            else
            {
                c->rate = 8.5;
                c->downPayment = 0.2 * (float)car->price;
                c->months = 36;
                float rem = (float)car->price * 0.8;
                c->emi = (rem + (c->rate * rem))/((float)c->months);
            }
        }    
        s->custData = insert_c(s->custData,c);
        s->salesAchieved += car->price;
        *month += (float)car->price;
        printf("\nCar sold Successfully;\nCar details:\n");

        int i,found=0;
         for(i=0;i<NUM_CARS && !found;i++)
         {
             if(strcmp(popCars[i].name,car->name)==0)
             {
                //  printf("\n%s",popCars[i].name);
                 found = 1;
                 popCars[i].no += 1;
             }
        }

        displayCar(car,sh);
    }    
}

void predict(float month[],int size)    //Que F
{
    float slope[size-1],avgSlope = 0.0;
    int i;
    for(i=0;i<size-1;i++)
    {
        slope[i] = month[i+1]-month[i];
        avgSlope += slope[i];
    }
    avgSlope /= (size-1);
    printf("Predicted Sales for the next month: %.2f\n",month[size-1]+avgSlope); 
}

void DisplayCarInfo(Showroom sh,int VIN)        //Que G
{
    Car_node *car = searchCar(sh.carData,VIN);
    if(car == NULL)
    {
        printf("\nNo such car found!!!");
    }
    else
    {
        displayCar(car,sh);
    }
}

void RangeSearchUtil(Showroom sh, Car_node *c, int l, int r)    //recursive utility function, range search traversal
{
    if(c!=NULL)
    {
        RangeSearchUtil(sh,c->left,l,r);
        if(c->VIN>=l && c->VIN<=r)
            displayCar(c,sh);
        RangeSearchUtil(sh,c->right,l,r);
    }    
}

void RangeSearch(Showroom sh[],int l,int r)     //Que H
{
    int i;
    for(i=0;i<NUM_SH;i++)
        RangeSearchUtil(sh[i],sh[i].carData,l,r);
}

void preOrder_cust(Customer *root) 
{
    if(root != NULL) 
    { 
         
        preOrder_cust(root->left); 
        printf("%s ",root->name);
        preOrder_cust(root->right); 
    } 
}

void preOrder_c(Car_node *root) 
{
    if(root != NULL) 
    { 
        
        preOrder_c(root->left);
        printf("%d ",root->VIN); 
        preOrder_c(root->right); 
    } 
} 

void preOrder_s(Salesman *root) 
{
    if(root != NULL) 
    { 
         
        printf("\nID:%d ",root->ID);
        
        preOrder_s(root->left);
        preOrder_cust(root->custData); 
        preOrder_s(root->right); 
    } 
}

int main()
{
    int shID,i;
    Showroom sh[NUM_SH];
    float month[NUM_SH];
    PopCar popCars[NUM_CARS];

    for(i=0;i<NUM_SH;i++)
    {
        sh[i].soldCars = 36;
        sh[i].availableCars = 0;
        sh[i].salesmanData = NULL;
        sh[i].carData = NULL;
    }

    readCar(sh,NUM_SH);
    readSalesman(sh,NUM_SH);
    readAVCar(sh,NUM_SH);
    readCustomer(sh,NUM_SH,month);
    createPopCarDatabase(popCars,NUM_CARS);

    int ID;
    char passwd[50];
    printf("\n-------------------------------------------WELCOME--------------------------------------------------\n");
    printf("Enter showroom no.(1,2,3): ");
    do
    {
        scanf("%d",&shID);
        if(shID!=1 && shID!=2 && shID!=3)
        {
            printf("\nWrong ID, please enter again: ");
        }    
    }while(shID!=1 && shID!=2 && shID!=3);
    shID--;

    Salesman *s;
    printf("Salesman Login:\n");
    do
    {
        printf("ID: ");
        scanf("%d",&ID);
        printf("\nPassword: ");
        scanf("%s",passwd);
        s = LoginSalesman(sh[shID],NUM_SH,ID,passwd);
        if(s == NULL)
            printf("Incorrect ID or Password!!\nEnter Again\n");
    }while(s == NULL);

    char ch;
    do
    {
        int choice;
        printf("\n1)Add Salesperson\n2)Sell car\n3)Display most popular car\n4)Predict next month sales\n5)Display car info\n6)Perform range search\n7)Display most successful salesperson\n");
        printf("Enter Choice: ");
        scanf("%d",&choice);
        printf("\n");
        switch (choice)
        {
            case 1:
                {
                    addSalesman(sh[shID]);
                    break;
                }

            case 2:
                {
                    sell(sh[shID],s,&month[NUM_SH-1],popCars);
                    break;
                }    
            
            case 3:
                {
                    showPopCar(popCars,NUM_CARS);
                    break;
                }    

            case 4:
                {
                    predict(month,NUM_SH);
                    break;    
                }
                    
            case 5:
                {
                    int VIN;
                    printf("\nEnter VIN of the car to be searched: ");
                    scanf("%d",&VIN);
                    DisplayCarInfo(sh[shID],VIN);
                    break;
                }        
            
            case 6:
                {
                    int l,r;
                    printf("\nEnter Lower bound :");
                    scanf("%d",&l);
                    printf("\nEnter upper bound :");
                    scanf("%d",&r);
                    printf("\n");
                    int i;
                    RangeSearch(sh,l,r);
                    break;
                }    
            
            case 7:
                {
                    mostSuccSalesman(sh,NUM_SH);
                    break;
                }

            default:
                {
                    printf("\nWrong choice!!!");
                    break;
                }    
        }
        printf("\nDo you wish to continue(Y/N): ");
        while ((getchar()) != '\n'); 	
        scanf("%c",&ch);
    }while(ch=='y' || ch=='Y');    
    return 0;
}
