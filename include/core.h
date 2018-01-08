/**
 * @file:	core.h
 * @brief:	class definitions
 */

#pragma once
#include "stdafx.h"
 
template <typename T> int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

#if PRECISION == 1
typedef float FTYPE;
#elif PRECISION == 2
typedef double FTYPE;
#elif PRECISION == 3
typedef long double FTYPE;
#else
typedef double FTYPE;
#endif

constexpr FTYPE PI = M_PI;
/**
 * @class:	Vec_3D<T>
 * @brief:	class handling basic 3D-vector functions
 */

template <typename T>
class Vec_3D : public std::array<T, 3>
{
public:
	// CONSTRUCTORS
	Vec_3D(){};
	Vec_3D(T x, T y, T z):
	std::array<T, 3>({x, y, z}) {};
    
    // METHODS
    T norm2() const;
	FTYPE norm() const;
		
	// OPERATORS	
	Vec_3D<T>& operator+=(const Vec_3D<T>& rhs);
    Vec_3D<T>& operator-=(const Vec_3D<T>& rhs);
    Vec_3D<T>& operator+=(T rhs);
	Vec_3D<T>& operator-=(T rhs);
	Vec_3D<T>& operator*=(T rhs);
	Vec_3D<T>& operator/=(T rhs);
	template<class U>
    explicit operator Vec_3D<U>() const;
};

// NON-MEMBER FUNCTIONS
template <typename T> Vec_3D<T> operator+(Vec_3D<T> lhs, const Vec_3D<T>& rhs);
template <typename T> Vec_3D<T> operator-(Vec_3D<T> lhs, const Vec_3D<T>& rhs);
template <typename T> Vec_3D<T> operator*(Vec_3D<T> lhs, T rhs);
template <typename T> Vec_3D<T> operator*(T lhs, Vec_3D<T> rhs);
template <typename T> Vec_3D<T> operator+(Vec_3D<T> lhs, T rhs);
template <typename T> Vec_3D<T> operator+(T lhs, Vec_3D<T> rhs);
template <typename T> Vec_3D<T> operator-(Vec_3D<T> lhs, T rhs);
template <typename T> Vec_3D<T> operator-(T lhs, Vec_3D<T> rhs);
template <typename T> Vec_3D<T> operator/(Vec_3D<T> lhs, T rhs);

/**
 * @class:	Mesh_base<T>
 * @brief:	class handling basic mesh functions, the most important are creating and destroing the underlying data structure
 *			creates a mesh of N1*N2*N3 cells
 */

// ignore following in SWIG wrapper
#ifndef SWIG
template <typename T>
class Mesh_base
{
public:
	// CONSTRUCTOR
	Mesh_base(unsigned n1, unsigned n2, unsigned n3);
	
	// VARIABLES
	unsigned N1, N2, N3, length; // acces dimensions and length of mesh
    std::vector<T> data; // data stored on the mesh
	
	// METHODS
    T* real() { return data.data();} // acces data through pointer
    const T* real() const { return data.data();} // acces data through const pointer
	void assign(T val);
	
	// OPERATORS
	T &operator[](int i){ return data[i]; }
	const T &operator[](int i) const{ return data[i]; }
	
	T& operator()(int i, int j, int k){ return data[i*N2*N3+j*N3+k]; }
	const T& operator()(int i, int j, int k) const{ return data[i*N2*N3+j*N3+k]; }
	
	T& operator()(int i, int j){ return data[i*N3+j]; }
	const T& operator()(int i, int j) const{ return data[i*N3+j]; }
	
	T& operator()(Vec_3D<int> pos);
	const T& operator()(Vec_3D<int> pos) const;
	
	Mesh_base& operator+=(const T& rhs);
	Mesh_base& operator-=(const T& rhs){ return *this+=-rhs; }
	Mesh_base& operator*=(const T& rhs);
	Mesh_base& operator/=(const T& rhs);
};

// template <typename T> void swap(Mesh_base<T>& first, Mesh_base<T>& second);

/**
 * @class:	Mesh
 * @brief:	creates a mesh of N*N*(N+2) cells
 */

class Mesh : public Mesh_base<FTYPE>
{
public:
	// CONSTRUCTORS & DESTRUCTOR
    Mesh(unsigned n);
    // default constructors / assignemnts belowe
    // Mesh(const Mesh& that);
    // Mesh(Mesh&& that) noexcept;
    // Mesh& operator=(Mesh that);
    // friend void swap(Mesh& first, Mesh& second);
	
	// VARIABLES
	unsigned N; // acces dimension of mesh
	
	// METHODS
    fftw_complex* complex() { return reinterpret_cast<fftw_complex*>(data.data());}
    const fftw_complex* complex() const { return reinterpret_cast<const fftw_complex*>(data.data());}

    void reset_part(bool part);
    void reset_re() { reset_part(0); }
    void reset_im() { reset_part(1); }
    
	// OPERATORS
	using Mesh_base<FTYPE>::operator ();
	FTYPE& operator()(Vec_3D<int> pos);
	const FTYPE& operator()(Vec_3D<int> pos) const;
	
	Mesh& operator+=(const FTYPE& rhs);
	Mesh& operator-=(const FTYPE& rhs){ return *this+=-rhs; }
	Mesh& operator*=(const FTYPE& rhs);
	Mesh& operator/=(const FTYPE& rhs);
};

/**
 * @class:	Particle_x
 * @brief:	class handling particles (position only)
 * @acces:	operator [] to get position coordinates
 */

class Particle_x
{
public:
	// CONSTRUCTORS
	Particle_x(){};
	Particle_x(FTYPE x, FTYPE y, FTYPE z):
	position(x,y,z) {};
	Particle_x(Vec_3D<FTYPE> position):
	position(position) {};
	
	// VARIABLES
	Vec_3D<FTYPE> position;
	
	// OPERATORS
	FTYPE &operator[](int i){ return position[i]; }
	const FTYPE& operator[](int i) const{ return position[i]; }
};

/**
 * @class:	Particle_v
 * @brief:	class handling particles (with velocitites)
 * @acces:	operator [] to get position coordinates
 * 			operator () to get velocity coordinates
 */

class Particle_v : public Particle_x
{
public:
	// CONSTRUCTORS
	Particle_v(){};
	Particle_v(FTYPE x, FTYPE y, FTYPE z, FTYPE vx, FTYPE vy, FTYPE vz):
		Particle_x(x,y,z), velocity(vx,vy,vz) {};
	Particle_v(Vec_3D<FTYPE> position, Vec_3D<FTYPE> velocity):
		Particle_x(position), velocity(velocity) {};
	
	// VARIABLES
	Vec_3D<FTYPE> velocity;

	// OPERATORS
	FTYPE &operator()(int i){ return velocity[i]; }
	const FTYPE& operator()(int i) const{ return velocity[i]; }
};
#endif

class Cosmo_Param
{
public:
    // CONSTRUCTORS, DESTRUCTOR
    void init(); //< lazy constructor
    Cosmo_Param();
    ~Cosmo_Param();

    // CCL VARIABLES
    ccl_configuration config;
    ccl_cosmology* cosmo;

    // COSMOLOGY (flat LCDM)
    FTYPE A = 1, ns, k2_G, sigma8;
    FTYPE Omega_m, Omega_b, H0, h;
    FTYPE Omega_c() const { return Omega_m - Omega_b; }
    FTYPE Omega_L() const { return 1 - Omega_m; }

    // PRECOMPUTED VALUES
    FTYPE D_norm;

    // DEALING WITH GSL 'void* param'
    explicit operator void*() const;
};
void to_json(nlohmann::json&, const Cosmo_Param&);
void from_json(const nlohmann::json&, Cosmo_Param&);

/**
 * @class:	Tracking
 * @brief:	class storing info about tracked particles
 */

class Tracking
{
public:
	// CONSTRUCTOR
	Tracking(int sqr_num_track_par, int par_num_per_dim);
	
	// VARIABLES
	int sqr_num_track_par, num_track_par; // square root of number of tracking particles
	std::vector<int> par_ids;
	std::vector<std::vector<Particle_x>> par_pos;
	
	// METHODS
	const unsigned num_step() const{return par_pos.size();};
	template <class T>  void update_track_par(T* particles);
};

/**
 * @brief An enum class that defines which integration scheme use in computation of correlation function.

Q - quadrature routine

N - non-adaptive integrator
A - adaptive integrator

G - general integrand (user-defined)
W - weight function with integrand

S - singularities can be more readily integrated
P - points of special difficulty can be supplied
I - infinite range of integration
O - oscillatory weight function, cos or sin
F - Fourier integral
C - Cauchy principal value
*/
enum class corr_int_type { QAGI, QAWO, QAWF, FFT, FFTLOG, PP };

/* SIMULATION BOX*/
struct Box_Opt {
    void init();
    /* cmd args */
    unsigned par_num_1d, mesh_num, mesh_num_pwr;
    FTYPE box_size;
    /* derived param*/
    unsigned par_num, Ng, Ng_pwr;
};


/* INTEGRATION */
struct Integ_Opt {
    void init();
    /* cmd args */
    FTYPE z_in, z_out, db;
    /* derived param*/
    FTYPE b_in, b_out;
};


/* OUTPUT */
struct Out_Opt {
    void init();
    /* cmd args */
    unsigned print_every, bins_per_decade, points_per_10_Mpc;
    std::vector<FTYPE> print_z; //< for which redshifts print output on top of print_every (optional)
    std::string out_dir; //< where to save output of the simulation
    bool print_par_pos, print_dens, print_pwr, print_extrap_pwr, print_corr, print_vel_pwr;
    /* derived param*/
    bool get_rho, get_pwr, get_pk_extrap;
};


/* APPROXIMATIONS */
struct Comp_App {
    /* cmd args */
    bool ZA, FF, FP, AA, FP_pp;
};


/* APPROXIMATIONS */
struct App_Opt {
    void init(const Box_Opt&);
    /* cmd args */
    FTYPE nu, rs;
    /* derived param*/
    FTYPE Hc, a, nu_dim;
    unsigned M;
};


/* RUN */
struct Run_Opt {
    void init();
    bool simulate();
    /* cmd args */
    unsigned nt, mlt_runs;
    unsigned long seed;
    bool pair;        
    /* other*/
    bool phase;
};

// define Range outside because of SWIG
struct Range { FTYPE lower, upper; };

/* OTHER PARAMETERS */
struct Other_par {
    void init(const Box_Opt&);
    // k-range where to use (linear) interpolation and k-range in which print 'pwr_spec_extrap_*'
    ///range in which compute the correlation function 
    Range k_print, x_corr;
    std::map<std::string,FTYPE> nyquist; //< Nyquist frequencies of potential mesh, analyses mesh and particle separation
};

/**
 * @class:	Sim_Param
 * @brief:	class storing simulation parameters
 */

class Sim_Param
{
public:
    // CONSTRUCTOR
    Sim_Param(int ac, char* av[]); //< load from command line arguments
    Sim_Param(std::string file_name); //< load from sim_param.json file

    // VARIABLES
    Box_Opt box_opt;
    Integ_Opt integ_opt;
    Out_Opt out_opt;
    Comp_App comp_app;
    Cosmo_Param cosmo;
    App_Opt app_opt;
    Run_Opt run_opt;
    Other_par other_par;

	// METHODS
    void print_info(std::string out, std::string app) const;
	void print_info() const;
	const FTYPE x_0() const{return box_opt.box_size/box_opt.mesh_num;}
    const FTYPE x_0_pwr() const{return box_opt.box_size/box_opt.mesh_num_pwr;}
    bool simulate() { return run_opt.simulate(); }
};

void to_json(nlohmann::json&, const Box_Opt&);
void to_json(nlohmann::json&, const Integ_Opt&);
void to_json(nlohmann::json&, const App_Opt&);
void to_json(nlohmann::json&, const Run_Opt&);
void to_json(nlohmann::json&, const Out_Opt&);

void from_json(const nlohmann::json&, Box_Opt&);
void from_json(const nlohmann::json&, Integ_Opt&);
void from_json(const nlohmann::json&, App_Opt&);
void from_json(const nlohmann::json&, Run_Opt&);
void from_json(const nlohmann::json&, Out_Opt&);

/**
 * @class:	Data_Vec
 * @brief:	class containing data [x, y,...]
 */

template <typename T, unsigned N>
class Data_Vec
{
public:
    // CONSTRUCTORS
    Data_Vec() = default;
    Data_Vec(size_t size) { data.fill(std::vector<T>(size)); }

    // VARIABLES
    std::array<std::vector<T>, N> data;

    // ELEMENT ACCESS
    std::vector<T>& operator[](int i){ return data[i]; }
    const std::vector<T>& operator[](int i) const { return data[i]; }

    // CAPACITY
    size_t dim() const noexcept{ return data.size(); }
    size_t size() const noexcept{ return data[0].size(); }
    void resize (size_t n){
        for (auto &vec : data) vec.resize(n);
    }
    void resize (size_t n, FTYPE val){
        for (auto &vec : data) vec.resize(n, val);
    }
    void reserve(size_t n){
        for (auto &vec : data) vec.reserve(n);
    }
    void erase(unsigned index){
        for (auto &vec : data) vec.erase(vec.begin() + index);
    }
    // MODIFIERS
    void fill(const T& val){
        for (auto &vec : data) std::fill(vec.begin(), vec.end(), val);
    }
};

// ignore following in SWIG wrapper
#ifndef SWIG

#include "core_power.h"

/**
 * @class:	App_Var
 * @brief:	class containing core variables for approximations
 */

template <class T> 
class App_Var
{
public:
	// CONSTRUCTORS
    App_Var(const Sim_Param &sim, std::string app_str);

	// DESTRUCTOR
	~App_Var();
	
    // VARIABLES
    const Sim_Param &sim;

	int step, print_every;
    FTYPE b, b_out, db;
    FTYPE D_init, dDda_init;
    const std::string app_str, z_suffix_const, out_dir_app;
    
    // LARGE FIELDS
	std::vector<Mesh> app_field;
    std::vector<Mesh> power_aux;
    T* particles;

    // OTHER VARIABLES
    Data_Vec<FTYPE, 2> corr_func_binned, pwr_spec_binned, pwr_spec_binned_0, vel_pwr_spec_binned_0;
    Interp_obj pwr_spec_input;
	fftw_plan p_F, p_B, p_F_pwr, p_B_pwr;
	Tracking track;
	std::vector<int> dens_binned;
	
	// METHODS
	FTYPE z() const{ return 1./b - 1.;}
	FTYPE b_half() const { return b - db/2.; }
	bool integrate() const { return (b <= b_out) && (db > 0);}
	bool printing() const { return print_every ? ((step % print_every) == 0) or (b == b_out) : print_every ; }
    void print_output();
    void upd_time();
    void print_mem() const;
    void print_info() const;	
	std::string z_suffix();
	bool is_init_pwr_spec_0, is_init_vel_pwr_spec_0; //< be careful about setting these to true
protected:	
    std::stringstream z_suffix_num;
    uint64_t memory_alloc; // only the largest chunks
};

/**
 * @class:	App_Var_AA
 * @brief:	class containing variables for adhesion approximation
 */
 
 class App_Var_AA: public App_Var<Particle_v>
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	App_Var_AA(const Sim_Param &sim, std::string app_str);

	// VARIABLES
	Mesh expotential;
};

/**
 * @class LinkedList
 * @brief class handling linked lists
 */

class LinkedList
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	LinkedList(unsigned par_num, int m, FTYPE hc);
	
	// VARIABLES
	unsigned par_num;
	FTYPE Hc;
	std::vector<int> LL;
	Mesh_base<int> HOC;
	
	// METHODS
	void get_linked_list(Particle_v* particles);
};

/**
 * @class:	App_Var_FP_mod
 * @brief:	class containing variables for modified Frozen-potential approximation
 */

class App_Var_FP_mod: public App_Var<Particle_v>
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	App_Var_FP_mod(const Sim_Param &sim, std::string app_str);
	
	// VARIABLES
    LinkedList linked_list;
    Interp_obj fs_interp;
};
#endif
