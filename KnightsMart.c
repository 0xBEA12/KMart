#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//*************************************************************//
typedef struct KnightsMartProduct 
{
 int itemNum;
 char itemName[21];
 double unitPrice;
 int stockQty;
 int restockQty;
 struct KnightsMartProduct *next;
} KMProduct;
//*************************************************************//
//*************************************************************//
typedef struct KnightsMartRestockProduct 
{
 int itemNum;
 struct KnightsMartRestockProduct *next;
} KMRestockProduct;
//*************************************************************//
//*************************************************************//
typedef struct KnightsMartSale 
{
 char firstName[21];
 char lastName[21];
 int numItemsOnList; // # of items on shopping list
 int *itemsPurchased; // array of item numbers
 struct KnightsMartSale *next;
} KMSale;
//*************************************************************//
/////////////////////////////////////////////////////////////////
KMProduct* AddItem(FILE* KMI, FILE* KMO, KMProduct* Products)
{
  KMProduct* NewItem = malloc(sizeof(KMProduct));
  //Needed so that Products can always equal the head of the list
  //as we traverse the linked list
  KMProduct* HelpPointer = Products;
  //Needed so that we can add new elements to the end of the linked
  //list as HelpPointer traverses it
  fscanf(KMI, "%d%s%lf%d%d", &NewItem->itemNum, NewItem->itemName, &NewItem->unitPrice, &NewItem->stockQty, &NewItem->restockQty);

  fprintf(KMO, "Item %d, %s, with a cost of $%3.2lf and initial stock of %d, has been added to the product database.\n", NewItem->itemNum, NewItem->itemName, NewItem->unitPrice, NewItem->stockQty);
  //used for the case where Products is empty
  if(HelpPointer == NULL)
  {
    NewItem->next = NULL;
    return NewItem;
  }
  //used when Products has at least 1 item already in its list
  else
  {
    while(HelpPointer->next != NULL)
    {
      HelpPointer = HelpPointer->next;
     
    }

      HelpPointer->next = NewItem;
      NewItem->next = NULL;
      return Products;
  }

}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void Inventory(FILE* KMO, KMProduct* Products)
{
  //needed to traverse through the Products linked list
  KMProduct* HelpPointer = Products;
  fprintf(KMO, "INVENTORY:  contains the following items:\n");
  //simple way to traverse Products linked list and fprintf out the
  //data of each node into specific format
  while(HelpPointer != NULL)
  {
    fprintf(KMO, "\t| Item %6d | %-20s | $%7.2lf | %4d unit(s) |\n", HelpPointer->itemNum, HelpPointer->itemName, HelpPointer->unitPrice, HelpPointer->stockQty);
    HelpPointer = HelpPointer->next;
  }
  return;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
KMRestockProduct* Restock(FILE* KMO, KMProduct* Products, KMRestockProduct* Restocked)
{
  KMProduct* ProductsHelper = Products;
  KMRestockProduct* RestockedHelper = Restocked;
  KMRestockProduct* DeleteHelper;

  fprintf(KMO, "RESTOCK:  the following items have been reordered and restocked:\n");

  if(RestockedHelper == NULL)
  {
    fprintf(KMO, "No items to restock\n");
    return NULL;
  }

  else
  {
    while(Restocked != NULL)
    {
      while(ProductsHelper != NULL)
      {
        if(Restocked->itemNum == ProductsHelper->itemNum)
        {
          fprintf(KMO, "\tItem %d, %s, restocked to a quantity of %d.\n", ProductsHelper->itemNum, ProductsHelper->itemName, ProductsHelper->restockQty);
          ProductsHelper->stockQty = ProductsHelper->restockQty;
        }

        else
        {
          ProductsHelper = ProductsHelper->next;
        }

      }

      DeleteHelper = RestockedHelper;
      RestockedHelper = RestockedHelper->next;
      //free all malloc storage as we traverse Sold linked list
      free(DeleteHelper);
      
    }

    return NULL;
  }

  
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
KMSale* Customer(FILE* KMO, FILE* KMI, KMProduct* Products, KMSale* Sold, KMRestockProduct* Restocked)
{
  KMSale* NewSale = malloc(sizeof(KMSale));
  //needed to traverse Products linked list
  KMProduct* ProductsHelper = Products;
  //needed to traverse Sold linked list
  KMSale* SoldHelper = Sold;
  //needed to traverse restocked linked list
  KMRestockProduct* RestockedHelper = Restocked;

  fscanf(KMI, "%s%s%d", NewSale->firstName, NewSale->lastName, &NewSale->numItemsOnList);
  fprintf(KMO, "Customer %s %s came and made some purchases.\n", NewSale->firstName, NewSale->lastName);
  NewSale->itemsPurchased = malloc(NewSale->numItemsOnList * sizeof(int));

  //simply scan in numbers to array
  for(int i = 0; i < NewSale->numItemsOnList; i++)
  {
    fscanf(KMI, "%d", &NewSale->itemsPurchased[i]);
  }

  //used when Sold has no items in its list
  if(SoldHelper == NULL)
  {
    for(int j = 0; j < NewSale->numItemsOnList; j += 2)
    {
      while(ProductsHelper != NULL)
      {
        //designed to account for a case where the item purchased is
        //the first node of the Products linked list
        if(ProductsHelper->itemNum == NewSale->itemsPurchased[j])
        {
          //accounts for a case where there is not enough stock to
          //completely fill order
          if(ProductsHelper->stockQty - NewSale->itemsPurchased[j+1] < 0)
          {
            NewSale->itemsPurchased[j+1] = ProductsHelper->stockQty;
            ProductsHelper->stockQty = 0;
            //for the case where Restocked linked list
            //is empty
            if(RestockedHelper == NULL)
            {
            
              KMRestockProduct* NewRestock = malloc(sizeof(KMRestockProduct));
              NewRestock->itemNum = NewSale->itemsPurchased[j];
              Restocked = NewRestock;
              NewRestock->next = NULL;
         
              
            }
            //for every other case where Restocked linked list
            //has items on it
            else
            {
              while(RestockedHelper->next != NULL)
              {
                //printf("%d/", RestockedHelper->itemNum);
                RestockedHelper = RestockedHelper->next;
              }
              KMRestockProduct* NewRestock = malloc(sizeof(KMRestockProduct));
              RestockedHelper->next = NewRestock;
              NewRestock->itemNum = NewSale->itemsPurchased[j];
              NewRestock->next = NULL;

            }
          }
          //accounts for every other case where there
          //is enough stock to fill orders
          else if(ProductsHelper->stockQty >= NewSale->itemsPurchased[j+1])
          {
            ProductsHelper->stockQty -= NewSale->itemsPurchased[j+1];
          }

          break;
        }

        else if(ProductsHelper->itemNum != NewSale->itemsPurchased[j])
        {
          ProductsHelper = ProductsHelper->next;
        }

      }
      
    }

    NewSale->next = NULL;
    return NewSale;
  }

  //used when Sold has items on its list
  else
  {
    while(SoldHelper->next != NULL)
    {
      SoldHelper = SoldHelper->next;
    }

    for(int j = 0; j < NewSale->numItemsOnList; j += 2)
    {
      while(ProductsHelper != NULL)
      {
        //designed to account for a case where the item purchased
        //is the first node of the Products linked list
        if(ProductsHelper->itemNum == NewSale->itemsPurchased[j])
        {
          //accounts for a case where there is not enough stock to
          //completely fill order
          if(ProductsHelper->stockQty - NewSale->itemsPurchased[j+1] < 0)
          {
            NewSale->itemsPurchased[j+1] = ProductsHelper->stockQty;
            ProductsHelper->stockQty = 0;

            if(RestockedHelper == NULL)
            {
              
              KMRestockProduct* NewRestock = malloc(sizeof(KMRestockProduct));
              NewRestock->itemNum = NewSale->itemsPurchased[j];
              Restocked = NewRestock;
              NewRestock->next = NULL;
            
            }
            //for every other case where Restocked linked list
            //has items on it
            else
            {
              while(RestockedHelper->next != NULL)
              {
                RestockedHelper = RestockedHelper->next;
              }
              
              KMRestockProduct* NewRestock = malloc(sizeof(KMRestockProduct));
              RestockedHelper->next = NewRestock;
              NewRestock->itemNum = NewSale->itemsPurchased[j];
              NewRestock->next = NULL;
              
            }

          }

          else if(ProductsHelper->stockQty >= NewSale->itemsPurchased[j+1])
          {
            ProductsHelper->stockQty -= NewSale->itemsPurchased[j+1];
          }

          break;
        }

        else
        {
        
          ProductsHelper = ProductsHelper->next;
      
        }

      }

    }
    SoldHelper->next = NewSale;
    NewSale->next = NULL;
    return Sold;
  }

}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
KMSale* PrintDaySummary(FILE* KMO, KMSale* Sold, KMProduct* Products)
{
  int Tally = 0;
  double Total = 0;
  double GrandTotal = 0;
  KMSale* SoldHelper = Sold;
  KMSale* DeleteHelper;
  KMProduct* ProductsHelper = Products;

  fprintf(KMO, "KnightsMart Daily Sales report:\n");
  //for case where no items are purchased
  if(SoldHelper == NULL)
  {
    fprintf(KMO, "There were no sales today.\n");
    return NULL;
  }
  //for every other case where an item is purchased
  else
  {
    //if there is still a node in Sold linked list find items
    //purchased and print them out
    while(SoldHelper != NULL)
    {
      Tally++;
      fprintf(KMO, "Sale #%d, %s %s purchased %d item(s):\n", Tally, SoldHelper->firstName, SoldHelper->lastName, (SoldHelper->numItemsOnList) / 2);
      //traverse each Sold->itemsPurchaed array and find the items purchased for 
      //each customer and add the total cost for each customer
      for(int i = 0; i < SoldHelper->numItemsOnList; i += 2)
      {
        //Traverse Products linked list until you find itemsPurchased
        while(ProductsHelper != NULL)
        {
          //for the case where you find the item purchased by the customer
          if (ProductsHelper->itemNum == SoldHelper->itemsPurchased[i])
          {
            fprintf(KMO, "\t\t| Item %6d | %-20s | $%7.2lf (x%4d) |\n", SoldHelper->itemsPurchased[i], ProductsHelper->itemName, ProductsHelper->unitPrice, SoldHelper->itemsPurchased[i+1]);
            Total += (ProductsHelper->unitPrice) * (SoldHelper->itemsPurchased[i+1]);
            GrandTotal += (ProductsHelper->unitPrice) * (SoldHelper->itemsPurchased[i+1]);
            break;
          }
          //for every other case traverse forwards by 1
          else
          {
            ProductsHelper = ProductsHelper->next;
          }

        }
        //reset the Products linked list to the head after each search
        ProductsHelper = Products;
      }

      fprintf(KMO, "\t\tTotal: $%3.2lf\n", Total);
      Total = 0;
      DeleteHelper = SoldHelper;
      SoldHelper = SoldHelper->next;
      //free all malloc storage as we traverse Sold linked list
      free(DeleteHelper->itemsPurchased);
      free(DeleteHelper);

    }

    fprintf(KMO, "Grand total: $%3.2lf\n", GrandTotal);
    return NULL;
  }

}
/////////////////////////////////////////////////////////////////
int main(void) 
{
  int NoD, NoC;
  char Command[15];

  KMProduct* Products = NULL;
  KMRestockProduct* Restocked = NULL;
  KMSale* Sold = NULL;

  FILE *KMI, *KMO;
  KMI = fopen("KnightsMart.in", "r");
  KMO = fopen("KnightsMart.out", "w");

  fscanf(KMI, "%d", &NoD);

  for(int i = 0; i < NoD; i++)
  {
    fprintf(KMO, "*************************\nUCF KnightsMart Day %d\n*************************\n", i + 1);
    fscanf(KMI, "%d", &NoC);
    for(int j = 0; j < NoC; j++)
    {
      fscanf(KMI, "%s", Command);

      if(strcmp(Command, "ADDITEM") == 0)
      {
        Products = AddItem(KMI, KMO, Products);
      }

      else if(strcmp(Command, "INVENTORY") == 0)
      {
        Inventory(KMO, Products);
      }

      else if(strcmp(Command, "RESTOCK") == 0)
      {
        Restock(KMO, Products, Restocked);
      }
      
      else if(strcmp(Command, "CUSTOMER") == 0)
      {
         Sold = Customer(KMO, KMI, Products, Sold, Restocked);
         
         
      }
      
      else if(strcmp(Command, "PRINTDAYSUMMARY") == 0)
      {
        Sold = PrintDaySummary(KMO, Sold, Products);
      }

    }

  }

  return 0;
}