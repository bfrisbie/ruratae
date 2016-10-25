#include "ruratae.hpp"

using namespace ruratae;

//=========================================================================
// private structures
//=========================================================================
struct instrument::set_msg
{
  enum _setmsgtype
  {
    INSTRUMENT_LISTENER,
    INSTRUMENT_GRAVITY,
    PARTICLE_CREATE,
    PARTICLE_DESTROY,
    PARTICLE_POSITION,
    PARTICLE_VELOCITY,
    PARTICLE_RECIPMASS,
    PARTICLE_RADIUS,
    PARTICLE_ELASTICITY,
    SPRING_CREATE,
    SPRING_DESTROY,
    SPRING_PARTICLEA,
    SPRING_PARTICLEB,
    SPRING_STIFFNESS,
    SPRING_DAMPING,
    SPRING_RESTLENGTH,
    INVALID_MSG
  };
  int   type;
  int   handle;
  union
  {
    int             val_i32;
    float           val_f32;
    vec3            val_vec3;
    particle_params val_pp;
    spring_params   val_sp;
  };
  set_msg(int _type = INVALID_MSG) : type(_type) {}
};

//=========================================================================
// core
//=========================================================================
instrument::instrument(int max_particles, int max_springs)
  : 
  m_particle_internal(max_particles),
  m_spring_internal(max_springs)
{
  // initialize token stacks
  for (int i = max_particles - 1; i >= 0; i--)
  {
    m_particle_stack.push(i);
  }
  for (int i = max_springs - 1; i >= 0; i--)
  {
    m_spring_stack.push(i);
  }
}

instrument::~instrument()
{
  //TODO: cleanup
}

vec3 instrument::process(float* out_buffer, int num_samps)
{
  // !!! NOTE: routine is HIGH-PRIORITY. use try_lock avoid thread blocks.
  instrument::set_msg v;
  int queue_i = 0;
  const int max_queue_items = 10; //TODO: user-configurable?
  while (!m_setter_queue.try_pop(v) && queue_i < max_queue_items)
  {
    mfn_process_queue_item(v);
    queue_i++;
  }

  //TODO: real DSP here...
  for (int i = 0; i < num_samps; i++)
  {
    *out_buffer++ = 0.0f;
  }
}

void instrument::drawlist()
{
  //TODO: do something render-y here
}

//=========================================================================
// global settings
//=========================================================================
vec3 instrument::get_listener() const
{
  return m_listener;
}

void instrument::set_listener(const vec3& v)
{
  set_msg msg(instrument::set_msg::INSTRUMENT_LISTENER);
  msg.val_vec3 = v;
  m_setter_queue.push(msg);
}

vec3 instrument::get_gravity() const
{
  return m_gravity;
}

void instrument::set_gravity(const vec3& v)
{
  set_msg msg(instrument::set_msg::INSTRUMENT_GRAVITY);
  msg.val_vec3 = v;
  m_setter_queue.push(msg);
}

//=========================================================================
// particles
//=========================================================================
int instrument::create_particle(const particle_params& p)
{
  int v;
  if (!m_particle_stack.pop(v))
    return -1;
  set_msg msg(instrument::set_msg::PARTICLE_CREATE);
  msg.handle = v;
  msg.val_pp = p;
  m_setter_queue.push(msg);
  return v;
}

void instrument::destroy_particle(int handle)
{
  //TODO: ensure handle is valid before processing...
  m_particle_stack.push(handle);
  set_msg msg(instrument::set_msg::PARTICLE_DESTROY);
  msg.handle = handle;
  m_setter_queue.push(msg);
}

vec3 instrument::get_particle_position(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_particle_position(int handle, const vec3& v)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::PARTICLE_POSITION);
  msg.handle = handle;
  msg.val_vec3 = v;
  m_setter_queue.push(msg);
}

vec3 instrument::get_particle_velocity(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_particle_velocity(int handle, const vec3& v)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::PARTICLE_VELOCITY);
  msg.handle = handle;
  msg.val_vec3 = v;
  m_setter_queue.push(msg);
}

float instrument::get_particle_reciprocal_mass(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_particle_reciprocal_mass(int handle, float v)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::PARTICLE_RECIPMASS);
  msg.handle = handle;
  msg.val_f32 = v;
  m_setter_queue.push(msg);
}

float instrument::get_particle_radius(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_particle_radius(int handle, float v)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::PARTICLE_RADIUS);
  msg.handle = handle;
  msg.val_f32 = v;
  m_setter_queue.push(msg);
}

float instrument::get_particle_elasticity(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_particle_elasticity(int handle, float v)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::PARTICLE_ELASTICITY);
  msg.handle = handle;
  msg.val_f32 = v;
  m_setter_queue.push(msg);
}

//=========================================================================
// springs
//=========================================================================
int instrument::create_spring(const spring_params& s)
{
  int v;
  if (!m_spring_stack.pop(v))
    return -1;
  set_msg msg(instrument::set_msg::SPRING_CREATE);
  msg.handle = v;
  msg.val_sp = s;
  m_setter_queue.push(msg);
  return v;
}

void instrument::destroy_spring(int handle)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::SPRING_DESTROY);
  msg.handle = handle;
  m_setter_queue.push(msg);
}

int instrument::get_spring_particle_a(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_spring_particle_a(int handle, int particle_handle)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::SPRING_PARTICLEA);
  msg.handle = handle;
  msg.val_i32 = particle_handle;
  m_setter_queue.push(msg);
}

int instrument::get_spring_particle_b(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_spring_particle_b(int handle, int particle_handle)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::SPRING_PARTICLEB);
  msg.handle = handle;
  msg.val_i32 = particle_handle;
  m_setter_queue.push(msg);
}

float instrument::get_spring_stiffness(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_spring_stiffness(int handle, float v)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::SPRING_STIFFNESS);
  msg.handle = handle;
  msg.val_f32 = v;
  m_setter_queue.push(msg);
}

float instrument::get_spring_damping(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_spring_damping(int handle, float v)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::SPRING_DAMPING);
  msg.handle = handle;
  msg.val_f32 = v;
  m_setter_queue.push(msg);
}

float instrument::get_spring_restlength(int handle)
{
  //TODO: ensure handle is valid before processing...
}

void instrument::set_spring_restlength(int handle, float v)
{
  //TODO: ensure handle is valid before processing...
  set_msg msg(instrument::set_msg::SPRING_RESTLENGTH);
  msg.handle = handle;
  msg.val_f32 = v;
  m_setter_queue.push(msg);
}

//=========================================================================
// private methods
//=========================================================================
void instrument::mfn_process_queue_item(const set_msg& msg)
{
  if (msg.type == instrument::set_msg::INSTRUMENT_LISTENER)
  {
    m_listener = msg.val_vec3;
  }
  else if (msg.type == instrument::set_msg::INSTRUMENT_GRAVITY)
  {
    m_gravity = msg.val_vec3;
  }
  else if (msg.type == instrument::set_msg::PARTICLE_CREATE)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::PARTICLE_DESTROY)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::PARTICLE_POSITION)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::PARTICLE_VELOCITY)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::PARTICLE_RECIPMASS)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::PARTICLE_RADIUS)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::PARTICLE_ELASTICITY)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::SPRING_CREATE)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::SPRING_DESTROY)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::SPRING_PARTICLEA)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::SPRING_PARTICLEB)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::SPRING_STIFFNESS)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::SPRING_DAMPING)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::SPRING_RESTLENGTH)
  {
    //TODO
  }
  else if (msg.type == instrument::set_msg::INVALID_MSG)
  {
    //TODO
  }
}
