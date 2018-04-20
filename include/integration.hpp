#pragma once
#include "stdafx.h"
#include "precision.hpp"
#include "templates/class_mesh.hpp"
#include "templates/class_particles.hpp"

class Cosmo_Param; //< params.hpp

void stream_step(const FTYPE_t da, std::vector<Particle_v<FTYPE_t>>& particles);
void stream_kick_stream(const FTYPE_t da, std::vector<Particle_v<FTYPE_t>>& particles, std::function<void()> kick_step, unsigned per);
void kick_step_no_momentum(const Cosmo_Param &cosmo, const FTYPE_t a, std::vector<Particle_v<FTYPE_t>>& particles, const std::vector< Mesh> &vel_field);
void kick_step_w_momentum(const Cosmo_Param &cosmo, const FTYPE_t a, const FTYPE_t da, std::vector<Particle_v<FTYPE_t>>& particles, const std::vector< Mesh> &force_field);