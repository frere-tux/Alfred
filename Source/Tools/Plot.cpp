#include "Plot.h"

using namespace Al;

Plot::Plot()
    : m_firstLine(false)
{
}

Plot::~Plot()
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void Plot::init()
{
    m_logFile.open("radioLog.dat");
    m_firstLine = true;
}

void Plot::stopLog()
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void Plot::logPulse(const unsigned int _pulse)
{
    if (m_logFile.is_open())
    {
        std::chrono::high_resolution_clock::time_point timer = std::chrono::high_resolution_clock::now();
        long long microsStart, microsEnd;

        if (m_firstLine)
        {
            m_t0 = timer - std::chrono::microseconds(_pulse);
            microsStart = 0;
            microsEnd = _pulse;
            m_firstLine = false;
        }
        else
        {
            auto elapsed = timer - m_t0;
            microsEnd = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

            elapsed -= std::chrono::microseconds(_pulse);
            microsStart = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }


        m_logFile << microsStart << ' ' << 0 << '\n';
        m_logFile << microsStart << ' ' << 1 << '\n';
        m_logFile << microsEnd << ' ' << 1 << '\n';
        m_logFile << microsEnd << ' ' << 0 << '\n';
    }
}

void Plot::plotLog()
{
    system("gnuplot -persistent ./plot.txt");
}


