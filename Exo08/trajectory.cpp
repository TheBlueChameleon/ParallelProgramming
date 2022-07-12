#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include "trajectory.hpp"
#include "swarm.hpp"

const std::vector<double>& Trajectory::getEnergies() const
{
    return energies;
}

const std::vector<Trajectory::frame_t>& Trajectory::getFrames() const
{
    return frames;
}

const Trajectory::frame_t& Trajectory::getFrame(const int ID) const
{
    return frames.at(ID);
}

void Trajectory::addFrame(const Swarm& frame)
{
    frames    .push_back(frame.getCoordinates());
    timestamps.push_back(frame.getTimestamp());
    energies  .push_back(frame.getETot());
}

void Trajectory::writeCoordinates() const
{
    writeCoordinates(std::cout);
}

void Trajectory::writeCoordinates(const std::string& filename) const
{
    std::ofstream hFile(filename);

    if (hFile.is_open())
    {
        writeCoordinates(hFile);
    }
    else
    {
        std::cerr << "Error while opening output file '" << filename << "' -- aborting write." << std::endl;
    }
}

void Trajectory::writeCoordinates(std::ostream& stream) const
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    stream << "# trajectory created at " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");

    for (const auto& frame : frames)
    {
        stream << "\n" << std::endl;
        for (const auto& point : frame)
        {
            stream << point.x << "\t" << point.y << "\n";
        }
    }
}

void Trajectory::writeEnergies() const
{
    writeEnergies(std::cout);
}

void Trajectory::writeEnergies(const std::string& filename) const
{
    std::ofstream hFile(filename);

    if (hFile.is_open())
    {
        writeEnergies(hFile);
    }
    else
    {
        std::cerr << "Error while opening output file '" << filename << "' -- aborting write." << std::endl;
    }
}

void Trajectory::writeEnergies(std::ostream& stream) const
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    stream << "# energy history created at " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << std::endl;
    stream << "# time\tenergy" << std::endl;

    for (int i = 0; i < frames.size(); ++i)
    {
        stream << timestamps[i] << "\t" << energies[i] << std::endl;
        ++i;
    }
}
