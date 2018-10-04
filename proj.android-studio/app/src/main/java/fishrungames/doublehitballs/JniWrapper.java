package fishrungames.doublehitballs;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class JniWrapper
{
    static Context context = null;

    public static void SetContext(Context context)
    {
        JniWrapper.context = context;
    }

    static {
        System.loadLibrary("DoubleHitBalls");
    }

    public static native void Init(int width, int height);
    public static native void SetJavaVM();

    public static String LoadFile(String fileName)
    {
        try {
            File file = new File(context.getFilesDir(), fileName);
            FileInputStream inputStream = new FileInputStream(file);
            byte[] data = new byte[(int) file.length()];
            inputStream.read(data);
            inputStream.close();
            return new String(data, "UTF-8");
        } catch (Exception e) {
            Log.d("SaveFile", "Failed to save a file.");
        }
        return "";
    }

    public static void SaveFile(String fileName, String fileContent)
    {
        try {
            File file = new File(context.getFilesDir(), fileName);
            FileOutputStream outputStream = new FileOutputStream(file);
            outputStream.write(fileContent.getBytes());
            outputStream.close();
        } catch (Exception e) {
            Log.d("SaveFile", "Failed to save a file.");
        }
    }
}