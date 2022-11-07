#include <ProcessorHeaders.h>

#include "TTLCondTrigCircBuf.h"
#include "TTLCondTrigLogic.h"

using namespace TTLConditionTrig;


//
// Configuration for processing conditions on one signal.


// Constructor.
ConditionConfig::ConditionConfig()
{
    // Initialize to safe defaults.
    clear();
}


// Default destructor is fine.


// This sets a known-sane configuration state.
void ConditionConfig::clear()
{
    // Set sane defaults.

    desiredFeature = FeatureType::levelHigh;

    delayMinSamps = 0;
    delayMaxSamps = 0;
    sustainSamps = 10;
    deadTimeSamps = 100;
    deglitchSamps = 0;

    outputActiveHigh = true;
}


// This forces configuration parameters to be valid and self-consistent.
void ConditionConfig::forceSanity()
{
    if ( (desiredFeature < ConditionConfig::levelHigh) || (desiredFeature > ConditionConfig::edgeFalling) )
        desiredFeature = ConditionConfig::levelHigh;

    if (delayMinSamps < 0)
        delayMinSamps = 0;

    if (delayMaxSamps < delayMinSamps)
        delayMaxSamps = delayMinSamps;

    if (sustainSamps < 1)
        sustainSamps = 1;

    if (deadTimeSamps < 0)
        deadTimeSamps = 0;

    if (deglitchSamps < 0)
        deglitchSamps = 0;
}


//
// Condition processing for one TTL signal.


// Constructor.
ConditionProcessor::ConditionProcessor()
{
    // The constructor should already have done this, but do it anyways.
    config.clear();

    // Initialize. Use a dummy timestamp and input level.
    resetInput(0, false);
    resetState();
}


// Configuration accessors.

void ConditionProcessor::setConfig(ConditionConfig &newConfig)
{
    config = newConfig;

    // NOTE - We're clearing pending output state but keeping the record of previous input.
    resetState();
}


ConditionConfig ConditionProcessor::getConfig()
{
    return config;
}


// State reset. This clears active events after a configuration change.
void ConditionProcessor::resetState()
{
    pendingOutputTimes.clear();
    pendingOutputLevels.clear();
    prevAcknowledgedOutput = !(config.outputActiveHigh);
}


// This overwrites our record of the previous input levels without causing an event update.
// This is used for initialization at the start of acquisition.
void ConditionProcessor::resetInput(int64 resetTime, bool newInput)
{
    prevInputTime = resetTime;
    prevInputLevel = newInput;
}


// Input processing. This schedules future output in response to input events.
void ConditionProcessor::handleInput(int64 inputTime, bool inputLevel)
{
// FIXME - handleInput NYI.

    // Update the "last input seen" record.
    prevInputTime = inputTime;
    prevInputLevel = inputLevel;
}


// State accessors.

bool ConditionProcessor::hasPendingOutput()
{
    return (pendingOutputTimes.count() > 0);
}


int64 ConditionProcessor::getNextOutputTime()
{
    // NOTE - This will return a safe value (0) if we don't have output.
    return pendingOutputTimes.snoop();
}


bool ConditionProcessor::getNextOutputLevel()
{
    // NOTE - This will return a safe value (false) if we don't have output.
    return pendingOutputLevels.snoop();
}


// This removes the next queued output event, after we've read it.
void ConditionProcessor::acknowledgeOutput()
{
    // Save whatever the last output was.
    // This will return a safe value (false) if we don't have pending output.
    prevAcknowledgedOutput = pendingOutputLevels.snoop();

    // Discard return values.
    pendingOutputTimes.dequeue();
    pendingOutputLevels.dequeue();
}


// Display polling accessors.

bool ConditionProcessor::getLastInput()
{
    return prevInputLevel;
}


bool ConditionProcessor::getLastAcknowledgedOutput()
{
    return prevAcknowledgedOutput;
}


// This is the end of the file.
