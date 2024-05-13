#include <stdio.h>
#include <stdlib.h>
#include<windows.h>



//////////        helpful functions              //////////////////////////////////////

COORD coord= {0,0};                  // this is global variable
//center of axis is set to the top left cornor of the screen
void gotoxy(int x,int y)
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}







/////////       program’s structs     ////////////////////////////////////////////////

typedef struct product product;
struct product
{
    char name [20];
    int price ;
    int quantity ;
};
typedef struct category category;
struct category
{
    char name [20];
    product *myProducts;
    int myProductsLength;
    int myProductsSize;
};


////////     main function    /////////////////////////////////////////////////////////

int main()
{

///////////      declaration      //////////////////////////////////////////////////////

    int mainStoreLength=0;
    int mainStoreSize=30;
    category *mainStore = malloc(mainStoreSize*sizeof(category));
    int axisY =0;
    int menuLength = 6;
    char menu [][20] = {"Display","Create category","Create product","Move product","sell","exit"};
    int open=1;
    char userInput;



///////////        program’s  functions     ////////////////////////////////////////////////

    product* ResizeProduct(product *prd1, int oldSize)
    {
        int newSize =oldSize*2;
        product *newProd = malloc(newSize * sizeof(product));
        for(int i = 0; i < oldSize; i++)
        {
            newProd[i] = prd1[i];
        }
        free(prd1);
        return newProd;
    }

    int isCategryFound(char userCategory1 [20])
    {
        int f=0;
        for(int i=0; i<mainStoreLength; i++)
        {
            if( strcmp(mainStore[i].name,userCategory1)==0)
            {
                f++;
            }
        }
        return f;
    }

    int isProductFound (char userCategory1 [20],char userproduct1 [20])
    {
        int f=0;
        for(int i=0; i<mainStoreLength; i++)
        {
            if( strcmp(mainStore[i].name,userCategory1)==0)
            {
                for(int j=0; j<mainStore[i].myProductsLength; j++)
                {
                    if( strcmp(mainStore[i].myProducts[j].name,userproduct1)==0)
                    {
                        f++;
                    }
                }
            }
        }
        return f;
    }

    void removeProduct (char userCategory1 [20],char userproduct1 [20])
    {
        int categoryIndex;
        int deleteIndex;
        for(int i=0; i<mainStoreLength; i++)
        {
            if( strcmp(mainStore[i].name,userCategory1)==0)
            {
                for(int j=0; j<mainStore[i].myProductsLength; j++)
                {
                    if( strcmp(mainStore[i].myProducts[j].name,userproduct1)==0)
                    {
                        categoryIndex=i;
                        deleteIndex=j;
                    }
                }
            }
        }
        for (deleteIndex ; deleteIndex<mainStore[categoryIndex].myProductsLength ; deleteIndex++)
        {
            mainStore[categoryIndex].myProducts[deleteIndex]=mainStore[categoryIndex].myProducts[deleteIndex+1];
        }
        mainStore[categoryIndex].myProductsLength--;
    }


    void addProduct( char userCategory1 [20],char userCategory2 [20],char userproduct1 [20])
    {
        product p;
        for(int i=0; i<mainStoreLength; i++)
        {
            if( strcmp(mainStore[i].name,userCategory1)==0)
            {
                for(int j=0; j<mainStore[i].myProductsLength; j++)
                {
                    if( strcmp(mainStore[i].myProducts[j].name,userproduct1)==0)
                    {
                        p=mainStore[i].myProducts[j];
                    }
                }
            }
        }
        for(int i=0; i<mainStoreLength; i++)
        {
            if( strcmp(mainStore[i].name,userCategory2)==0)
            {
                mainStore[i].myProducts[mainStore[i].myProductsLength]=p;
                mainStore[i].myProductsLength++;
            }
        }
    }
///////////        program’s main functions     ////////////////////////////////////////////////

    void mainDisplay()
    {
        system("cls");
        if(mainStoreLength==0)
        {
            SetColor(4);
            printf("* the store have not category yet (create a new category ) .\n\n");
            SetColor(15);
        }
        else
        {
            for(int i=0; i<mainStoreLength; i++)
            {
                SetColor(3);
                printf("\n\nthe (%d) category (name) :- %s\n",i+1,mainStore[i].name);
                printf("the (%d) category has :- (%d) products \n",i+1,mainStore[i].myProductsLength);
                SetColor(15);
                for(int j=0; j<mainStore[i].myProductsLength; j++)
                {
                    SetColor(8);
                    printf("\n   the (name) of product num (%d) :- %s\n",j+1,mainStore[i].myProducts[j].name);
                    SetColor(15);
                    printf("   the (price) of product num (%d) :- %d\n",j+1,mainStore[i].myProducts[j].price);
                    printf("   the (quantity) of product num (%d) :- %d\n",j+1,mainStore[i].myProducts[j].quantity);

                }
            }
        }

        SetColor(6);
        printf("* plz (enter any key) to return to home page :");
        SetColor(15);
        char end=getch();
    }
    ////////////////////////////////////////////
    void mainCreateCategory()
    {
        system("cls");
        char end=0;
        do
        {
            if (mainStoreLength>=mainStoreSize)
            {
                SetColor(4);
                printf("* the store is full .\n\n");
                SetColor(15);
            }
            else
            {
                printf("* plz enter the name of new category :- ");
                scanf("%s",&mainStore[mainStoreLength].name);
                mainStore[mainStoreLength].myProductsSize=3;
                mainStore[mainStoreLength].myProducts=malloc(mainStore[mainStoreLength].myProductsSize*sizeof(product));
                mainStore[mainStoreLength].myProductsLength=0;
                mainStoreLength++;
                SetColor(6);
                printf("\n*     to (exit) press->(esc)     |     to (continue) press->(any key else) \n");
                SetColor(15);
                end=getch();
            }
        }
        while(end!=27);
    }
    ////////////////////////////////////////////
    void mainCreateProduct()
    {
        system("cls");
        char end=0;
        do
        {
            printf("* plz enter the (name) of (category) :- ");
            char userCategry [20];
            scanf("%s",userCategry);
            int found = 0;
            for(int i=0; i<mainStoreLength; i++)
            {
                if (mainStore[i].myProductsLength>=mainStore[i].myProductsSize)
                {
                    mainStore[i].myProducts = ResizeProduct(mainStore[i].myProducts,mainStore[i].myProductsSize );
                    mainStore[i].myProductsSize*=2;
                }
                int productNum =mainStore[i].myProductsLength;
                if( strcmp(mainStore[i].name,userCategry)==0)
                {
                    printf("* plz enter the (name) of product :- ");
                    scanf("%s",&mainStore[i].myProducts[productNum].name);
                    printf("* plz enter the (price) of product :- ");
                    scanf("%d",&mainStore[i].myProducts[productNum].price);
                    printf("* plz enter the (quantity) of product :- ");
                    scanf("%d",&mainStore[i].myProducts[productNum].quantity);
                    mainStore[i].myProductsLength++;
                    found++;
                }
            }
            if (found==0)
            {
                SetColor(4);
                printf("* plz Enter a valid category name.\n\n");
                SetColor(15);
            }
            SetColor(6);
            printf("\n*     to (exit) press->(esc)     |     to (continue) press->(any key else) \n");
            SetColor(15);
            end=getch();
        }
        while(end!=27);
    }
    /////////////////////////////////////////////
    void mainMoveProduct()
    {
        system("cls");
        char end=0;
        do
        {
            printf("* plz enter the (name) of (old category) :- ");
            char userCategry [20];
            scanf("%s",userCategry);
            printf("* plz enter the (name) of (product) :- ");
            char userProduct [20];
            scanf("%s",userProduct);
            printf("* plz enter the (name) of (new category) :- ");
            char userNewCategry [20];
            scanf("%s",userNewCategry);
            if(isProductFound(userCategry,userProduct)!=0&&isCategryFound(userNewCategry)!=0)
            {
                addProduct(userCategry,userNewCategry,userProduct);
                removeProduct(userCategry,userProduct);
            }
            else
            {
                SetColor(4);
                printf("* plz Enter a valid category and product name.\n\n");
                SetColor(15);
            }
            SetColor(6);
            printf("\n*     to (exit) press->(esc)     |     to (continue) press->(any key else) \n");
            SetColor(15);
            end=getch();
        }
        while(end!=27);

        }
    ///////////////////////////////////////////////
    void mainSell()
    {
        system("cls");
        int selledProductlength=0;
        int selledProductsize=1;
        product *selledProduct = malloc(selledProductsize * sizeof(product));
        char end=0;
        do
        {
            printf("* plz enter the (name) of (category) :- ");
            char userCategry [20];
            scanf("%s",userCategry);
            printf("* plz enter the (name) of (product) :- ");
            char userProduct [20];
            scanf("%s",userProduct);
            int found = 0;
            for(int i=0; i<mainStoreLength; i++)
            {
                int productNum =mainStore[i].myProductsLength;
                if( strcmp(mainStore[i].name,userCategry)==0)
                {
                    for(int j=0; j<productNum; j++)
                    {
                        if( strcmp(mainStore[i].myProducts[j].name,userProduct)==0)
                        {
                            found++;
                            printf("* plz enter the (quantity) of (product) :- ");
                            int userQuantityProduct;
                            scanf("%d",&userQuantityProduct);
                            if(userQuantityProduct>mainStore[i].myProducts[j].quantity)
                            {
                                SetColor(4);
                                printf("* that is just (%d) items of this product in the stock.\n\n",mainStore[i].myProducts[j].quantity);
                                SetColor(15);
                            }
                            else
                            {
                                mainStore[i].myProducts[j].quantity-=userQuantityProduct;
                                if(selledProductlength>=selledProductsize)
                                {
                                    selledProduct = ResizeProduct(selledProduct,selledProductsize);
                                    selledProductsize*=2;
                                }
                                selledProduct[selledProductlength]=mainStore[i].myProducts[j];
                                selledProduct[selledProductlength].quantity=userQuantityProduct;
                                selledProductlength++;
                            }
                        }
                    }
                }
            }
            if (found==0)
            {
                SetColor(4);
                printf("* plz Enter a valid category and product name.\n\n");
                SetColor(15);
            }
            SetColor(6);
            printf("\n*     to (exit) press->(esc)     |     to (continue) press->(any key else) \n");
            SetColor(15);
            end=getch();
        }
        while(end!=27);
        int totalPrice=0;
        for(int i=0; i<selledProductlength; i++)
        {
            SetColor(8);
            printf("\n   the (name) of product num (%d) :- %s\n",i+1,selledProduct[i].name);
            SetColor(15);
            printf("   the (price) of product num (%d) :- %d\n",i+1,selledProduct[i].price);
            printf("   the (quantity) of product num (%d) :- %d\n",i+1,selledProduct[i].quantity);
            totalPrice+=selledProduct[i].quantity*selledProduct[i].price;
        }
        SetColor(3);
        printf("\n\n   the total of price is :- %d\n\n\n\n",totalPrice);
        SetColor(15);

        SetColor(6);
        printf("* plz (enter any key) to return to home page :");
        SetColor(15);
        end=getch();
    }





///////////       the first of program      ///////////////////////////////////////////////

    while(open)
    {
        gotoxy(1,0);
        SetColor(3);
        printf("welcome to our store move up or down and choose :- ");
        SetColor(15);

        for (int i=0; i<menuLength; i++)
        {
            if(i==axisY)
            {
                SetColor(6);
                gotoxy(1,(i+2));
                printf("-> %s",menu[i]);
                SetColor(15);
            }
            else
            {
                gotoxy(3,(i+2));
                printf("%s",menu[i]);
            }
        }
        userInput=getch();
        if(userInput==-32)
        {
            char ch=getch();
            switch(ch)
            {
            case 72:
                axisY--;
                if(axisY<0)
                {
                    axisY+=menuLength;
                }
                gotoxy(1,axisY+2);
                break;
            case 80:
                axisY++;
                if(axisY>menuLength-1)
                {
                    axisY-=menuLength;
                }
                gotoxy(1,axisY+2);
                break;
            default:
                axisY=0;
                gotoxy(1,axisY+2);
                break;
            }
        }
        else
        {
            switch(userInput)
            {
            case 27:
                open=0;
                break;
            case 13:
            {
                switch(axisY)
                {
                case 0:
                    mainDisplay();
                    break;
                case 1:
                    mainCreateCategory();
                    break;
                case 2:
                    mainCreateProduct();
                    break;
                case 3:
                    mainMoveProduct();
                    break;
                case 4:
                    mainSell();
                    break;
                case 5:
                    open=0;
                    break;
                }
            }
            break;
            default:
                axisY=0;
                gotoxy(1,axisY+2);
                break;
            }
        }
        system("cls");
    }

    return 0;
}
