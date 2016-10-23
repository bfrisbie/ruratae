#ifndef RURATAE_H__
#define RURATAE_H__

//=============================================================================
// public datatypes
//=============================================================================

typedef struct _ruratae_t ruratae_t;
typedef struct _ruratae_drawlist_t ruratae_drawlist_t;
typedef enum _ruratae_particle_status_t
{
  PARTICLE_NOFLAG = 0,
  PARTICLE_PAUSED = 1 << 0
}
ruratae_particle_status_t;
typedef enum _ruratae_spring_status_t
{
  SPRING_NOFLAG = 0,
  SPRING_PAUSED = 1 << 0
}
ruratae_spring_status_t;

//=============================================================================
// ruratae core api
//=============================================================================

/* generate a new instrument instance */
ruratae_t* ruratae_create(
  int max_particles, 
  int max_springs);

/* cleanup instrument instance */
void ruratae_destroy(
  ruratae_t* p);

/* integrate and process DSP for instrument
   returns a mono signal and a center of mass position vector */
void ruratae_process(
  ruratae_t* p,
  float*     out_buffer,
  float*     cx,
  float*     cy,
  float*     cz,
  int        num_samps);

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

/* get the engine's gravity setting */
void ruratae_get_gravity(
  ruratae_t* p,
  float*     x,
  float*     y,
  float*     z);

/* set the engine's gravity setting
   type: (0 = direction vector, 1 = position vector) */
void ruratae_set_gravity(
  ruratae_t* p,
  float      x,
  float      y,
  float      z);

//=============================================================================
// particles
//=============================================================================

/* create a new particle and returns its handle
   returns -1 if unable to create handle */
int ruratae_create_particle(
  ruratae_t* p,
  float      x, 
  float      y, 
  float      z, 
  float      vx, 
  float      vy, 
  float      vz, 
  float      recip_mass, 
  float      radius,
  float      restitution,
  int        status);

/* destroy particle
   will destroy any associated springs as well */
void ruratae_destroy_particle(
  ruratae_t* p, 
  int        particle_id);

/* get particle position */
void ruratae_get_particle_position(
  ruratae_t* p, 
  int        particle_id, 
  float*     x, 
  float*     y, 
  float*     z);

/* set particle position */
void ruratae_set_particle_position(
  ruratae_t* p, 
  int        particle_id, 
  float      x,
  float      y, 
  float      z);

/* get particle velocity */
void ruratae_get_particle_velocity(
  ruratae_t* p, 
  int        particle_id, 
  float*     vx, 
  float*     vy, 
  float*     vz);

/* set particle velocity */
void ruratae_set_particle_velocity(
  ruratae_t* p, 
  int        particle_id, 
  float      vx, 
  float      vy, 
  float      vz);

/* get particle reciprocal mass
   set to 0 for stationary particle */
float ruratae_get_particle_recip_mass(
  ruratae_t* p, 
  int        particle_id);

/* set particle reciprocal mass
   set to 0 for stationary particle */
void ruratae_set_particle_recip_mass(
  ruratae_t* p, 
  int        particle_id, 
  float      recip_mass);

/* get particle radius */
float ruratae_get_particle_radius(
  ruratae_t* p, 
  int        particle_id);

/* set particle radius */
void ruratae_set_particle_radius(
  ruratae_t* p, 
  int        particle_id, 
  float      radius);

/* get particle inellastic restitution */
float ruratae_get_particle_restitution(
  ruratae_t* p,
  int        particle_id);

/* set particle inellastic restitution */
void ruratae_set_particle_restitution(
  ruratae_t* p,
  int        particle_id,
  float      restitution);

/* get particle status flags */
int ruratae_get_particle_status(
  ruratae_t* p,
  int        particle_id);

/* set particle status flags */
void ruratae_set_particle_status(
  ruratae_t* p,
  int        particle_id,
  int        status);

//=============================================================================
// springs
//=============================================================================

/* create a new spring and returns its handle
   requires two handles for associated particles
   returns -1 if unable to create handle */
int ruratae_create_spring(
  ruratae_t* p, 
  int        particle_a_id, 
  int        particle_b_id,
  float      stiffness,
  float      damping,
  float      restlength,
  int        status);

/* destroy spring */
void ruratae_destroy_spring(
  ruratae_t* p, 
  int        spring_id);

/* get spring stiffness */
float ruratae_get_spring_stiffness(
  ruratae_t* p, 
  int        spring_id);

/* set spring stiffness */
void ruratae_set_spring_stiffness(
  ruratae_t* p, 
  int        spring_id,
  float      stiffness);

/* get spring damping */
float ruratae_get_spring_damping(
  ruratae_t* p, 
  int        spring_id);

/* set spring damping */
void ruratae_set_spring_damping(
  ruratae_t* p, 
  int        spring_id,
  float      damping);

/* get spring restlength */
float ruratae_get_spring_restlength(
  ruratae_t* p,
  int        spring_id);

/* set spring restlength */
void ruratae_set_spring_restlength(
  ruratae_t* p,
  int        spring_id,
  float      restlength);

/* get spring status flags */
int ruratae_get_spring_status(
  ruratae_t* p,
  int        spring_id);

/* set spring status flags */
void ruratae_set_spring_status(
  ruratae_t* p,
  int        spring_id,
  int        status);

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
