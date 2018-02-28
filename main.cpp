#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

ifstream fin1("small.in");
ifstream fin2("medium.in");
ifstream fin3("big.in");
ifstream fin4("example.in");
/*fout displays the output as required by the Pizza problem, while fout2 displays it more nicely in a file.*/
ofstream fout("result.out");
ofstream fout2("test.out");

int main()
{
    //declare and read in the input file
    int totalRows, totalColumns, minToppings,maxCells;
    fin3>>totalRows>>totalColumns>>minToppings>>maxCells;
    char topping;
    int** pizza =new int*[totalColumns];
    for(int i=0;i<totalColumns;i++)
    {
        pizza[i]=new int[totalRows];
    }
    /*When we declare the array as a normal one, the program will crash when using big.in, so we use
     an array of pointers to int arrays. But, with the pointer array, the result is different each
     time with medium.in, and with other 2 input files, it crashes at the end. So when not using big.in,
     comment out the pointer array and the delete commands at the end and uncomment the normal array below.*/
    //int pizza[totalRows][totalColumns];

    //as we read the input file, we make an array with 0s and 1s, representing the two toppings
    for(int i=0;i<totalRows;i++)
    {
         for(int j=0;j<totalColumns;j++)
        {
            fin3>>topping;
            if(topping=='T')
            {
                pizza[i][j]=1;
            }
            else if(topping=='M')
            {
                pizza[i][j]=0;
            }
        }
    }

    //display them
    cout<<totalRows<<" "<<totalColumns<<" "<<minToppings<<" "<<maxCells<<endl;

    for(int i=0;i<totalRows;i++)
    {
        for(int j=0;j<totalColumns;j++)
        {
            cout<<pizza[i][j]<<" ";
        }

        cout<<endl;
    }

    //find out how many possible rectangles can be made with maxCells number of cells that fit in the pizza
    int rectangles=0;
    for(int i=1;i<=maxCells;i++)
    {
        if(maxCells%i==0)
        {
            if(i<=totalRows && maxCells/i<=totalColumns)
            {
                rectangles++;
            }
        }
    }

    //create an array that holds the sizes of all the shapes, we store them in pairs,
    //so 1,5 means 1 row and 5 columns and 5,1 means 5 rows, 1 column
    int shapes[2*rectangles];
    int current=0;
    for(int i=1;i<=maxCells;i++)
    {
        if(maxCells%i==0)
        {
            if(i<=totalRows && maxCells/i<=totalColumns)
            {
                shapes[current]=i;
                shapes[current+1]=maxCells/i;
                current+=2;
            }
        }
    }

    for(int i=0;i<2*rectangles;i++)
    {
        cout<<shapes[i]<<" "<<flush;
    }

    int wastedCells[rectangles];
    int pizzaSlices[rectangles];
    for(int i=0;i<rectangles;i++)
    {
        wastedCells[i]=0;
    }

    for (int i=0;i<rectangles;i++)
    {
        int slices=0;
        int rows=shapes[2*i];
        cout<<"\nrows:"<<rows<<endl;
        int columns=shapes[2*i+1];
        cout<<"\ncolumns:"<<columns<<endl;
        if(rows<=columns)
        {
            int currentColumn,currentRow=0,width;
            while(currentRow<totalRows)
            {
                if(currentRow+rows>totalRows)
                {
                     rows=totalRows-currentRow;
                }
                currentColumn=0;
                width=columns;
                while(currentColumn<totalColumns)
                {
                    if(currentColumn+width>totalColumns)
                    {
                        width=totalColumns-currentColumn;
                    }
                    int sum=0;
                    for(int k=0;k<rows;k++)
                    {
                        for(int l=0;l<width;l++)
                        {
                           sum+=pizza[currentRow+k][currentColumn+l];
                        }
                    }
                    cout<<sum<<" on row "<<currentRow;

                    if(sum>=minToppings && rows*width-sum>=minToppings)
                    {
                        slices++;
                        cout<<" "<<currentColumn;
                        currentColumn+=width;
                        cout<<" passed"<<endl;
                    }
                    else
                    {
                        if(width==columns)
                        {
                            cout<<" "<<currentColumn;
                            currentColumn++;
                            wastedCells[i]+=rows;
                            cout<<" +"<<rows<<endl;
                        }
                        else
                        {
                            cout<<" "<<currentColumn;
                            wastedCells[i]+=rows*width;
                            currentColumn=totalColumns;
                            cout<<" +"<<rows*width<<endl;
                        }
                    }
                }
                currentRow+=rows;
            }
        }
        else
        {
            int currentColumn=0,currentRow,length;
            while(currentColumn<totalColumns)
            {
                if(currentColumn+columns>totalColumns)
                {
                    columns=totalColumns-currentColumn;
                }
                currentRow=0;
                length=rows;
                while(currentRow<totalRows)
                {
                    if(currentRow+length>totalRows)
                    {
                        length=totalRows-currentRow;
                    }
                    int sum=0;
                    for(int k=0;k<length;k++)
                    {
                        for(int l=0;l<columns;l++)
                        {
                           sum+=pizza[currentRow+k][currentColumn+l];
                        }
                    } cout<<sum<<" on column "<<currentColumn;


                    if(sum>=minToppings && length*columns-sum>=minToppings)
                    {
                        slices++;
                        cout<<" "<<currentRow;
                        currentRow+=length;
                        cout<<" passed"<<endl;
                    }
                    else
                    {
                        if(length==rows)
                        {
                            cout<<" "<<currentRow;
                            currentRow++;
                            wastedCells[i]+=columns;
                            cout<<" +"<<columns<<endl;
                        }
                        else
                        {
                            cout<<" "<<currentRow;
                            wastedCells[i]+=length*columns;
                            currentRow=totalRows;
                            cout<<" +"<<length*columns<<endl;
                        }
                    }
                }
                currentColumn+=columns;
            }
        }
        pizzaSlices[i]=slices;
    }
    int indexOfMinimum=0;
    for(int i=0;i<rectangles;i++)
    {
        cout<<wastedCells[i]<<" ";
        if(wastedCells[indexOfMinimum]>wastedCells[i])
        {
           indexOfMinimum=i;
        }
    }

    cout<<"\nThe least possible amount of wasted cells is "<<wastedCells[indexOfMinimum]<<endl;

    int optimalRows=shapes[2*indexOfMinimum];
    int optimalColumns=shapes[2*indexOfMinimum+1];

    cout<<optimalRows<<" "<<optimalColumns;

    fout<<pizzaSlices[indexOfMinimum]<<'\n';
    int sliceNr=0;
    if(optimalRows<=optimalColumns)
    {
        int currentColumn,currentRow=0,width;
        while(currentRow<totalRows)
        {
            if(currentRow+optimalRows>totalRows)
            {
                 optimalRows=totalRows-currentRow;
            }
            currentColumn=0;
            width=optimalColumns;
            while(currentColumn<totalColumns)
            {
                if(currentColumn+width>totalColumns)
                {
                    width=totalColumns-currentColumn;
                }
                int sum=0;
                for(int k=0;k<optimalRows;k++)
                {
                    for(int l=0;l<width;l++)
                    {
                       sum+=pizza[currentRow+k][currentColumn+l];
                    }
                }

                if(sum>=minToppings && optimalRows*width-sum>=minToppings)
                {
                    sliceNr++;
                    fout2<<"Slice "<<sliceNr<<" :"<<currentRow<<" "<<currentColumn<<" "<<currentRow+optimalRows-1<<" "<<currentColumn+width-1<<'\n';
                    fout<<currentRow<<" "<<currentColumn<<" "<<currentRow+optimalRows-1<<" "<<currentColumn+width-1<<'\n';
                    currentColumn+=width;
                }
                else
                {
                    if(width==optimalColumns)
                    {
                        currentColumn++;
                    }
                    else
                    {
                        currentColumn=totalColumns;
                    }
                }
            }
            currentRow+=optimalRows;
        }
    }
    else
    {
        int currentColumn=0,currentRow,length;
        while(currentColumn<totalColumns)
        {
            if(currentColumn+optimalColumns>totalColumns)
            {
                optimalColumns=totalColumns-currentColumn;
            }
            currentRow=0;
            length=optimalRows;
            while(currentRow<totalRows)
            {
                if(currentRow+length>totalRows)
                {
                    length=totalRows-currentRow;
                }
                int sum=0;
                for(int k=0;k<length;k++)
                {
                    for(int l=0;l<optimalColumns;l++)
                    {
                       sum+=pizza[currentRow+k][currentColumn+l];
                    }
                }

                if(sum>=minToppings && length*optimalColumns-sum>=minToppings)
                {
                    sliceNr++;
                    fout2<<"Slice "<<sliceNr<<" :"<<currentRow<<" "<<currentColumn<<" "<<currentRow+length-1<<" "<<currentColumn+optimalColumns-1<<'\n';
                    fout<<currentRow<<" "<<currentColumn<<" "<<currentRow+length-1<<" "<<currentColumn+optimalColumns-1<<'\n';
                    currentRow+=length;
                }
                else
                {
                    if(length==optimalRows)
                    {
                        currentRow++;
                    }
                    else
                    {
                        currentRow=totalRows;
                    }
                }
            }
            currentColumn+=optimalColumns;
        }
    }



    for(int i=0;i<totalRows;i++)
    {
        delete[] pizza[i];
    }

    delete[] pizza;
}
