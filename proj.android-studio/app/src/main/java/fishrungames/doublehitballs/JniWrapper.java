package fishrungames.doublehitballs;

public class JniWrapper
{
    static {
        System.loadLibrary("DoubleHitBalls");
    }

   
    public static native void Init(int width, int height);

}