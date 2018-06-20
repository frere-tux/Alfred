#pragma once

#include <AlTypes.h>
#include <string>
#include <map>

#include <Managers/Manager.h>
#include "Object.h"

namespace Al
{
    class ObjectsManager : public Manager
    {
    public:
        ObjectsManager() {}
        ~ObjectsManager() {}

        virtual void Init()  override;

        const Object* GetObject(const u32 _roomId, u32 _objectId);


    private:

        std::map<u32, Room> m_rooms;

    };
}
