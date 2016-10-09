#pragma once

#include <fstream>
#include <chrono>

namespace Al
{
    class Plot
    {
    public:
        Plot();
        ~Plot();

        void init();
        void logPulse(const unsigned int _pulse);
        void plotLog();

    private:
        bool            m_firstLine;
        std::ofstream   m_logFile;
        std::chrono::high_resolution_clock::time_point m_t0;
    };
}
