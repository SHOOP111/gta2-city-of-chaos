#include "systems/RadioSystem.hpp"

namespace gta2 {

RadioSystem::RadioSystem() {
    // 0. Off
    {
        RadioStationInfo s;
        s.station = RadioStation::Off;
        s.name = "Radio OFF";
        s.genre = "Silence";
        m_stations.push_back(s);
    }
    // 1. Head Radio
    {
        RadioStationInfo s;
        s.station = RadioStation::HeadRadio;
        s.name = "Head Radio 98.3";
        s.genre = "Classic Rock & Pop";
        s.djName = "Pharris McGuire";
        s.trackList = {"Taxi Drivers Must Die", "Drive-By Boogie", "Joyride '99", "Anywhere City Blues"};
        m_stations.push_back(s);
    }
    // 2. Futuro FM
    {
        RadioStationInfo s;
        s.station = RadioStation::FuturoFM;
        s.name = "Futuro FM";
        s.genre = "Synthwave / Cyberpunk";
        s.djName = "Dean Frantz";
        s.trackList = {"Neon Gridline", "Cybernetic Heart", "Retrograde Velocity", "Zaibatsu Overdrive"};
        m_stations.push_back(s);
    }
    // 3. KGBH 96
    {
        RadioStationInfo s;
        s.station = RadioStation::KGBH96;
        s.name = "KGBH 96.0";
        s.genre = "Underground Hip-Hop";
        s.djName = "DJ Bomba";
        s.trackList = {"Concrete Jungle", "Micro-Uzi Flow", "Street Respect", "Payphone Hustle"};
        m_stations.push_back(s);
    }
    // 4. Radio Zaibatsu
    {
        RadioStationInfo s;
        s.station = RadioStation::RadioZaibatsu;
        s.name = "Radio Zaibatsu";
        s.genre = "Corporate Electronic";
        s.djName = "Synthetic Voice Alpha";
        s.trackList = {"Corporate Mandate", "Executive Privileges", "Z-Type Pulse", "Efficiency Max"};
        m_stations.push_back(s);
    }
    // 5. Heavenly Voices
    {
        RadioStationInfo s;
        s.station = RadioStation::HeavenlyVoices;
        s.name = "Heavenly Voices";
        s.genre = "Orchestral / Choral";
        s.djName = "Brother Marcus";
        s.trackList = {"Peace in the Valley", "Chant of Serenity", "Dawn of Enlightenment"};
        m_stations.push_back(s);
    }
    // 6. Loonie Laughs
    {
        RadioStationInfo s;
        s.station = RadioStation::LoonieLaughs;
        s.name = "Loonie Laughs FM";
        s.genre = "Anarchy Punk / Talk Radio";
        s.djName = "Madman Sammy";
        s.trackList = {"Asylum Riot", "Chemical Joy", "Smiley Face Rampage", "Burn the City Down"};
        m_stations.push_back(s);
    }

    m_currentStationIndex = 1;
    m_currentStation = RadioStation::HeadRadio;
}

void RadioSystem::nextStation() {
    m_currentStationIndex = (m_currentStationIndex + 1) % m_stations.size();
    m_currentStation = m_stations[m_currentStationIndex].station;
    m_trackTimer = 0.0f;
    m_currentTrackIndex = 0;
}

void RadioSystem::previousStation() {
    m_currentStationIndex = (m_currentStationIndex + m_stations.size() - 1) % m_stations.size();
    m_currentStation = m_stations[m_currentStationIndex].station;
    m_trackTimer = 0.0f;
    m_currentTrackIndex = 0;
}

void RadioSystem::setStation(RadioStation station) {
    for (size_t i = 0; i < m_stations.size(); ++i) {
        if (m_stations[i].station == station) {
            m_currentStationIndex = i;
            m_currentStation = station;
            m_trackTimer = 0.0f;
            return;
        }
    }
}

void RadioSystem::turnOff() {
    setStation(RadioStation::Off);
}

const RadioStationInfo& RadioSystem::getCurrentStationInfo() const {
    return m_stations[m_currentStationIndex];
}

const std::string& RadioSystem::getCurrentTrackName() const {
    const auto& info = getCurrentStationInfo();
    if (info.trackList.empty()) {
        static std::string none = "No Broadcast";
        return none;
    }
    return info.trackList[m_currentTrackIndex % info.trackList.size()];
}

void RadioSystem::update(float dt) {
    if (m_currentStation == RadioStation::Off) return;

    m_trackTimer += dt;
    // Rotate song every 45 seconds
    if (m_trackTimer >= 45.0f) {
        m_trackTimer = 0.0f;
        const auto& info = getCurrentStationInfo();
        if (!info.trackList.empty()) {
            m_currentTrackIndex = (m_currentTrackIndex + 1) % info.trackList.size();
        }
    }
}

} // namespace gta2
