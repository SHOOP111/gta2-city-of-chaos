#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace gta2 {

// Unique ID handles
using EntityID = uint32_t;
constexpr EntityID INVALID_ENTITY_ID = 0;

// Gang Factions
enum class GangFaction : uint8_t {
    None = 0,
    Zaibatsu,   // Corporate syndicate (Red 'Z', high-tech, sleek Z-Types)
    Loonies,    // Lunatics & Anarchists (Smiley face, green buggies, fire)
    Yakuza,     // Japanese mob (Blue Dragon, katanas, fast sports cars)
    Rednecks,   // Militia (Confederate flag, pickup trucks, shotguns)
    Scientists, // SRS Research (Gold shields, lightning weapons)
    Krishnas    // Orange robes, peace chants (special bonus multiplier)
};

// Wanted Level / Police Threat
enum class WantedLevel : uint8_t {
    Clean = 0,
    Star1, // 1-2 Foot patrol cops with nightsticks/pistols
    Star2, // Police cruisers patrol & chase
    Star3, // SWAT vans & road barricades with spike strips
    Star4, // SWAT assault units with body armor & M16s
    Star5, // FBI / Special Agents in black cruisers with SMGs
    Star6  // National Guard Army with Tanks & APCs
};

// Vehicle Class Categories
enum class VehicleType : uint8_t {
    Sedan = 0,
    SportsExotic,
    MuscleCar,
    CopCruiser,
    SwatVan,
    FbiSedan,
    ArmyTank,
    Ambulance,
    FireTruck,
    Taxi,
    VanDelivery,
    Bus,
    LoonieBuggy,
    ZaibatsuZType,
    YakuzaMiara,
    PickupTruck
};

// Weapon Types
enum class WeaponType : uint8_t {
    Fists = 0,
    Pistol,
    DualPistols,
    MicroSMG,
    Shotgun,
    AssaultRifle,
    RocketLauncher,
    Flamethrower,
    ElectroGun,
    Grenade,
    Molotov,
    VehicleMachineGun,
    VehicleOilSlick,
    VehicleBomb
};

// Pedestrian Roles & AI States
enum class PedestrianRole : uint8_t {
    Civilian = 0,
    GangMember,
    PoliceFoot,
    SwatOfficer,
    FbiAgent,
    ArmySoldier,
    Paramedic,
    Firefighter,
    Criminal
};

enum class PedestrianState : uint8_t {
    Idle = 0,
    Wandering,
    FleeingPanic,
    HostileAttacking,
    EnteringVehicle,
    InVehicle,
    Dead,
    Arresting,
    Reviving,
    Extinguishing
};

// Mission Tiers & States
enum class MissionTier : uint8_t {
    Green = 0, // Easy / Entry level (Low respect req)
    Yellow,    // Medium / Tactical (Mid respect req)
    Red        // Hard / All-out war (High respect req)
};

enum class MissionStatus : uint8_t {
    Available = 0,
    Active,
    Completed,
    Failed
};

enum class ObjectiveType : uint8_t {
    KillTarget = 0,
    DestroyVehicle,
    StealVehicle,
    DeliverToPoint,
    SurviveTime,
    TriggerKillFrenzy
};

// Game Application States
enum class GameState : uint8_t {
    TitleScreen = 0,
    Playing,
    MissionBriefing,
    Paused,
    Busted,
    Wasted,
    DistrictComplete
};

// Radio Stations
enum class RadioStation : uint8_t {
    Off = 0,
    HeadRadio,      // Modern Rock / Pop
    FuturoFM,       // Synthwave / Cyberpunk
    KGBH96,         // Hip-Hop / Breakbeats
    RadioZaibatsu,  // Corporate Ambient / Electronic
    HeavenlyVoices, // Choral / Orchestral
    LoonieLaughs    // Anarchy Punk / Talk Radio
};

// Tile Categories
enum class TileType : uint8_t {
    RoadAsphalt = 0,
    RoadMarking,
    Sidewalk,
    BuildingRoof,
    BuildingWall,
    Grass,
    Water,
    StuntRamp,
    TrainTracks,
    ResprayShopFloor,
    BombShopFloor
};

} // namespace gta2
