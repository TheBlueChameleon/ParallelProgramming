#include <ostream>
#include <string>
#include <vector>

#include "swarm.hpp"

class Trajectory
{
    public:
        using frame_t = std::vector<Swarm::coord_t>;

    private:
        std::vector<frame_t> frames;
        std::vector<double>  timestamps;
        std::vector<double>  energies;

    public:
        Trajectory() = default;

        const std::vector<double>&  getEnergies() const;
        const std::vector<frame_t>& getFrames() const;
        const frame_t&              getFrame(const int ID) const;
        void                        addFrame(const Swarm& frame);

        void writeCoordinates() const;
        void writeCoordinates(const std::string& filename) const;
        void writeCoordinates(std::ostream& stream) const;

        void writeEnergies() const;
        void writeEnergies(const std::string& filename) const;
        void writeEnergies(std::ostream& stream) const;
};
