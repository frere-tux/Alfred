#include "ObjectsManager.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include <Debug/DebugManager.h>

namespace Al
{

void ObjectsManager::Init()
{
    FILE * pFile = fopen ("/home/pi/Projects/Alfred/Data/Objects.dat" , "r");

    char buffer[65536];

    rapidjson::FileReadStream fileStream(pFile, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream<0>(fileStream);

    assert(document.IsObject());
    assert(document.HasMember("rooms"));

    const rapidjson::Value& rooms = document["rooms"];
    assert(rooms.IsArray());

    g_DebugManager->addLog(LogType_Message, "-------------------OBJECTS-------------------");

    for (rapidjson::Value::ConstValueIterator itrRooms = rooms.Begin(); itrRooms != rooms.End(); ++itrRooms)
    {
        const rapidjson::Value& roomValue = *itrRooms;
        assert(roomValue.IsObject());

        const rapidjson::Value& roomId = roomValue["id"];
        assert(roomId.IsUint());

        const rapidjson::Value& roomName = roomValue["name"];
        assert(roomName.IsString());

        const rapidjson::Value& roomGroupId = roomValue["groupId"];
        assert(roomGroupId.IsUint());

        const rapidjson::Value& roomObjects = roomValue["objects"];
        assert(roomObjects.IsArray());

        Room room(roomId.GetUint(), roomName.GetString(), roomGroupId.GetUint());

        g_DebugManager->addLog(LogType_Message, "%s (id: %u - groupId: %u)", room.m_name.c_str(), room.m_id, room.m_groupId);

        for (rapidjson::Value::ConstValueIterator itrObjects = roomObjects.Begin(); itrObjects != roomObjects.End(); ++itrObjects)
        {
            const rapidjson::Value& objectValue = *itrObjects;
            assert(objectValue.IsObject());

            const rapidjson::Value& objectId = objectValue["id"];
            assert(objectId.IsUint());

            const rapidjson::Value& objectName = objectValue["name"];
            assert(objectName.IsString());

            const rapidjson::Value& objectElementId = objectValue["elementId"];
            assert(objectElementId.IsUint());

            g_DebugManager->addLog(LogType_Message, "    %s (id: %u, elementId: %u)", objectName.GetString(), objectId.GetUint(), objectElementId.GetUint());

            assert(room.m_objects.find(objectId.GetUint()) == room.m_objects.end());

            room.m_objects[objectId.GetUint()] = Object(objectId.GetUint(), objectName.GetString(), room.m_groupId, objectElementId.GetUint());
        }

        assert(m_rooms.find(room.m_id) == m_rooms.end());

        m_rooms[room.m_id] = room;

        g_DebugManager->addLog(LogType_Message, "               ---------------");
    }

    g_DebugManager->addLog(LogType_Message, "---------------------------------------------");
}

const Object* ObjectsManager::GetObject(const u32 _roomId, u32 _objectId)
{
    auto room = m_rooms.find(_roomId);
    if (room != m_rooms.end())
    {
        auto object = room->second.m_objects.find(_objectId);
        if (object != room->second.m_objects.end())
        {
            return &object->second;
        }
    }

    return nullptr;
}

}
