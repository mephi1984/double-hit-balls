package fishrungames.doublehitballs;

import fishrungames.salmonengineandroid.EngineWrapper;
//Deprecated
//import fishrungames.doublehitballs.R;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;

//Deprecated
//import java.lang.reflect.Field;


public class GL2JNIActivity extends Activity
{

	private static GL2JNIActivity instance;
	
	GLView mView;
	
	@Override
	protected void onCreate(Bundle icicle)
	{
		
		super.onCreate(icicle);

		instance = this;

		EngineWrapper.LoadSalmonEngineLibrary();
		EngineWrapper.SetActivityInstance(this);
		EngineWrapper.SetupEnviroment();
		JniWrapper.SetJavaVM();
		JniWrapper.SetContext(this);
		
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = this.getPackageManager();
		try {
		        appInfo = packMgmr.getApplicationInfo("fishrungames.DoubleHitBalls", 0);
		    } catch (NameNotFoundException e) {
		 
		 e.printStackTrace();
	
		throw new RuntimeException("Unable to locate assets, aborting...");
		    }
		apkFilePath = appInfo.sourceDir;
		
		EngineWrapper.SetupApkFilePath(apkFilePath);
		
		mView = new GLView(getApplication());
		
		setContentView(mView);
		
		EngineWrapper.SetView(mView);
		
	}
	
	@Override
	protected void onPause()
	{
		EngineWrapper.CallDestroy();
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
		super.onStop();
	}
	
	public boolean onTouchEvent (MotionEvent event)
	{
		EngineWrapper.ProcessTouchEvent(event);
		return true;
    }

	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		EngineWrapper.ProcessKeyDown(keyCode, event);
		return super.onKeyDown(keyCode, event);
	}

	public static GL2JNIActivity getInstance() {
		if (instance == null) {
			throw new RuntimeException("error GL2JNIActivity getInstance() - you are trying to get activity instance when it is not created or already destroyed");
		}
		return instance;
	}

}