package fishrungames.doublehitballs;

import fishrungames.engine.FileWrapper;

//Deprecated
//import fishrungames.doublehitballs.R;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;

//Deprecated
//import java.lang.reflect.Field;


public class GL2JNIActivity extends Activity
{
	
	GLView mView;
	
	boolean IsScrolling = false;
	
	private GestureDetector gestureDetector;
	
	@Override
	protected void onCreate(Bundle icicle)
	{
		
		super.onCreate(icicle);
		
		gestureDetector = new GestureDetector(new MyGestureListener());
		
		FileWrapper.LoadHalibutEngineLibrary();
		FileWrapper.SetActivityInstance(this);
		FileWrapper.SetupEnviroment();
		/*
		 * Deprecated		
		 try
		 
		{
			for (Field f : R.raw.class.getFields())
			{
				FileWrapper.AddToFileMap(f.getName(), f.getInt(null));
			}
		
		} catch (IllegalArgumentException e)
		{
			FileWrapper.ConsoleOut("IllegalArgumentException\n");
			onStop();
		} catch (IllegalAccessException e)
		{
			FileWrapper.ConsoleOut("IllegalAccessException\n");
			onStop();
		}*/
		
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = this.getPackageManager();
		try {
		        appInfo = packMgmr.getApplicationInfo("fishrungames.doublehitballs", 0);
		    } catch (NameNotFoundException e) {
		 
		 e.printStackTrace();
		throw new RuntimeException("Unable to locate assets, aborting...");
		    }
		apkFilePath = appInfo.sourceDir;
		
		FileWrapper.SetupApkFilePath(apkFilePath);
		
		mView = new GLView(getApplication());
		
		setContentView(mView);
		
	}
	
	@Override
	protected void onPause()
	{
		FileWrapper.ConsoleOut("OnPause\n");
		JniWrapper.StopSounds();
		super.onPause();
		mView.onPause();
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		mView.onResume();
	}
	
	@Override
	protected void onStop()
	{
		
		//FileWrapper.ConsoleOut("OnStop\n");
		//StopSounds();
		super.onStop();
	}
	
	public boolean onTouchEvent (MotionEvent event)
	{
		if (gestureDetector.onTouchEvent(event))
		{
			return true;
		}
		
		if (event.getAction() == MotionEvent.ACTION_UP)
		{
			float x = event.getX();
    		float y = (float)mView.getHeight()-event.getY();
			
			if(IsScrolling) 
			{	
				IsScrolling  = false;
            }
			
			JniWrapper.OnTapUp(x, y, event.getEventTime());
			
		}
		return true;
    }

	class MyGestureListener extends SimpleOnGestureListener 
	{
        @Override
        public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) 
        {
        	JniWrapper.OnFling(velocityX, velocityY, e2.getEventTime());
            return true;
        }
        
        public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
        {
        	JniWrapper.OnScroll(distanceX, distanceY, e2.getEventTime());
        	IsScrolling = true;
        	return true;
        }
             
        public boolean onDown(MotionEvent event)
        {
        	
        	float x = event.getX();
    		float y = (float)mView.getHeight()-event.getY();
    		
    		JniWrapper.OnTapDown(x, y, event.getEventTime());
    		
    		return true;
        }
      
	}

}