#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#define ShareSounds [Sounds shareInstance]

@interface Sounds : NSObject

+ (instancetype)shareInstance;

- (void)playBackgroundSound;
- (void)stopBackgroundSound;

- (void)playGunshotSound;
- (void)stopGunshotSound;

@end
