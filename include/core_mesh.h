/**
 * @file:	core_fce.h
 * @brief:	basic functions to work with mesh
 */

#pragma once

#include "stdafx.h"
#include "core.h"
#include <fftw3.h>

void get_k_vec(int N, int index, int* k_vec);
void get_k_vec(int N, int index, Vec_3D<int> &k_vec);
int get_k_sq(int N, int index);
void get_per(Vec_3D<double> &position, int per);
void get_per(Vec_3D<int> &position, int per);

void assign_to(Mesh* field, const Vec_3D<double> &position, const double value, int order);
void assign_from(const Mesh &field, const Vec_3D<double> &position, double* value, int order);
void assign_from(const std::vector< Mesh> &field, const Vec_3D<double> &position, Vec_3D<double>* value, int order);

void fftw_execute_dft_r2c(const fftw_plan &p_F, Mesh& rho);
void fftw_execute_dft_c2r(const fftw_plan &p_B, Mesh& rho);
void fftw_execute_dft_r2c_triple(const fftw_plan &p_F, std::vector<Mesh>& rho);
void fftw_execute_dft_c2r_triple(const fftw_plan &p_B, std::vector<Mesh>& rho);

double mean(double* p_data, int len);
double std_dev(double* p_data, int len, double t_mean);