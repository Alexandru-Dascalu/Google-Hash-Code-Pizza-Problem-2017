/*Google Hash Code 2018 Practice Problem solution-1.9 million points on Hash Judge System.*/
/** @author Alexandru Dascalu*/

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

int main()
{
    //these variables hold the size of the pizza
    int totalRows, totalColumns;
    //holds the number of minimum cells of a topping type and the most cells possible in a slice
    int minToppings,maxCells;

    string inputFiles[4]={"medium.in","big.in","example.in","small.in"};
    string outputFiles[4]={"resultMedium.out","resultBig.out","resultExample.out","resultSmall.out"};
    ifstream fin;
    ofstream fout;

    for(int currentFile=0;currentFile<4;currentFile++)
    {
        fin.open(inputFiles[currentFile].c_str());
        fout.open(outputFiles[currentFile].c_str());
        cout<<inputFiles[currentFile]<<endl;
        /*read the data about the size of the pizza, the minimum number of cells of each topping in
         *a slice and the maximum size of the slice*/
        fin>>totalRows>>totalColumns>>minToppings>>maxCells;
        char topping;

        /*make the pizza array, we use an array of pointers to pointers because the big.in
         *file has a 1000x1000 pizza, and with a normal array, it would not fit into stack
         *memory and the program would crash*/

        int** pizza =new int*[totalRows];
        for(int i=0;i<totalRows;i++)
        {
            pizza[i]=new int[totalColumns];
        }

        //as we read the input file, we assign 0s and 1s in the array, representing the two toppings
        for(int i=0;i<totalRows;i++)
        {
            for(int j=0;j<totalColumns;j++)
            {
                //tomatoes are represented by 1s, mushrooms by 0s
                fin>>topping;
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

        /*find out how many possible rectangles can be made with maxCells number of cells.
        Basically, we find out how many pairs of integers have maxCells as their product,
        and increment the number of rectangles after we make sure a rectangle with those
        dimensions actually fits in the array.*/
        int rectangles=0;
        int i=1;
        int quotient=maxCells;
        while(i<quotient)
        {
            if(maxCells%i==0)
            {
                quotient=maxCells/i;

                //see if an iXquotient rectangle fits in the pizza
                if(i<=totalRows && quotient<=totalColumns)
                {
                    rectangles++;
                }

                //see if an quotientXi rectangle fits in the pizza
                if(quotient<=totalRows && i<=totalColumns)
                {
                    rectangles++;
                }
            }
            i++;
        }
        cout<<rectangles<<endl;

        //create an array that holds the sizes of all the rectangles, stored in pairs,
        //so 1,5 means 1 row and 5 columns and 5,1 means 5 rows, 1 column
        int shapes[2*rectangles];

        int current=0;
        i=1;
        quotient=maxCells;
        while(i<quotient)
        {
            //again, we find all the pairs that have maxCells as their product and add the pairs to the arrays
            if(maxCells%i==0)
            {
                quotient=maxCells/i;

                //see if an iXquotient rectangle fits in the pizza and add it's sizes to the array if it does
                if(i<=totalRows && quotient<=totalColumns)
                {
                    shapes[current]=i;
                    shapes[current+1]=quotient;
                    current+=2;
                }

                //see if an quotientXi rectangle fits in the pizza and add it's sizes to the array if it does
                if(quotient<=totalRows && i<=totalColumns)
                {
                    shapes[current]=quotient;
                    shapes[current+1]=i;
                    current+=2;
                }
            }
            i++;
        }

        //display the pairs of rows and columns for valid rectangle configurations
        cout<<"Rectangle configurations:"<<endl;
        for(int i=0;i<2*rectangles;i++)
        {
            cout<<shapes[i]<<" ";
        }
        cout<<endl;

        /*this array will hold the number of wasted cells (cells not included in a slice)
         for each rectangle confirguration*/
        int wastedCells[rectangles];

        /*this array holds the number of valid slices the pizza is cut into for each
         *rectangle configuration, because we need this number for the output file*/
        int pizzaSlices[rectangles];

        //we will add to values in this array, so we have to initialize it with 0s
        for(int i=0;i<rectangles;i++)
        {
            wastedCells[i]=0;
        }

        /*for each rectangle configuration, we try to cut the pizza in rectangles
         *with these dimensions, and see which configuration is most optimal*/
        for (int i=0;i<rectangles;i++)
        {
            //the number of valid slices we will cut the pizza into
            int slices=0;

            /*We assign the size of the current rectangle, we use 2*i because the
             *shapes array is twice the size of the number of rectangles, and it
             *stores the dimensions of the rectangle as pairs of integers, so number
             *of rows will be on even indexes and number of columns on odd indexes*/
            int rows=shapes[2*i];
            //cout<<"\nrows:"<<rows;
            int columns=shapes[2*i+1];
            //cout<<"\ncolumns:"<<columns;

            /*We divide the pizza into smaller strips, and for simplicity's sake
             *we do it based which size is smaller(so if the current rectangle has less
             *rows than it has columns, we divide it in horizontal strips, and
             *we divide it in vertical strips otherwise).*/
            if(rows<=columns)
            {
                //is the index of the left most column of the slice we are checking
                //not initialised here because it is reset for each slice of the pizza
                int currentColumn;
                //is the index of the upper most row of the slice we are checking
                //same for each 'strip' we divide the pizza into
                int currentRow=0;
                //the width of the slice we are checking
                int width;
                /*Then,in these strips we see if a slice of rowsXcolumns cells is a valid
                 *slice. If it is, we increment the number of valid slices and advance currentColumn
                 *by the width of the rectangle. Else we add the number of cells in one column of
                 *the rectangle to the wastedCells array and advance currentColumn by 1.*/
                while(currentRow<totalRows)
                {
                    //if we reach the bottom of the pizza we may need to make the 'strip' smaller
                    if(currentRow+rows>totalRows)
                    {
                        rows=totalRows-currentRow;
                    }
                    currentColumn=0;

                    //for each 'strip' the width is columns in the begining
                    width=columns;

                    while(currentColumn<totalColumns)
                    {
                        /*at the end of the strip we may not have enough
                         *space left, so we make the width smaller*/
                        if(currentColumn+width>totalColumns)
                        {
                            width=totalColumns-currentColumn;
                        }

                        //the sum of the cells in the slice we are checking
                        int sum=0;

                        //add all the cells of the slice we are checking
                        for(int k=0;k<rows;k++)
                        {
                            for(int l=0;l<width;l++)
                            {
                                sum+=pizza[currentRow+k][currentColumn+l];
                            }
                        }
                        //cout<<sum<<" on row "<<currentRow;

                        //Check if there enough toppings of each type
                        if(sum>=minToppings && rows*width-sum>=minToppings)
                        {
                            slices++;
                            //cout<<" "<<currentColumn;
                            currentColumn+=width;
                            //cout<<" passed"<<endl;
                        }
                        //if not, we advance currentColumn and add to the wastedCells array
                        else
                        {
                            //we advance currentColumn by 1 if width has not been changed
                            if(width==columns)
                            {
                                //cout<<" "<<currentColumn;
                                currentColumn++;
                                wastedCells[i]+=rows;
                                //cout<<" +"<<rows<<endl;
                            }
                            /*else, we know we are at the end of the 'strip', so we add all
                             *of the cells left to the wastedCells (if a slice does not have
                             *enough toppings of both types, a smaller slice within wont have
                             *enough as well, so there is no point in checking smaller slices),
                             *and set currentColumn to totalColumns so the loop will stop.*/
                            else
                            {
                                //cout<<" "<<currentColumn;
                                wastedCells[i]+=rows*width;
                                currentColumn=totalColumns;
                                //cout<<" +"<<rows*width<<endl;
                            }
                        }
                    }
                    //advance currentRow so we go to the next horizontal 'slice'
                    currentRow+=rows;
                }
            }
            //algorithm is almost the same, but the 'slices' are vertical
            else
            {
                //is the index of the left most column of the slice we are checking
                //not initialised here because it is reset for each slice of the pizza
                int currentColumn=0;
                //is the index of the upper most row of the slice we are checking
                //same for each 'strip' we divide the pizza into
                int currentRow;
                //the length of the slice we are checking
                int length;

                /*Then,in these strips we see if a slice of rowsXcolumns cells is a valid
                *slice. If it is, we increment the number of valid slices and advance currentRow
                *by the length of the rectangle. Else we add the number of cells in one row of
                *the rectangle to the wastedCells array and advance currentRow by 1.*/
                while(currentColumn<totalColumns)
                {
                     //if we reach the leftmost edge of the pizza we may need to make the 'strip' smaller
                    if(currentColumn+columns>totalColumns)
                    {
                        columns=totalColumns-currentColumn;
                    }
                    currentRow=0;

                    //for each 'strip' the length is rows in the begining
                    length=rows;

                    while(currentRow<totalRows)
                    {
                        /*at the end of the strip we may not have enough space left,
                         *so we make the length smaller*/
                        if(currentRow+length>totalRows)
                        {
                            length=totalRows-currentRow;
                        }
                        int sum=0;

                        //add all the cells of the slice we are checking
                        for(int k=0;k<length;k++)
                        {
                            for(int l=0;l<columns;l++)
                            {
                                sum+=pizza[currentRow+k][currentColumn+l];
                            }
                        }
                        //cout<<sum<<" on column "<<currentColumn;

                        //check if there are enough toppings of each type
                        if(sum>=minToppings && length*columns-sum>=minToppings)
                        {
                            slices++;
                            //cout<<" "<<currentRow;
                            currentRow+=length;
                            //cout<<" passed"<<endl;
                        }
                        //if not, we advance currentRow and add to the wastedCells array
                        else
                        {
                            //we advance currentRow by 1 if the length has not been changed
                            if(length==rows)
                            {
                                //cout<<" "<<currentRow;
                                currentRow++;
                                wastedCells[i]+=columns;
                                //cout<<" +"<<columns<<endl;
                            }
                            /*else, we know we are at the end of the 'strip', so we add all
                            *of the cells left to the wastedCells, and set currentRow to
                            *totalRows so the loop will stop.*/
                            else
                            {
                                //cout<<" "<<currentRow;
                                wastedCells[i]+=length*columns;
                                currentRow=totalRows;
                                //cout<<" +"<<length*columns<<endl;
                            }
                        }
                    }
                    //we advance the currentColumn to go to the next vertical 'strip'
                    currentColumn+=columns;
                }
            }
            //add the number of valid pizza slices we found starting from a
            //given rectangle configuration to the array
            pizzaSlices[i]=slices;
        }

        //we find the minimum number of wasted cells
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

        //set the optimal rows and columns based on where the least number of wasted cells was
        int optimalRows=shapes[2*indexOfMinimum];
        int optimalColumns=shapes[2*indexOfMinimum+1];
        cout<<optimalRows<<" "<<optimalColumns<<endl;

        //write the number of slices to the file
        fout<<pizzaSlices[indexOfMinimum]<<'\n';

        /*We repeat the entire algorithm from above, but we dont count wastedCells,
         *and we add valid slices to the output file*/
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

        //we delete the pointer array
        for(int i=0;i<totalRows;i++)
        {
            delete[] pizza[i];
        }
        delete[] pizza;

        //close input stream and outputstream
        fin.clear();
        fin.close();
        fout.clear();
        fout.close();
    }
}

