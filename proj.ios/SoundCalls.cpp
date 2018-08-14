#include "SoundCalls.h"
#include "BridgeDelegate.h"

void SoundCalls::playBackgroundSound()
{
    BridgeDelegate::playGunshotSound();
}

void SoundCalls::stopBackgroundSound()
{
    BridgeDelegate::stopBackgroundSound();
}

void SoundCalls::playGunshotSound()
{
    BridgeDelegate::playGunshotSound();
}

void SoundCalls::stopGunshotSound()
{
    BridgeDelegate::stopGunshotSound();
}
