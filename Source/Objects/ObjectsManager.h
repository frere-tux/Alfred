#pragma once

#include <AlTypes.h>
#include <string>
#include <map>

#include <Managers/Manager.h>

namespace Al
{
    struct Object
    {
        Object() : m_id(UNDEFINED_OBJECT_ID), m_groupId(0), m_elementId(0) {}

        Object(u8 _id, const std::string& _name, u32 _groupId, u32 _elementId)
            : m_id(_id)
            , m_groupId(_groupId)
            , m_elementId(_elementId)
            , m_name(_name)
        {}

        ~Object() {}

        u8    m_id;
        u32   m_groupId;
        u32   m_elementId;
        std::string m_name;
    };

    struct Room
    {
        Room() : m_id(UNDEFINED_ROOM_ID), m_groupId(0) {}

        Room(u8 _id, const std::string& _name, u32 _groupId)
            : m_id(_id)
            , m_groupId(_groupId)
            , m_name(_name)
        {}

        ~Room() {}

        u8    m_id;
        u32   m_groupId;
        std::string m_name;
        std::map<u32, Object> m_objects;
    };

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
