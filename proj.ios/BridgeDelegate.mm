#include "BridgeDelegate.h"
#import "Sounds.h"

void BridgeDelegate::playBackgroundSound()
{
    [ShareSounds playBackgroundSound];
}

void BridgeDelegate::stopBackgroundSound()
{
    [ShareSounds stopBackgroundSound];
}

void BridgeDelegate::playGunshotSound()
{
    [ShareSounds playGunshotSound];
}

void BridgeDelegate::stopGunshotSound()
{
    [ShareSounds stopGunshotSound];
}
