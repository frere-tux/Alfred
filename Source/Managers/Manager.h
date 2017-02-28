#pragma once

namespace Al
{
    class Manager
    {
    protected:
        Manager() {}

    public:
        static Manager& getInstance()
        {
            static Manager instance;
            return instance;
        }

        virtual ~Manager() {}

        Manager(const Manager&) = delete;
        void operator=(const Manager&) = delete;

        virtual void Init() {}

        static void InitManagers();
    };
}
