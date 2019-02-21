
public class Cell 
{
    private final int topping;
    
    private boolean allocated;
    
    public Cell(int topping)
    {
        if(topping!=1 && topping!=0)
        {
            throw new IllegalArgumentException();
        }
        
        this.topping = topping;
        allocated = false;
    }
    
    public int getTopping()
    {
        return topping;
    }
    
    public boolean isAllocated()
    {
        return allocated;
    }
    
    public void allocate()
    {
        if(allocated)
        {
            throw new IllegalArgumentException();
        }
        else
        {
            allocated = true;
        }
    }
}
