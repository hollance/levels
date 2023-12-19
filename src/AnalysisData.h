#pragma once

struct Measurement
{
    void reset() noexcept
    {
        value = 0.0f;
    }

    void update(float newValue) noexcept
    {
        auto oldValue = value.load();
        while (newValue > oldValue && !value.compare_exchange_weak(oldValue, newValue));
    }

    float read() noexcept
    {
        return value.exchange(0.0f);
    }

    std::atomic<float> value;
};

struct AnalysisData
{
    void reset() noexcept
    {
        levelL.reset();
        levelR.reset();
        levelM.reset();
        levelS.reset();
    }

    Measurement levelL;
    Measurement levelR;
    Measurement levelM;
    Measurement levelS;
};
