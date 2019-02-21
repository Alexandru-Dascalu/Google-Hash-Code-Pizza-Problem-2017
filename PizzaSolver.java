import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

public class PizzaSolver 
{
    public static void main(String[] args) 
    {
        String[] inputFiles = {"example.in","small.in","medium.in","big.in"};
        String[] outputFiles = {"resultExample.out","resultSmall.out","resultMedium.out","resultBig.out"};
        
        for(int currentFile=0; currentFile < inputFiles.length; currentFile++)
        {
            System.out.println(inputFiles[currentFile]);
            Scanner pizzaReader = null;
            try 
            {
                pizzaReader = new Scanner(new File("Input Files\\" + inputFiles[currentFile]));
            }
            catch (FileNotFoundException e) 
            {
               System.out.println(e.getMessage());
               System.exit(-1);
            }
            
            int pizzaRows = pizzaReader.nextInt();
            int pizzaColumns = pizzaReader.nextInt();
            int minToppings = pizzaReader.nextInt();
            int maxCells = pizzaReader.nextInt();
            
            Cell[][] pizza = readPizza(pizzaReader, pizzaRows, pizzaColumns);
            pizzaReader.close();
            
            ArrayList<Slice> allocatedSlices = new ArrayList<>();
            for(int i=0; i < pizzaRows; i++)
            {
                for(int j = 0; j < pizzaColumns; j++)
                {
                    if(!pizza[i][j].isAllocated() && pizza[i][j].getTopping()==1)
                    {
                        Slice origin = new Slice(i, j, i, j);
                        ArrayList<Slice> largestSlices = new ArrayList<>();
                        
                        computeLargestSlices(pizza, maxCells, origin, largestSlices);
                        largestSlices.sort(null);
                        
                        for(Slice slice: largestSlices)
                        {
                            int sliceSum = getSliceSum(pizza, slice);
                            
                            if (sliceSum >= minToppings && (slice.getRows() * slice.getColumns() - sliceSum) >= minToppings)
                            {
                                allocatedSlices.add(slice);
                                allocateSlice(pizza, slice);
                                break;
                            }
                        }
                    }
                }
            }
            
            writeSlices(allocatedSlices, outputFiles[currentFile]);
            
        }
    }
    
    private static void writeSlices(ArrayList<Slice> allocatedSlices, String path)
    {
        PrintWriter sliceWriter = null;
        try 
        {
            sliceWriter = new PrintWriter(path);
        }
        catch (FileNotFoundException e) 
        {
            System.out.println(e.getMessage());
            System.exit(-1);
        }
        
        sliceWriter.println(allocatedSlices.size());
       
        for(Slice slice: allocatedSlices)
        {
            sliceWriter.println(slice.toString());
        }
        
        sliceWriter.close();
    }
    
    private static void allocateSlice(Cell[][] pizza, Slice slice)
    {
        //System.out.println(slice.toString());
        for(int i = slice.getStartRow(); i <= slice.getEndRow(); i++)
        {
            for(int j = slice.getStartColumn(); j <= slice.getEndColumn(); j++)
            {
                pizza[i][j].allocate();
            }
        }
    }
    
    private static boolean computeLargestSlices(Cell[][] pizza, int maxCells, Slice slice, ArrayList<Slice> largestSlices)
    {
        int sliceSize = (slice.getEndRow() - slice.getStartRow() + 1) *
                (slice.getEndColumn() - slice.getStartColumn() + 1);
        
        if(sliceSize > maxCells)
        {
            return false;
        }      
        else if(overlapsAllocated(pizza, slice))
        {
            return false;
        }
        else
        {
            boolean moreUpwards = false;
            if(slice.getStartRow() > 0)
            {
                Slice upExtendedSlice = new Slice(slice.getStartRow() - 1, slice.getStartColumn(),
                    slice.getEndRow(), slice.getEndColumn());
                
                moreUpwards = computeLargestSlices(pizza, maxCells, upExtendedSlice, largestSlices);
            }
            
            boolean moreDownwards = false;
            if(slice.getEndRow() < pizza.length - 1)
            {
                Slice downExtendedSlice = new Slice(slice.getStartRow(), slice.getStartColumn(),
                    slice.getEndRow() + 1, slice.getEndColumn());
                
                moreDownwards = computeLargestSlices(pizza, maxCells, downExtendedSlice, largestSlices);
            }
            
            boolean moreLeftwards = false;
            if(slice.getStartColumn() > 0)
            {
                Slice leftExtendedSlice = new Slice(slice.getStartRow(), slice.getStartColumn() - 1,
                    slice.getEndRow(), slice.getEndColumn());
                
                moreLeftwards = computeLargestSlices(pizza, maxCells, leftExtendedSlice, largestSlices);
            }
            
            boolean moreRightwards = false;
            if(slice.getEndColumn() < pizza[0].length - 1)
            {
                Slice rightExtendedSlice = new Slice(slice.getStartRow(), slice.getStartColumn(),
                    slice.getEndRow(), slice.getEndColumn() + 1);
                
                moreRightwards = computeLargestSlices(pizza, maxCells, rightExtendedSlice, largestSlices);
            }
            
            if(!(moreUpwards || moreDownwards || moreLeftwards || moreRightwards))
            {
                largestSlices.add(slice);
            }
            
            return true;
        }
    }
    
    private static boolean overlapsAllocated(Cell[][] pizza, Slice slice)
    {
        for(int i = slice.getStartRow(); i <= slice.getEndRow(); i++)
        {
            for(int j = slice.getStartColumn(); j <= slice.getEndColumn(); j++)
            {
                if(pizza[i][j].isAllocated())
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    private static int getSliceSum(Cell[][] pizza, Slice slice)
    {
        int sum = 0;
        
        for(int i = slice.getStartRow(); i <= slice.getEndRow(); i++)
        {
            for(int j = slice.getStartColumn(); j <= slice.getEndColumn(); j++)
            {
                sum += pizza[i][j].getTopping();
            }
        }
        
        return sum;
    }
    
    private static Cell[][] readPizza(Scanner pizzaReader, int pizzaRows, int pizzaColumns)
    {
        Cell[][] pizza = new Cell[pizzaRows][pizzaColumns];
        
        pizzaReader.nextLine();
        char topping;
        for(int i=0;i<pizzaRows;i++)
        {
            String pizzaRow = pizzaReader.nextLine();
            
            for(int j=0;j<pizzaColumns;j++)
            {
                topping = pizzaRow.charAt(j);
                Cell newCell = null;
                if(topping=='T')
                {
                    newCell = new Cell(1);
                }
                else if(topping=='M')
                {
                    newCell = new Cell(0);
                }
                
                pizza[i][j] = newCell;
            }
            
        }
        
        return pizza;
    }

}
