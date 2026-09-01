# GTA 2: City of Chaos (Modern C++ Top-Down Open World Engine)

[![Build and Test](https://github.com/SHOOP111/gta2-city-of-chaos/actions/workflows/ci.yml/badge.svg)](https://github.com/SHOOP111/gta2-city-of-chaos/actions/workflows/ci.yml)
[![Release](https://github.com/SHOOP111/gta2-city-of-chaos/actions/workflows/release.yml/badge.svg)](https://github.com/SHOOP111/gta2-city-of-chaos/actions/workflows/release.yml)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

```
  ██████╗ ████████╗ █████╗     ██████╗      ██████╗██╗████████╗██╗   ██╗     ██████╗ ███████╗     ██████╗██╗  ██╗ █████╗  ██████╗ ███████╗
 ██╔════╝ ╚══██╔══╝██╔══██╗    ╚════██╗    ██╔════╝██║╚══██╔══╝╚██╗ ██╔╝    ██╔═══██╗██╔════╝    ██╔════╝██║  ██║██╔══██╗██╔═══██╗██╔════╝
 ██║  ███╗   ██║   ███████║     █████╔╝    ██║     ██║   ██║    ╚████╔╝     ██║   ██║█████╗      ██║     ███████║███████║██║   ██║███████╗
 ██║   ██║   ██║   ██╔══██║    ██╔═══╝     ██║     ██║   ██║     ╚██╔╝      ██║   ██║██╔══╝      ██║     ██╔══██║██╔══██║██║   ██║╚════██║
 ╚██████╔╝   ██║   ██║  ██║    ███████╗    ╚██████╗██║   ██║      ██║       ╚██████╔╝██║         ╚██████╗██║  ██║██║  ██║╚██████╔╝███████║
  ╚═════╝    ╚═╝   ╚═╝  ╚═╝    ╚══════╝     ╚═════╝╚═╝   ╚═╝      ╚═╝        ╚═════╝ ╚═╝          ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝
```

An ultra-advanced, high-performance, feature-rich C++20 recreation and evolution of the legendary **Grand Theft Auto 2 (1999)** top-down open-world formula. Built from the ground up with modular architecture, custom vehicle physics, gang respect dynamics, dynamic 6-star law enforcement AI, multi-layered city tilemap, procedural audio synthesis, day/night lighting, and complete GitHub Actions CI/CD automation.

---

## 🌟 Key Highlights & Features

### 🏢 1. Living Open-World City (Downtown, Residential, Industrial)
- **Multi-layered Tilemap Engine**: Dynamic terrain with roads, sidewalks, pedestrian crossings, elevated expressways, secret stunt ramps, water hazards, and destructible props.
- **Dynamic 2.5D Lighting & Day/Night Cycle**: Real-time ambient illumination, streetlights activating at dusk, vehicle headlights casting dynamic directional light cones, and pulsating neon gang signs.
- **AI Road Traffic & Waypoint Graph**: Smart traffic navigation with lane discipline, traffic light obedience, pedestrian crossing yielding, honking, and emergency vehicle siren reactions (pulling over to the shoulder).

### 👥 2. Dynamic Gang Respect & Faction System
- **3 Rival Factions per District**:
  - 👔 **The Zaibatsu Corporation**: Corporate syndicate wielding high-tech weapons and executive cruisers.
  - 🤪 **The Loonies**: Anarchist asylum escapees driving custom green hotrods and loving explosive chaos.
  - 🐉 **The Yakuza**: High-discipline syndicate controlling casinos and industrial docks with deadly katanas and sports sedans.
- **Respect Matrix (`-100` to `+100`)**: Wiping out one gang boosts respect with their rivals. Gangs will salute you when respect is high, or launch shoot-on-sight drive-by hits when respect is negative.
- **Payphone Mission Dispatchers**: Color-coded gang payphones across territories unlocking Green (Tier 1), Yellow (Tier 2), and Red (Tier 3) high-stakes missions.

### 🚔 3. 6-Star Dynamic Law Enforcement AI
- ⭐ **1 Star**: Beat cops on foot attempting arrest with nightsticks/pistols.
- ⭐⭐ **2 Stars**: Fast patrol squad cars chasing, boxing in, and performing PIT maneuvers.
- ⭐⭐⭐ **3 Stars**: SWAT tactical vans and barricaded road blocks with spike strips.
- ⭐⭐⭐⭐ **4 Stars**: Heavily armored SWAT tactical assault squads with assault rifles and body armor.
- ⭐⭐⭐⭐⭐ **5 Stars**: Undercover Special Agents in high-speed black sedans wielding rapid-fire SMGs.
- ⭐⭐⭐⭐⭐⭐ **6 Stars**: State National Guard deployment with Armored Personnel Carriers and heavy Tanks firing explosive artillery!
- 🚗 **Pay 'N' Spray Respray Shops**: Escape the heat by repainting and repairing your ride.

### 🏎️ 4. Arcade Vehicle Physics & Weaponized Rides
- **16+ Distinct Vehicle Archetypes**: Sedans, Muscle Cars, Cop Cruisers, SWAT Vans, Fire Trucks, Ambulances, Tanks, Sports Exotics, Delivery Vans, and Buses.
- **Realistic Top-Down Vehicle Dynamics**: Weight transfer, tire friction & slip angle, handbrake drift mechanics, realistic suspension bounce, smoke trails, and skid marks.
- **Vehicle Weapon Upgrades**: Machine gun grilles, dual oil slicks, mine droppers, and vehicle bombs (ignition detonator or radio remote).
- **Carjacking & Emergency Vehicles**: Steal any car in real time; drive ambulances to revive downed peds or fire trucks with high-pressure water cannons.

### 🔫 5. Destructive Weapon Arsenal
- 👊 **Fists & Baseball Bat**: Close-quarters melee takedowns.
- 🔫 **Pistols & Dual Pistols**: Reliable rapid sidearms.
- ⚡ **Micro-SMG / Uzi**: High rate of fire with full drive-by shooting capability from vehicles.
- 💥 **Pump Shotgun**: Brutal close-range multi-pellet spread.
- 🎯 **Assault Rifle (M16)**: High-velocity long-range armor penetration.
- 🚀 **Rocket Launcher (RPG)**: High-yield explosive demolition.
- 🔥 **Flamethrower**: Dynamic propagating fire that ignites terrain, vehicles, and pedestrians.
- ⚡ **Electro-Gun**: High-voltage electrical arcs chaining between nearby targets.
- 💣 **Grenades & Molotov Cocktails**: Throwable explosives with arc trajectory, bounce physics, and fuse timers.

### 📻 6. Procedural Audio Synth & In-Car Radio
- **Built-in Procedural Audio Synthesizer**: Zero external audio dependencies required! Synthesizes engine roars, screeching tires, gunshots, explosions, sirens, and radio jingles in real time.
- **Dynamic Radio Stations**: Switch between 6 radio stations (Rock, Electronic, Hip-Hop, Jazz, Talk Radio, Classical) while driving.

### 🤖 7. Autonomous GitHub Actions CI/CD (Zero Local Build Requirement)
- **Multi-Platform CI Matrix**: Automated compilation and unit testing on **Ubuntu Linux (GCC & Clang)**, **Windows (MSVC)**, and **macOS (AppleClang)**.
- **Headless Unit & Simulation Test Suite**: Runs physics, AI state machines, weapon ballistics, gang respect logic, and 60-tick headless game loops inside GitHub Actions.
- **Release Automation**: Generates multi-platform standalone binary packages automatically upon release tag creation.

---

## 🎮 Controls

| Action | Primary Key | Secondary / Controller |
|---|---|---|
| **Move / Steer** | `W`, `A`, `S`, `D` or `Arrow Keys` | Left Thumbstick / D-Pad |
| **Accelerate / Reverse** | `W` (Gas), `S` (Brake/Reverse) | Right Trigger / Left Trigger |
| **Handbrake Drift** | `Spacebar` | `A` Button / `R1` |
| **Fire Weapon / Horn** | `Left Ctrl` or `Left Mouse Button` | Right Bumper / `X` Button |
| **Enter / Exit Vehicle** | `Enter` or `F` | `Y` Button / `Triangle` |
| **Next / Previous Weapon** | `Q` / `E` or `Mouse Scroll Wheel` | `D-Pad Left` / `D-Pad Right` |
| **Next Radio Station** | `R` | `D-Pad Up` |
| **Special Vehicle Action** | `Left Shift` (Siren / Nitro / Oil Slick) | Left Bumper / `L3` |
| **Pause / Menu** | `Escape` | `Start` Button |

---

## 🏗️ Architecture & Codebase Design

The project is structured with strict separation of concerns using modern C++20 principles:

```
gta2-city-of-chaos/
├── include/
│   ├── core/          # Game engine lifecycle, math, random generators, configs
│   ├── world/         # Tilemap, camera 2.5D, road navigation graphs, day/night cycles
│   ├── entities/      # Player, Pedestrians, Vehicles, Projectiles, Particles
│   ├── systems/       # Physics, Gang respect, Wanted law enforcement, Missions, Audio, Radio
│   ├── ui/            # HUD, Minimap radar, Menus, Floating retro text
│   └── audio/         # Procedural sound effect and music synth engine
├── src/               # Implementation files for all subsystems
└── tests/             # Automated test suite (Headless game loop, physics, weapons, gangs)
```

---

## 🚀 Building & Testing via GitHub Actions

This repository is configured to build and test **exclusively in the cloud via GitHub Actions CI**:

1. **Push to `main` or any pull request**: Automatically triggers `.github/workflows/ci.yml`.
2. **Build Matrix**: Tests against Linux (GCC/Clang), Windows (MSVC), and macOS.
3. **Automated Unit & Headless Tests**: All 7 test suites execute in CI.
4. **Artifacts**: Nightly builds and release binaries are uploaded to GitHub Actions artifacts and Releases.

To build manually on a machine with CMake 3.22+ and a C++20 compiler:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

---

## 📜 License
Released under the **MIT License**. Created with passion for classic open-world gaming.
