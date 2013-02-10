package fishrungames.doublehitballs;

public class JniWrapper
{
    static {
        System.loadLibrary("gl2jni");
    }

   
    public static native void Init(int width, int height);

}