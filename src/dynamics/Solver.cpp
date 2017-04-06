# include "../../include/dynamics/Solver.hpp"

Solver::Solver()
{}

Solver::~Solver()
{}

void Solver::solve(const float& dt, std::vector<ParticlePtr>& particles)
{
    do_solve(dt, particles);
}

void Solver::solve(const float& dt, std::vector<ParticleSkieurPtr>& particles)
{
    do_solve(dt, particles);
}
