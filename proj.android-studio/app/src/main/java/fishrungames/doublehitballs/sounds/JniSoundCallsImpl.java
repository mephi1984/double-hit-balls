package fishrungames.doublehitballs.sounds;

import android.media.MediaPlayer;

import fishrungames.doublehitballs.GL2JNIActivity;
import fishrungames.doublehitballs.R;

/**
 * example sounds for JNI calls
 * @author Artem Budarin
 * Created by Artem Budarin on 06.08.2018.
 */

public class JniSoundCallsImpl implements JniSoundCalls {

    private MediaPlayer backgroundPlayer = null;
    private MediaPlayer gunshotPlayer = null;

    @Override
    public void playBackgroundSound() {
        backgroundPlayer = MediaPlayer.create(GL2JNIActivity.getInstance(), R.raw.background_sound);
        backgroundPlayer.setLooping(true);
        backgroundPlayer.start();
    }

    @Override
    public void stopBackgroundSound() {
        if (backgroundPlayer != null) {
            backgroundPlayer.stop();
        }
    }

    @Override
    public void playGunshotSound() {
        gunshotPlayer = MediaPlayer.create(GL2JNIActivity.getInstance(), R.raw.gunshot_sound);
        gunshotPlayer.setLooping(false);
        gunshotPlayer.start();
    }

    @Override
    public void stopGunshotSound() {
        if (gunshotPlayer != null) {
            gunshotPlayer.stop();
        }
    }

}
