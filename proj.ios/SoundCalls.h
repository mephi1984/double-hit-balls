#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#define ShareSoundCalls [SoundCalls shareInstance]

@interface SoundCalls : NSObject

+ (instancetype)shareInstance;

- (void)playBackgroundSound;
- (void)stopBackgroundSound;

- (void)playGunshotSound;
- (void)stopGunshotSound;

@end
