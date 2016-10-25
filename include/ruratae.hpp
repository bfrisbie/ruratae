#ifndef RURATAE_H__
#define RURATAE_H__

#include "util.hpp"

namespace ruratae
{
struct vec3
{
  float x, y, z;
  vec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
    : x(_x), y(_y), z(_z) {}
};

struct particle_params
{
  vec3 position;
  vec3 velocity;
  float reciprocal_mass;
  float radius;
  float elasticity;
};

struct spring_params
{
  int particle_a;
  int particle_b;
  float stiffness;
  float damping;
  float restlength;
};

struct particle;
struct spring;
class instrument
{
public:
  instrument(int max_particles, int max_springs);
  ~instrument();

  vec3 process(float* out_buffer, int num_samps);
  void drawlist();

  //=========================================================================
  // global settings
  //=========================================================================
  vec3 get_listener() const;
  void set_listener(const vec3& v);

  vec3 get_gravity() const;
  void set_gravity(const vec3& v);

  //=========================================================================
  // particles
  //=========================================================================
  int create_particle(const particle_params& p);
  void destroy_particle(int handle);

  vec3 get_particle_position(int handle);
  void set_particle_position(int handle, const vec3& v);

  vec3 get_particle_velocity(int handle);
  void set_particle_velocity(int handle, const vec3& v);

  float get_particle_reciprocal_mass(int handle);
  void set_particle_reciprocal_mass(int handle, float v);

  float get_particle_radius(int handle);
  void set_particle_radius(int handle, float v);

  float get_particle_elasticity(int handle);
  void set_particle_elasticity(int handle, float v);

  //=========================================================================
  // springs
  //=========================================================================
  int create_spring(const spring_params& s);
  void destroy_spring(int handle);

  int get_spring_particle_a(int handle);
  void set_spring_particle_a(int handle, int particle_handle);

  int get_spring_particle_b(int handle);
  void set_spring_particle_b(int handle, int particle_handle);

  float get_spring_stiffness(int handle);
  void set_spring_stiffness(int handle, float v);

  float get_spring_damping(int handle);
  void set_spring_damping(int handle, float v);

  float get_spring_restlength(int handle);
  void set_spring_restlength(int handle, float v);

private:
  struct set_msg;
  void mfn_process_queue_item(const set_msg& msg);
  threadsafe_queue<set_msg> m_setter_queue;
  std::vector<particle> m_particle_internal;
  threadsafe_stack<int> m_particle_stack;
  std::vector<spring>   m_spring_internal;
  threadsafe_stack<int> m_spring_stack;
  vec3 m_listener;
  vec3 m_gravity;
};
};

#endif /* RURATAE_H__ */