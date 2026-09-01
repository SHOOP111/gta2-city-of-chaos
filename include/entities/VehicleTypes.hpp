#pragma once

#include <string>
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

struct VehicleArchetype {
    VehicleType type = VehicleType::Sedan;
    std::string name = "Sedan";
    float length = 48.0f;
    float width = 24.0f;
    float mass = 1200.0f;
    float maxSpeed = 380.0f;
    float reverseMaxSpeed = 140.0f;
    float acceleration = 450.0f;
    float brakeForce = 600.0f;
    float turnSpeed = 2.8f;
    float driftGrip = 0.82f;       // Lateral friction retention
    float handbrakeGrip = 0.28f;    // Grip when handbraking
    float maxHealth = 300.0f;
    ColorRGBA primaryColor = {180, 40, 40, 255};
    ColorRGBA secondaryColor = {20, 20, 20, 255};
    bool hasSiren = false;
    bool isArmored = false;
    bool canCrushCars = false;
    int maxPassengers = 4;
};

inline VehicleArchetype getVehicleArchetype(VehicleType type) {
    VehicleArchetype a;
    a.type = type;
    switch (type) {
        case VehicleType::Sedan:
            a.name = "Wellard Sedan";
            a.length = 46.0f; a.width = 23.0f; a.mass = 1200.0f;
            a.maxSpeed = 360.0f; a.acceleration = 420.0f; a.turnSpeed = 2.7f;
            a.primaryColor = {65, 105, 225, 255};
            break;
        case VehicleType::SportsExotic:
            a.name = "Stinger GT";
            a.length = 44.0f; a.width = 24.0f; a.mass = 950.0f;
            a.maxSpeed = 540.0f; a.acceleration = 720.0f; a.turnSpeed = 3.6f;
            a.driftGrip = 0.75f; a.primaryColor = {230, 30, 30, 255};
            break;
        case VehicleType::MuscleCar:
            a.name = "B-Type Muscle";
            a.length = 48.0f; a.width = 25.0f; a.mass = 1450.0f;
            a.maxSpeed = 460.0f; a.acceleration = 580.0f; a.turnSpeed = 2.4f;
            a.driftGrip = 0.60f; a.primaryColor = {240, 160, 20, 255};
            break;
        case VehicleType::CopCruiser:
            a.name = "Police Squad Cruiser";
            a.length = 48.0f; a.width = 24.0f; a.mass = 1350.0f;
            a.maxSpeed = 480.0f; a.acceleration = 560.0f; a.turnSpeed = 3.0f;
            a.hasSiren = true;
            a.primaryColor = {240, 240, 240, 255}; a.secondaryColor = {20, 20, 20, 255};
            break;
        case VehicleType::SwatVan:
            a.name = "SWAT Enforcer";
            a.length = 56.0f; a.width = 28.0f; a.mass = 2800.0f;
            a.maxSpeed = 340.0f; a.acceleration = 360.0f; a.turnSpeed = 2.0f;
            a.maxHealth = 700.0f; a.hasSiren = true; a.isArmored = true;
            a.primaryColor = {30, 40, 50, 255};
            break;
        case VehicleType::FbiSedan:
            a.name = "FBI Interceptor";
            a.length = 48.0f; a.width = 24.0f; a.mass = 1400.0f;
            a.maxSpeed = 510.0f; a.acceleration = 640.0f; a.turnSpeed = 3.2f;
            a.hasSiren = true; a.maxHealth = 450.0f;
            a.primaryColor = {15, 15, 15, 255};
            break;
        case VehicleType::ArmyTank:
            a.name = "Rhino Tank";
            a.length = 68.0f; a.width = 36.0f; a.mass = 15000.0f;
            a.maxSpeed = 220.0f; a.acceleration = 280.0f; a.turnSpeed = 1.6f;
            a.maxHealth = 3000.0f; a.isArmored = true; a.canCrushCars = true;
            a.primaryColor = {68, 85, 48, 255};
            break;
        case VehicleType::Ambulance:
            a.name = "Medic Ambulance";
            a.length = 54.0f; a.width = 26.0f; a.mass = 2200.0f;
            a.maxSpeed = 400.0f; a.acceleration = 430.0f; a.turnSpeed = 2.2f;
            a.hasSiren = true; a.maxHealth = 500.0f;
            a.primaryColor = {250, 250, 250, 255}; a.secondaryColor = {220, 20, 20, 255};
            break;
        case VehicleType::FireTruck:
            a.name = "Fire Tender";
            a.length = 64.0f; a.width = 28.0f; a.mass = 4500.0f;
            a.maxSpeed = 350.0f; a.acceleration = 380.0f; a.turnSpeed = 1.9f;
            a.hasSiren = true; a.maxHealth = 900.0f;
            a.primaryColor = {220, 20, 20, 255}; a.secondaryColor = {240, 240, 240, 255};
            break;
        case VehicleType::Taxi:
            a.name = "Cab Taxi";
            a.length = 46.0f; a.width = 23.0f; a.mass = 1250.0f;
            a.maxSpeed = 370.0f; a.acceleration = 440.0f; a.turnSpeed = 2.8f;
            a.primaryColor = {245, 200, 10, 255}; a.secondaryColor = {20, 20, 20, 255};
            break;
        case VehicleType::VanDelivery:
            a.name = "Box Van";
            a.length = 52.0f; a.width = 26.0f; a.mass = 2000.0f;
            a.maxSpeed = 320.0f; a.acceleration = 340.0f; a.turnSpeed = 2.1f;
            a.primaryColor = {160, 160, 170, 255};
            break;
        case VehicleType::Bus:
            a.name = "City Transit Coach";
            a.length = 80.0f; a.width = 28.0f; a.mass = 6000.0f;
            a.maxSpeed = 280.0f; a.acceleration = 250.0f; a.turnSpeed = 1.5f;
            a.maxHealth = 1000.0f;
            a.primaryColor = {40, 120, 220, 255};
            break;
        case VehicleType::LoonieBuggy:
            a.name = "Loonie Dementia Buggy";
            a.length = 40.0f; a.width = 24.0f; a.mass = 800.0f;
            a.maxSpeed = 490.0f; a.acceleration = 680.0f; a.turnSpeed = 3.8f;
            a.driftGrip = 0.55f;
            a.primaryColor = {50, 205, 50, 255}; a.secondaryColor = {240, 240, 20, 255};
            break;
        case VehicleType::ZaibatsuZType:
            a.name = "Zaibatsu Z-Type";
            a.length = 50.0f; a.width = 25.0f; a.mass = 1350.0f;
            a.maxSpeed = 520.0f; a.acceleration = 650.0f; a.turnSpeed = 3.2f;
            a.maxHealth = 450.0f;
            a.primaryColor = {20, 20, 20, 255}; a.secondaryColor = {220, 20, 20, 255};
            break;
        case VehicleType::YakuzaMiara:
            a.name = "Yakuza Miara GT";
            a.length = 44.0f; a.width = 24.0f; a.mass = 1050.0f;
            a.maxSpeed = 510.0f; a.acceleration = 620.0f; a.turnSpeed = 3.4f;
            a.primaryColor = {30, 80, 220, 255}; a.secondaryColor = {240, 240, 240, 255};
            break;
        case VehicleType::PickupTruck:
            a.name = "Redneck Pickup";
            a.length = 50.0f; a.width = 25.0f; a.mass = 1700.0f;
            a.maxSpeed = 390.0f; a.acceleration = 480.0f; a.turnSpeed = 2.4f;
            a.primaryColor = {160, 82, 45, 255};
            break;
    }
    return a;
}

} // namespace gta2
