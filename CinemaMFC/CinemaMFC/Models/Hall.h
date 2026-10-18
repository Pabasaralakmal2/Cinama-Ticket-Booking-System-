#pragma once
#include <string>
class Hall {
private:
    std::string hallId; 
    std::string hallName;
    std::string projectionType;
    std::string soundSystem;
    bool supports3D;
    int capacity;
public:
    Hall() : supports3D(false), capacity(0) {}
    std::string GetHallId() const { return hallId; }
    std::string GetHallName() const { return hallName; }
    std::string GetProjectionType() const { return projectionType; }
    std::string GetSoundSystem() const { return soundSystem; }
    bool GetSupports3D() const { return supports3D; }
    int GetCapacity() const { return capacity; }
    void SetHallId(const std::string& id) { hallId = id; }
    void SetHallName(const std::string& name) { hallName = name; }
    void SetProjectionType(const std::string& type) { projectionType = type; }
    void SetSoundSystem(const std::string& system) { soundSystem = system; }
    void SetSupports3D(bool support) { supports3D = support; }
    void SetCapacity(int cap) { capacity = cap; }
};
