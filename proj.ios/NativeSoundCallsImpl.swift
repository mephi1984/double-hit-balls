import Foundation
import AVFoundation

//  example sounds for native calls
//  Created by Artem Budarin on 08.08.2018.

public class NativeSoundCallsImpl : NativeSoundCalls {
    
    var backgroundPlayer: AVAudioPlayer?
    var gunshotPlayer: AVAudioPlayer?
    
    func playBackgroundSound() {
        guard let url = Bundle.main.url(forResource: "background_sound", withExtension: "mp3") else { return }
        do {
            try AVAudioSession.sharedInstance().setCategory(AVAudioSessionCategoryPlayback)
            try AVAudioSession.sharedInstance().setActive(true)
            backgroundPlayer = try AVAudioPlayer(contentsOf: url)
            
            //swift 4
            //backgroundPlayer = try AVAudioPlayer(contentsOf: url, fileTypeHint: AVFileType.mp3.rawValue)
            
            backgroundPlayer?.numberOfLoops = NSInteger.max
            backgroundPlayer?.play()
        } catch let error {
            print(error.localizedDescription)
        }
    }
    
    func stopBackgroundSound() {
        backgroundPlayer?.stop()
    }
    
    func playGunshotSound() {
        guard let url = Bundle.main.url(forResource: "gunshot_sound", withExtension: "mp3") else { return }
        do {
            try AVAudioSession.sharedInstance().setCategory(AVAudioSessionCategoryPlayback)
            try AVAudioSession.sharedInstance().setActive(true)
            gunshotPlayer = try AVAudioPlayer(contentsOf: url)
            
            //swift 4
            //gunshotPlayer = try AVAudioPlayer(contentsOf: url, fileTypeHint: AVFileType.mp3.rawValue)
            
            gunshotPlayer?.play()
        } catch let error {
            print(error.localizedDescription)
        }
    }
    
    func stopGunshotSound() {
        gunshotPlayer?.stop()
    }
    
}

