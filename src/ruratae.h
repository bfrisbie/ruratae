#ifndef RURATAE_H__
#define RURATAE_H__

//=============================================================================
// public datatypes
//=============================================================================

typedef unsigned long rhandle_t;
typedef struct _ruratae_t ruratae_t;
typedef struct _ruratae_drawlist_t 
{
  rhandle_t* particle_id;
  float* particle_pos[3];
  float* particle_vel[3];
  rhandle_t* springs;
  rhandle_t* spring_particles[2];
}
ruratae_drawlist_t;

//=============================================================================
// instrument instance
//=============================================================================

/* generate a new instrument instance */
ruratae_t* ruratae_create(
  unsigned long max_particles, 
  unsigned long max_springs);

/* cleanup instrument instance */
void ruratae_destroy(
  ruratae_t* p);

/* integrate and process DSP for instrument
   returns a mono signal and a center of mass position vector */
void ruratae_process(
  ruratae_t*    p, 
  float*        out_buffer,
  float*        cx,
  float*        cy,
  float*        cz,
  unsigned long num_samps);

/* return pointer to drawlist structure */
const ruratae_drawlist_t* ruratae_get_drawlist(
  ruratae_t* p);

/* get the engine's listener position */
void ruratae_get_listener_position(
  ruratae_t* p,
  float*     x, 
  float*     y, 
  float*     z);

/* set the engine's listener position */
void ruratae_set_listener_position(
  ruratae_t* p, 
  float      x, 
  float      y, 
  float      z);

/* get the engine's gravity setting
   type: (0 = direction vector, 1 = position vector) */
void ruratae_get_gravity(
  ruratae_t* p,
  int*       type,
  float*     force,
  float*     x,
  float*     y,
  float*     z);

/* set the engine's gravity setting
   type: (0 = direction vector, 1 = position vector) */
void ruratae_set_gravity(
  ruratae_t* p,
  int        type,
  float      force,
  float      x,
  float      y,
  float      z);

//=============================================================================
// particles
//=============================================================================

/* create a new particle and returns its handle
   returns -1 if unable to create handle */
rhandle_t ruratae_create_particle(
  ruratae_t* p,
  float      x, 
  float      y, 
  float      z, 
  float      vx, 
  float      vy, 
  float      vz, 
  float      recip_mass, 
  float      radius);

/* destroy particle
   will destroy any associated springs as well */
void ruratae_destroy_particle(
  ruratae_t* p, 
  rhandle_t  particle_id);

/* get particle status (1 = enabled / 0 = disabled) */
int ruratae_get_particle_status(
  ruratae_t* p, 
  rhandle_t  particle_id);

/* set particle status (1 = enabled / 0 = disabled) */
void ruratae_get_particle_status(
  ruratae_t* p, 
  rhandle_t  particle_id, 
  int        status);

/* get particle position */
void ruratae_get_particle_position(
  ruratae_t* p, 
  rhandle_t  particle_id, 
  float*     x, 
  float*     y, 
  float*     z);

/* set particle position */
void ruratae_set_particle_position(
  ruratae_t* p, 
  rhandle_t  particle_id, 
  float      x,
  float      y, 
  float      z);

/* get particle velocity */
void ruratae_get_particle_velocity(
  ruratae_t* p, 
  rhandle_t  particle_id, 
  float*     vx, 
  float*     vy, 
  float*     vz);

/* set particle velocity */
void ruratae_set_particle_velocity(
  ruratae_t* p, 
  rhandle_t  particle_id, 
  float      vx, 
  float      vy, 
  float      vz);

/* get particle reciprocal mass
   set to 0 for stationary particle */
float ruratae_get_particle_reciprocal_mass(
  ruratae_t* p, 
  rhandle_t  particle_id);

/* set particle reciprocal mass
   set to 0 for stationary particle */
void ruratae_set_particle_reciprocal_mass(
  ruratae_t* p, 
  rhandle_t  particle_id, 
  float      recip_mass);

/* get particle radius */
float ruratae_get_particle_radius(
  ruratae_t* p, 
  rhandle_t  particle_id);

/* set particle radius */
void ruratae_get_particle_radius(
  ruratae_t* p, 
  rhandle_t  particle_id, 
  float      radius);

//=============================================================================
// springs
//=============================================================================

/* create a new spring and returns its handle
   requires two handles for associated particles
   returns -1 if unable to create handle */
rhandle_t ruratae_create_spring(
  ruratae_t* p, 
  rhandle_t  particle_A_id, 
  rhandle_t  particle_B_id);

/* destroy spring */
void ruratae_destroy_spring(
  ruratae_t* p, 
  rhandle_t  spring_id);

/* get spring status (1 = enabled / 0 = disabled) */
int ruratae_get_spring_status(
  ruratae_t* p, 
  rhandle_t  spring_id);

/* set spring status (1 = enabled / 0 = disabled) */
void ruratae_get_spring_status(
  ruratae_t* p, 
  rhandle_t  spring_id, 
  int        status);

/* get spring stiffness */
float ruratae_get_spring_stiffness(
  ruratae_t* p, 
  rhandle_t  spring_id);

/* set spring stiffness */
void ruratae_set_spring_stiffness(
  ruratae_t* p, 
  rhandle_t  spring_id, 
  float      stiffness);

/* get spring damping */
float ruratae_get_spring_damping(
  ruratae_t* p, 
  rhandle_t  spring_id);

/* set spring damping */
void ruratae_set_spring_damping(
  ruratae_t* p, 
  rhandle_t  spring_id, 
  float      damping);

/* get spring restlength */
float ruratae_get_spring_restlength(
  ruratae_t* p,
  rhandle_t  spring_id);

/* set spring restlength */
void ruratae_set_spring_restlength(
  ruratae_t* p,
  rhandle_t  spring_id,
  float      restlength);

//=============================================================================
// utility functions
//=============================================================================

// string (1d-line, wire)
// box2d (2d-box, wire/shell)
// box3d (3d-box, wire/shell/solid)
// connect with string
// find_nearest_particle/spring within range
// create impulse with rolloff

#endif /* RURATAE_H__ */
