#pragma once

#include <string>
#include <vector>
#include "../core/Types.hpp"

namespace gta2 {

struct RadioStationInfo {
    RadioStation station = RadioStation::Off;
    std::string name;
    std::string genre;
    std::string djName;
    std::vector<std::string> trackList;
};

class RadioSystem {
public:
    RadioSystem();

    void nextStation();
    void previousStation();
    void setStation(RadioStation station);
    void turnOff();

    [[nodiscard]] RadioStation getCurrentStation() const noexcept { return m_currentStation; }
    [[nodiscard]] const RadioStationInfo& getCurrentStationInfo() const;
    [[nodiscard]] const std::string& getCurrentTrackName() const;

    void update(float dt);

private:
    RadioStation m_currentStation = RadioStation::HeadRadio;
    std::vector<RadioStationInfo> m_stations;
    size_t m_currentStationIndex = 1; // 0 = Off, 1 = HeadRadio
    float m_trackTimer = 0.0f;
    size_t m_currentTrackIndex = 0;
};

} // namespace gta2
