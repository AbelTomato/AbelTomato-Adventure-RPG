#include "core/game_core.hpp"
using json = nlohmann::ordered_json;

json make_error_response(
    const std::string& request_id,
    const std::string& code,
    const std::string& message,
    const json& details)
{
    json response = {
        {"ok", false},
        {"version", 1},
        {"request_id", request_id},
        {"state", nullptr},
        {"events", json::array()},
        {"error", {
            {"code", code},
            {"message", message},
            {"details", details}
        }}
    };

    return response;
}
json make_success_response(
    const std::string& request_id,
    int version,
    const json& state,
    const json& events)
{
    json response;
    response["ok"] = true;
    response["version"] = version;
    response["request_id"] = request_id;
    response["state"]=state;
    response["events"]= events;
    response["error"]= nullptr;
    return response;
}

json* find_entity(json& entities, const std::string& entity_id)
{
    for (auto& entity : entities) {
        std::string id = entity.value("id", "");

        if (id == entity_id) {
            return &entity;
        }
    }

    return nullptr;
}

json handle_attack(const json& input)
{

    std::string request_id =input.value("request_id","");
    int version = input.value("version",-1);
    json state = input.at("state");
    json &entities = state.at("entities");
    json action = input.at("action");
    std::string source_id = action.value("actor_id","");
    std::string target_id= action.value("target_id","");
    std::string damage_type = "physical";


    json* actor = find_entity(entities, source_id);
    json* target = find_entity(entities, target_id);

    //return error code
    if (actor == nullptr) {
        return make_error_response(
            request_id,
            "ENTITY_NOT_FOUND",
            "Actor entity not found",
            {{"actor_id", source_id}}
        );
    }
    
    if (target == nullptr) {
        return make_error_response(
            request_id,
            "ENTITY_NOT_FOUND",
            "Target entity not found",
            {{"target_id", target_id}}
        );
    }


    int phy_attack = actor->at("attrs").value("physical_attack",0);
    int phy_defense = target->at("attrs").value("physical_defense",0);
    int hp_before = target->at("attrs").value("hp",0);
    int damage_val = std::max(1,phy_attack - phy_defense);
    int hp_after = std::max(0,hp_before-damage_val);
    (*target)["attrs"]["hp"] = hp_after;
    (*target)["status"]["alive"] = (*target)["attrs"]["hp"] == 0? false:true;

    json event_attack_start,damage,hp_changed;
    event_attack_start["seq"]=1;
    event_attack_start["type"]="attack_started";
    event_attack_start["source_id"]=source_id;
    event_attack_start["target_id"]=target_id;
    event_attack_start["tags"]=json::array();
    event_attack_start["tags"].push_back("physical");
    event_attack_start["tags"].push_back("melee");

    damage["seq"]=2;
    damage["type"]="damage";
    damage["source_id"]=source_id;
    damage["target_id"]=target_id;
    damage["value"] = damage_val;
    //damage_type后期需要自己在json中找
    damage["damage_type"] =damage_type;
    //message同样自己拼接
    damage["message"] = ("Abel 对史莱姆造成 18 点物理伤害");

    hp_changed["seq"]=3;
    hp_changed["type"]="hp_changed";
    hp_changed["entity_id"] = target_id;
    hp_changed["from"] = hp_before;
    hp_changed["to"]= hp_after;
    json events = json::array();
    events.push_back(event_attack_start);
    events.push_back(damage);
    events.push_back(hp_changed);

    if(hp_after == 0)
    {
        json death;
        death["seq"] = 4;
        death["type"] = "death";
        death["entity_id"] = target_id;
        events.push_back(death);
    }

    return make_success_response(request_id,version,state,events);

}
json handle_request(const json &input)
{
    json action = input.at("action");
    std::string action_type = action.value("type","");
    std::string request_id = input.value("request_id","");
    //这里应该是设计一个枚举 管理action_type
    if(action_type != "attack")
    {
        return make_error_response(
            request_id,
            "INVALID_ACTION",
            "Unsupported action type",
            {{"action_type", action_type}}
        );
    }
    int version = input.value("version",-1);

    if(version!=1)
    {
        return make_error_response(request_id,
            "UNSUPPORTED_VERSION",
            "Unsupported API contract version",
            {{"version",version}}
        );
    }

    else
    {
        return handle_attack(input);
    }
}

std::string handle_json(const std::string& input_json)
{
    try {
        json input = json::parse(input_json);
        json response=handle_request(input);
        return response.dump();
    }catch (const json::parse_error&) {
    return make_error_response(
            "",
            "INVALID_JSON",
            "Invalid JSON input",
            json::object()
        ).dump();
    }
    catch (const json::exception&) {
    return make_error_response(
        "",
        "INVALID_SCHEMA",
        "Request JSON does not match API contract",
        json::object()
    ).dump();
    }
    catch (const std::exception&) {
    return make_error_response(
        "",
        "INTERNAL_ERROR",
        "Internal C++ core error",
        json::object()
    ).dump();
    }
}
    
