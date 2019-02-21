
public class Slice implements Comparable<Slice>
{
    private final int startRow;
    
    private final int startColumn;
    
    private final int endRow;
    
    private final int endColumn;
    
    public Slice(int startRow, int startColumn, int endRow, int endColumn)
    {
        this.startRow = startRow;
        this.startColumn = startColumn;
        this.endRow = endRow;
        this.endColumn = endColumn;
    }
    
    public int getStartRow() 
    {
        return startRow;
    }

    public int getStartColumn() 
    {
        return startColumn;
    }

    public int getEndRow() 
    {
        return endRow;
    }

    public int getEndColumn() 
    {
        return endColumn;
    }
    
    public int getRows()
    {
        return (endRow - startRow + 1);
    }
    
    public int getColumns()
    {
        return (endColumn - startColumn + 1);
    }
    
    public int getSize()
    {
        return getRows() * getColumns();
    }

    public String toString()
    {
        String slice = startRow + " " + startColumn + " " + endRow + " " + 
                endColumn;
        return slice;
    }
    
    public int compareTo(Slice otherSlice)
    {
        int difference = getSize() - otherSlice.getSize();
        if (difference > 0) 
        {
            return -1;
        }
        else if (difference < 0) 
        {
            return 1;
        }
        else 
        {
            return 0;
        }
    }
}
