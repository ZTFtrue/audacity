/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  ClipSegment.h

  Matthieu Hodgkinson

**********************************************************************/
#pragma once

#include "AudioSegment.h"
#include "ClipTimeAndPitchSource.h"
#include "Observer.h"
#include "PlaybackDirection.h"

#include <memory>

class ClipInterface;
class TimeAndPitchInterface;

using PitchRatioChangeCbSubscriber =
   std::function<void(std::function<void(double)>)>;

/*!
 * It is important that objects of this class are instantiated and destroyed on
 * the same thread, due to the owned Observer::Subscription.
 */
class STRETCHING_SEQUENCE_API ClipSegment final : public AudioSegment
{
public:
   ClipSegment(ClipInterface&, double durationToDiscard, PlaybackDirection);

   // AudioSegment
   size_t GetFloats(float* const* buffers, size_t numSamples) override;
   bool Empty() const override;
   size_t GetWidth() const override;

private:
   const sampleCount mTotalNumSamplesToProduce;
   sampleCount mTotalNumSamplesProduced = 0;
   ClipTimeAndPitchSource mSource;
   // Careful that this guy is constructed last, as its ctor refers to *this.
   // todo(mhodgkinson) make this safe.
   std::unique_ptr<TimeAndPitchInterface> mStretcher;
   Observer::Subscription mOnSemitoneShiftChangeSubscription;
};
