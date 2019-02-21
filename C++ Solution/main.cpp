/*Google Hash Code 2018 Practice Problem solution-1.9 million points on Hash Judge System.*/
/** @author Alexandru Dascalu*/

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

/** A data structure that represents a rectangle configuration, it is made up
 * of a number of rows and a number of columns.*/
struct rectangle
{
	int rows;
	int columns;
};

struct slice
{
	int startRow;
	int startColumn;
	int endRow;
	int endColumn;
};

/*A function that finds out what rectangle configurations we can have with
 * a rectangle that has maxCells cells. The vector that it returns does not
 * have rectangle configurations with maxCells cells that do not fit in the
 * whole pizza.*/
vector<rectangle> possibleRectangles(int maxCells, int totalRows, int totalColumns);

/*A function that reads data from a file representing a pizza and returns a
 * pointer to a matrix representing the pizza with 0s and 1s.*/
int** makePizza(string path, int & totalRows, int & totalColumns,
		int & minToppings, int & maxCells);

/*A function that counts the number of wasted cells if the piza was divided in slices
 * using a certain rectangle configuration and certain number of minimum toppings
 * and returns the number.*/
int wastedCellsForRectangle(rectangle sliceShape, int** pizza, int pizzaRows,
		int pizzaColumns, int minToppings);

/*Calculates a vector of pizza slices from the given pizza using the given
 * rectangle configuration.*/
vector<slice> getPizzaSlices(rectangle sliceShape, int** pizza, int pizzaRows, int pizzaColumns,
		int minToppings);

/*A function that takes in a vector of pizza slices and writes them to a file
 * at the given path.*/
void writeSlicesToFile(vector<slice> pizzaSlices, string path);

int main()
{
    string inputFiles[4]={"medium.in","big.in","example.in","small.in"};
    string outputFiles[4]={"resultMedium.out","resultBig.out","resultExample.out","resultSmall.out"};

    //divide the pizza of each file
    for(int currentFile = 0; currentFile < 4; currentFile++)
    {
    	string filePath = "Input Files\\" + inputFiles[currentFile];

    	//these variables hold the size of the pizza
        int pizzaRows, pizzaColumns;
        //holds the number of minimum cells of a topping type and the most cells possible in a slice
        int minToppings, maxCells;

        /*get a pointer to the pizza matrix and also initialise the size of the
         * pizza, the max cells in a slice and the minimum number of toppings.*/
        int** pizza = makePizza(filePath, pizzaRows, pizzaColumns, minToppings, maxCells);

        //vector of all possible rectangles configurations with maxCells cells
        //that can fit in our pizza
        vector<rectangle> possibleSliceShapes = possibleRectangles(maxCells, pizzaRows, pizzaColumns);

        /*this array will hold the number of wasted cells (cells not included in a slice)
         for each rectangle confirguration*/
        int wastedCells[possibleSliceShapes.size()];

        //we will add to values in this array, so we have to initialize it with 0s
        for(unsigned int i=0; i<possibleSliceShapes.size(); i++)
        {
            wastedCells[i] = wastedCellsForRectangle(possibleSliceShapes[i], pizza,
            		pizzaRows, pizzaColumns, minToppings);
        }

        //we find the minimum number of wasted cells
        int indexOfMinimum=0;
        for(unsigned int i=0; i < possibleSliceShapes.size(); i++)
        {
            cout<<wastedCells[i]<<" ";
            if(wastedCells[indexOfMinimum]>wastedCells[i])
            {
                indexOfMinimum=i;
            }
        }
        cout<<"\nThe least possible amount of wasted cells is "<<wastedCells[indexOfMinimum]<<endl;

        //set the optimal rows and columns based on where the least number of wasted cells was
        rectangle optimalShape = possibleSliceShapes[indexOfMinimum];

        vector<slice> pizzaSlices = getPizzaSlices(optimalShape, pizza, pizzaRows,
        		pizzaColumns, minToppings);

        writeSlicesToFile(pizzaSlices, outputFiles[currentFile]);

        //we delete the pointer array
        for(int i=0;i<pizzaRows;i++)
        {
            delete[] pizza[i];
        }
        delete[] pizza;
    }
}

/*A function that reads data from a file at the given path representing a
 * pizza and returns a pointer to a matrix representing the pizza with 0s
 * and 1s. It also reads the number of total rows, total columns, max cells
 * in a slice and the number of minimum toppings in a slice.*/
int** makePizza(string path, int & totalRows, int & totalColumns, int & minToppings, int & maxCells)
{
	ifstream fin(path.c_str());
	cout<<path<<endl;

	/*read the data about the size of the pizza, the minimum number of cells of each topping in
	 *a slice and the maximum size of the slice*/
	fin>>totalRows>>totalColumns>>minToppings>>maxCells;

	/*make the pizza array, we use an array of pointers to pointers because the big.in
	 * file has a 1000x1000 pizza, and with a normal array, it would not fit into stack
	 * file memory and the program would crash*/
	int** pizza = new int*[totalRows];
	for(int i=0; i < totalRows; i++)
	{
		pizza[i] = new int[totalColumns];
	}

	char topping;
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

	return pizza;
}

/*A function that finds out what rectangle configurations we can have with
 * a rectangle that has maxCells cells. The vector that it returns does not
 * have rectangle configurations with maxCells cells that do not fit in the
 * whole pizza.*/
vector<rectangle> possibleRectangles(int maxCells, int totalRows, int totalColumns)
{
	/*find out what possible rectangles can be made with maxCells number of cells.
	 *  Basically, we find out how many pairs of integers have maxCells as their product,
	 *  and make a rectangle data structure with those sizes and add it to the vector
	 *  after we make sure a rectangle with those dimensions actually fits in the array.*/
	int i=1;
	int quotient=maxCells;
	vector<rectangle> possibleSliceShapes;

	while(i<quotient)
	{
		if(maxCells%i==0)
		{
			quotient=maxCells/i;

			//see if an iXquotient rectangle fits in the pizza
			if(i<=totalRows && quotient<=totalColumns)
			{
				rectangle newRectangle;
				newRectangle.rows = i;
				newRectangle.columns = quotient;

				possibleSliceShapes.push_back(newRectangle);

				cout<<i<<" "<<quotient<<" ";
			}

			//see if an quotientXi rectangle fits in the pizza
			if(quotient<=totalRows && i<=totalColumns)
			{
				rectangle newRectangle;
				newRectangle.rows = quotient;
				newRectangle.columns = i;

				possibleSliceShapes.push_back(newRectangle);

				cout<<quotient<<" "<<i<<" ";
			}
		}

		i++;
	}

	cout<<endl;

	return possibleSliceShapes;
}

/*A function that counts the number of wasted cells if the piza was divided in slices
 * using a certain rectangle configuration and certain number of minimum toppings
 * and returns the number.*/
int wastedCellsForRectangle(rectangle sliceShape, int** pizza, int pizzaRows,
		int pizzaColumns, int minToppings)
{
	/*We divide the pizza into smaller strips, and for simplicity's sake
	 *we do it based which side is smaller(so if the current rectangle has less
	 *we rows than it has columns, we divide it in horizontal strips, and
	 *we we divide it in vertical strips otherwise).*/

	/*The width of the "strips" the pizza is didvided in. Equal to the number of
	 * rows or columns in the rectangle, depending on which side of the rectangle
	 * is smaller.*/
	int stripWidth;

	/*The length of the slice, and by length I mean the side of the slice not
	 *  equal to the width of the strip. Equal to the number of
	 * rows or columns in the rectangle, depending on which side of the rectangle
	 * is larger.*/
	 int originalSliceLength;

	/*The number of rows or columns on the side of the pizza perpendicular to the
	 * strips. So if the strips are horizontal, this variable will be the number
	 * of rows of the pizza, if the strips are vertical, this variabe will be
	 * number of columns of the pizza.*/
	int pizzaSidePerpendicularWithStrips;

	/*The number of rows or columns on the side of the pizza parallel to the
	 * strips. So if the strips are vertical, this variable will be the number
	 * of rows of the pizza, if the strips are horizontal, this variabe will be
	 * number of columns of the pizza.*/
	int pizzaSideParallelWithStrips;

	if (sliceShape.rows <= sliceShape.columns)
	{
		stripWidth = sliceShape.rows;
		originalSliceLength = sliceShape.columns;
		pizzaSidePerpendicularWithStrips = pizzaRows;
		pizzaSideParallelWithStrips = pizzaColumns;
	}
	else
	{
		stripWidth = sliceShape.columns;
		originalSliceLength = sliceShape.rows;
		pizzaSidePerpendicularWithStrips = pizzaColumns;
		pizzaSideParallelWithStrips = pizzaRows;
	}

	/*is the index of the upper row or column of the slice we are checking
	 * same for each 'strip' we divide the pizza into*/
	int currentStripIndex = 0;

	//the number of wasted cells if we divide the pizza using this rectangle
	//configuration
	int wastedCells = 0;

	/*Then,in these strips we see if a slice of rowsXcolumns cells is a valid
	 *slice. If it is, we increment the number of valid slices and advance currentColumn
	 *by the width of the rectangle. Else we add the number of cells in one column of
	 *the rectangle to the wastedCells array and advance currentColumn by 1.*/
	while (currentStripIndex < pizzaSidePerpendicularWithStrips)
	{
		//if we reach the bottom of the pizza we may need to make the 'strip' smaller
		if (currentStripIndex + stripWidth > pizzaSidePerpendicularWithStrips)
		{
			stripWidth = pizzaSidePerpendicularWithStrips - currentStripIndex;
		}

		/*is the index of the left most column or upper most row (depends on if
		 * the strips are vertical or not)of the slice we are checking
		 * not initialised here because it is reset for each slice of the pizza*/
		int currentSliceIndex = 0;

		/*at the end of the strip, the length of the last slice might become smaller,
		 * so we have a variable to represent it but also one that holds the original
		 * length of the slice*/
		int variableSliceLength = originalSliceLength;

		while (currentSliceIndex < pizzaSideParallelWithStrips)
		{
			//cout<<"Slice Index "<<currentSliceIndex<<endl;

			/*at the end of the strip we may not have enough
			 *space left, so we make the width smaller*/
			if (currentSliceIndex + variableSliceLength > pizzaSideParallelWithStrips)
			{
				variableSliceLength = pizzaSideParallelWithStrips - currentSliceIndex;
			}

			//the sum of the cells in the slice we are checking
			int sum = 0;

			//add all the cells of the slice we are checking
			for (int i = 0; i < stripWidth; i++)
			{
				for (int j = 0; j < variableSliceLength; j++)
				{
					if(sliceShape.rows <= sliceShape.columns)
					{
						sum += pizza[currentStripIndex + i][currentSliceIndex + j];
					}
					else
					{
						sum += pizza[currentSliceIndex + j][currentStripIndex + i];
					}
				}
			}
			//cout<<sum<<" on row "<<currentRow;

			//Check if there enough toppings of each type
			if (sum >= minToppings && (stripWidth * variableSliceLength - sum) >= minToppings)
			{
				//cout<<" "<<currentColumn;
				currentSliceIndex += variableSliceLength;
				//cout<<" passed"<<endl;
			}
			//if not, we advance currentSliceIndex and add to the wastedCells array
			else
			{
				//we advance currentSliceIndex by 1 if width has not been changed
				if (variableSliceLength == originalSliceLength)
				{
					//cout<<" "<<currentColumn;
					currentSliceIndex++;
					wastedCells += stripWidth;
					//cout<<" +"<<rows<<endl;
				}
				/*else, we know we are at the end of the 'strip', so we add all
				 *of the cells left to the wastedCells (if a slice does not have
				 *enough toppings of both types, a smaller slice within wont have
				 *enough as well, so there is no point in checking smaller slices).*/
				else
				{
					//cout<<" "<<currentColumn;
					wastedCells += stripWidth * variableSliceLength;

					/* currentStripIndex becomes equal with pizzaSidesPerpendicularWithStrips
					 * so the loop will stop.*/
					currentSliceIndex += variableSliceLength;
					//cout<<" +"<<rows*width<<endl;
				}
			}
		}
		/*advance currentRow so we go to the next horizontal 'slice'. At the end,
		 * currentStripIndex becomes equal with pizzaSidesPerpendicularWithStrips
		 * so the loop will stop.*/
		currentStripIndex += stripWidth;
	}

	return wastedCells;
}

/*Calculates a vector of pizza slices from the given pizza using the given
 * rectangle configuration.*/
vector<slice> getPizzaSlices(rectangle sliceShape, int** pizza, int pizzaRows,
		int pizzaColumns, int minToppings)
{
	/*We divide the pizza into smaller strips, and for simplicity's sake
	 *we do it based which side is smaller(so if the current rectangle has less
	 *we rows than it has columns, we divide it in horizontal strips, and
	 *we we divide it in vertical strips otherwise).*/

	/*The width of the "strips" the pizza is didvided in. Equal to the number of
	 * rows or columns in the rectangle, depending on which side of the rectangle
	 * is smaller.*/
	int stripWidth;

	/*The length of the slice, and by length I mean the side of the slice not
	 *  equal to the width of the strip. Equal to the number of
	 * rows or columns in the rectangle, depending on which side of the rectangle
	 * is larger.*/
	int originalSliceLength;

	/*The number of rows or columns on the side of the pizza perpendicular to the
	 * strips. So if the strips are horizontal, this variable will be the number
	 * of rows of the pizza, if the strips are vertical, this variabe will be
	 * number of columns of the pizza.*/
	int pizzaSidePerpendicularWithStrips;

	/*The number of rows or columns on the side of the pizza parallel to the
	 * strips. So if the strips are vertical, this variable will be the number
	 * of rows of the pizza, if the strips are horizontal, this variabe will be
	 * number of columns of the pizza.*/
	int pizzaSideParallelWithStrips;

	int optimalRows = sliceShape.rows;
	int optimalColumns = sliceShape.columns;

	if (optimalRows <= optimalColumns)
	{
		stripWidth = optimalRows;
		originalSliceLength = optimalColumns;
		pizzaSidePerpendicularWithStrips = pizzaRows;
		pizzaSideParallelWithStrips = pizzaColumns;
	}
	else
	{
		stripWidth = optimalColumns;
		originalSliceLength = optimalRows;
		pizzaSidePerpendicularWithStrips = pizzaColumns;
		pizzaSideParallelWithStrips = pizzaRows;
	}

	/*is the index of the upper row or column of the slice we are checking
	 * same for each 'strip' we divide the pizza into*/
	int currentStripIndex = 0;

	vector<slice> pizzaSlices;

	/*Then,in these strips we see if a slice of rowsXcolumns cells is a valid
	 *slice. If it is, we increment the number of valid slices and advance currentColumn
	 *by the width of the rectangle. Else we add the number of cells in one column of
	 *the rectangle to the wastedCells array and advance currentColumn by 1.*/
	while (currentStripIndex < pizzaSidePerpendicularWithStrips)
	{
		//if we reach the bottom of the pizza we may need to make the 'strip' smaller
		if (currentStripIndex + stripWidth > pizzaSidePerpendicularWithStrips)
		{
			stripWidth = pizzaSidePerpendicularWithStrips - currentStripIndex;
		}

		/*is the index of the left most column or upper most row (depends on if
		 * the strips are vertical or not)of the slice we are checking
		 * not initialised here because it is reset for each slice of the pizza*/
		int currentSliceIndex = 0;

		/*at the end of the strip, the length of the last slice might become smaller,
		 * so we have a variable to represent it but also one that holds the original
		 * length of the slice*/
		int variableSliceLength = originalSliceLength;

		while (currentSliceIndex < pizzaSideParallelWithStrips)
		{
			//cout<<"Slice Index "<<currentSliceIndex<<endl;

			/*at the end of the strip we may not have enough
			 *space left, so we make the width smaller*/
			if (currentSliceIndex + variableSliceLength
					> pizzaSideParallelWithStrips)
			{
				variableSliceLength = pizzaSideParallelWithStrips
						- currentSliceIndex;
			}

			//the sum of the cells in the slice we are checking
			int sum = 0;

			//add all the cells of the slice we are checking
			for (int i = 0; i < stripWidth; i++)
			{
				for (int j = 0; j < variableSliceLength; j++)
				{
					if (sliceShape.rows <= sliceShape.columns)
					{
						sum += pizza[currentStripIndex + i][currentSliceIndex
								+ j];
					}
					else
					{
						sum += pizza[currentSliceIndex + j][currentStripIndex
								+ i];
					}
				}
			}
			//cout<<sum<<" on row "<<currentRow;

			//Check if there enough toppings of each type
			if (sum >= minToppings && (stripWidth * variableSliceLength - sum)
										>= minToppings)
			{
				slice currentSlice;

				if(optimalRows <= optimalColumns)
				{
					currentSlice.startRow = currentStripIndex;
					currentSlice.startColumn = currentSliceIndex;
					currentSlice.endRow = currentStripIndex + stripWidth - 1;
					currentSlice.endColumn = currentSliceIndex + variableSliceLength - 1;
				}
				else
				{
					currentSlice.startRow = currentSliceIndex;
					currentSlice.startColumn = currentStripIndex;
					currentSlice.endRow = currentSliceIndex + variableSliceLength - 1;
					currentSlice.endColumn = currentStripIndex + stripWidth - 1;
				}

				pizzaSlices.push_back(currentSlice);
				currentSliceIndex += variableSliceLength;
			}
			//if not, we advance currentSliceIndex
			else
			{
				//we advance currentSliceIndex by 1 if slice length has not been changed
				if (variableSliceLength == originalSliceLength)
				{
					currentSliceIndex++;
				}
				/*else, we know we are at the end of the 'strip', so we terminate the loop.*/
				else
				{
					/* currentStripIndex becomes equal with pizzaSidesPerpendicularWithStrips
					 * so the loop will stop.*/
					currentSliceIndex += variableSliceLength;
				}
			}
		}
		/*advance currentRow so we go to the next horizontal 'slice'. At the end,
		 * currentStripIndex becomes equal with pizzaSidesPerpendicularWithStrips
		 * so the loop will stop.*/
		currentStripIndex += stripWidth;
	}

	return pizzaSlices;
}

/*A function that takes in a vector of pizza slices and writes them to a file
 * at the given path.*/
void writeSlicesToFile(vector<slice> pizzaSlices, string path)
{
	ofstream fout(path.c_str());

	//write number of slices to the file
	fout<<pizzaSlices.size();

	//write the 4 coordinates of each slice to the file
	for(unsigned int i = 0; i< pizzaSlices.size(); i++)
	{
		slice currentSlice = pizzaSlices[i];
		fout<<endl<<currentSlice.startRow<<" "<<currentSlice.startColumn<<" "
				<<currentSlice.endRow<<" "<<currentSlice.endColumn;
	}

	fout.clear();
	fout.close();
}
