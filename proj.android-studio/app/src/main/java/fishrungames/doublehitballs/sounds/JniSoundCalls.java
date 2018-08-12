package fishrungames.doublehitballs.sounds;

import android.media.MediaPlayer;

import fishrungames.doublehitballs.GL2JNIActivity;
import fishrungames.doublehitballs.R;

/**
 * example sounds for JNI calls
 * @author Artem Budarin
 * Created by Artem Budarin on 06.08.2018.
 */

public class JniSoundCalls {

    native public static void initJniSounds();

    private static MediaPlayer backgroundPlayer = null;
    private static MediaPlayer gunshotPlayer = null;

    public static void init() {
        initJniSounds();
    }

    public static void JniPlayBackgroundSound() {
        backgroundPlayer = MediaPlayer.create(GL2JNIActivity.getInstance(), R.raw.background_sound);
        backgroundPlayer.setLooping(true);
        backgroundPlayer.start();
    }

    public static void JniStopBackgroundSound() {
        if (backgroundPlayer != null) {
            backgroundPlayer.stop();
        }
    }

    public static void JniPlayGunshotSound() {
        gunshotPlayer = MediaPlayer.create(GL2JNIActivity.getInstance(), R.raw.gunshot_sound);
        gunshotPlayer.setLooping(false);
        gunshotPlayer.start();
    }

    public static void JniStopGunshotSound() {
        if (gunshotPlayer != null) {
            gunshotPlayer.stop();
        }
    }

}
