#pragma once

#include "CommonHeaders.h"

class TickTimer
{
public:
    TickTimer()
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        mTicksPerSecond = static_cast<uint64_t>(frequency.QuadPart);

        setStatsStride(2.0);

        reset();
    }

    ~TickTimer() { } 

    void reset()
    {
        LARGE_INTEGER curr;
        QueryPerformanceCounter(&curr);
        mLastTick = static_cast<uint64_t>(curr.QuadPart);

        mStatTicks = 0;
        mFrameCount = 0;
        mTotalTicks = 0;

        mAveragedFps = 0.0;
        mAveragedFrameTime = 0.;
    }

    double tick()
    {
        LARGE_INTEGER curr;
        QueryPerformanceCounter(&curr);
        uint64_t currTick = static_cast<uint64_t>(curr.QuadPart);

        // 'Time travel' might happen if previous tick() was called from another physical core
        uint64_t deltaTicks = (mLastTick > currTick) ? 0 : currTick - mLastTick;
        mLastTick = currTick;

        // Update stats
        mTotalTicks += deltaTicks;
        mStatTicks += deltaTicks;
        mFrameCount++;
        
        // Calculate stats 
        if (mStatTicks >= mStatStride)
        {
            mAveragedFrameTime = ticksToSeconds(mStatTicks) / mFrameCount;
            mAveragedFps = 1 / mAveragedFrameTime;

            mStatTicks = 0;
            mFrameCount = 0;
        }

        return ticksToSeconds(deltaTicks);
    }

    // How long before we update the stats(fps, frametime, etc)
    void setStatsStride(double seconds)
    {
        mStatStride = secondsToTicks(seconds);
    }

    double getAverageFps()
    {
        return mAveragedFps;
    }

    double getAverageFrameTime()
    {
        return mAveragedFrameTime;
    }

    inline double getTotalSeconds()
    {
        return ticksToSeconds(mTotalTicks);
    }

    inline double ticksToSeconds(uint64_t ticks)
    {
        return static_cast<double>(ticks) / mTicksPerSecond;
    }

    inline uint64_t secondsToTicks(double seconds)
    {
        return static_cast<uint64_t>(seconds * mTicksPerSecond);
    }

protected:
    uint64_t mTicksPerSecond;

    uint64_t mLastTick;

    uint64_t mTotalTicks;

    uint64_t mStatStride;
    uint64_t mStatTicks;
    uint64_t mFrameCount;

    double mAveragedFps;
    double mAveragedFrameTime;
};