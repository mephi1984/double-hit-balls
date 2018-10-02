#import "Sounds.h"

@interface Sounds ()

@property (nonatomic, strong) AVAudioPlayer* backgroundPlayer;

@property (nonatomic, strong) AVAudioPlayer* gunshotPlayer;

@end

@implementation Sounds

static Sounds *_shareInstance;

+(instancetype)shareInstance {
    _shareInstance = [[super allocWithZone:NULL] init];
    return _shareInstance;
}

- (void)playBackgroundSound {
    NSString *soundFilePath = [[NSBundle mainBundle] pathForResource:@"background_sound" ofType:@"mp3"];
    NSURL *soundFileURL = [NSURL fileURLWithPath:soundFilePath];
    self.backgroundPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:soundFileURL error:nil];
    self.backgroundPlayer.numberOfLoops = -1; //Infinite
    self.backgroundPlayer.play;
}

- (void)stopBackgroundSound {
    if (self.backgroundPlayer != nil && self.backgroundPlayer.isPlaying) {
        self.backgroundPlayer.stop;
    }
}

- (void)playGunshotSound {
    NSString *soundFilePath = [[NSBundle mainBundle] pathForResource:@"gunshot_sound" ofType:@"mp3"];
    NSURL *soundFileURL = [NSURL fileURLWithPath:soundFilePath];
    self.gunshotPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:soundFileURL error:nil];
    self.gunshotPlayer.numberOfLoops = 0;
    self.gunshotPlayer.play;
}

- (void)stopGunshotSound {
    if (self.gunshotPlayer != nil && self.gunshotPlayer.isPlaying) {
        self.gunshotPlayer.stop;
    }
}

@end

