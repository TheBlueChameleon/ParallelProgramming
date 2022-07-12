#ifndef SWARM_HPP
#define SWARM_HPP

#include <numbers>
#include <vector>

class Swarm
{
    public:
        struct coord_t
        {
            double x, y;
        };

        static constexpr double cutoff = std::numbers::pi / 2.;
        static constexpr double xMax   = 10.;
        static constexpr double yMax   = 10.;
        static constexpr double dt     = .1;


    private:
        double timestamp = 0.;

        std::vector<coord_t> coordinates  ;
        std::vector<coord_t> velocities   ;
        std::vector<coord_t> accelerations;

        std::vector<coord_t> nextCoordinates  ;
        std::vector<coord_t> nextVelocities   ;
        std::vector<coord_t> nextAccelerations;

    public:
        Swarm() = default;
        Swarm(int N, double timestamp = 0.);

        double                      getTimestamp    () const;
        const std::vector<coord_t>& getCoordinates  () const;
        const std::vector<coord_t>& getVelocities   () const;
        const std::vector<coord_t>& getAccelerations() const;

        double  getEKin() const;
        double  getEPot() const;
        double  getETot() const;

        void    addParticle(const coord_t& coordinate, const coord_t& velocity);

        double  distanceBetween   (int i, int j) const;
        double  interactionBetween(int i, int j) const;
        coord_t forceBetween      (int i, int j) const;
        coord_t forceOn           (int i)        const;

        void    evolve();
};

#endif // SWARM_HPP
