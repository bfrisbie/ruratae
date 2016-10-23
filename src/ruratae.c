#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ruratae.h"

//=============================================================================
// c data structs
//=============================================================================

typedef struct _int_stack_t
{
  int  max;
  int  cur;
  int* arr;
}
int_stack_t;

void int_stack_destroy(int_stack_t* p);
int_stack_t* int_stack_create(int max)
{
  int_stack_t* p = (int_stack_t*)malloc(sizeof(int_stack_t));
  if (NULL == p)
    goto memory_fault;

  p->max = max;
  p->cur = -1;
  p->arr = NULL;

  p->arr = (int*)malloc(sizeof(int) * max);
  if (NULL == p->arr)
    goto memory_fault;

  return p;

memory_fault:
  int_stack_destroy(p);
  return NULL;
}

void int_stack_destroy(int_stack_t* p)
{
  free(p->arr);
  free(p);
}

int int_stack_pop(int_stack_t* p, int* val)
{
  if (p->cur >= 0)
    return (*val = p->arr[p->cur--]);
  return 0;
}

int int_stack_push(int_stack_t* p, int val)
{
  if (p->cur < p->max - 1)
    return (p->arr[++p->cur] = val);
  return 0;
}

//=============================================================================
// private datatypes
//=============================================================================

typedef struct _ruratae_particle_t ruratae_particle_t;
typedef struct _ruratae_spring_t ruratae_spring_t;
typedef struct _ruratae_t
{
  ruratae_particle_t* particles;
  int_stack_t*        particle_stack;
  ruratae_spring_t*   springs;
  int_stack_t*        spring_stack;
  float               listener[3];
  float               gravity[3];
}
ruratae_t;

typedef struct _ruratae_particle_t
{
  int   enabled;
  float position[3];
  float velocity[3];
  float acceleration[3];
  float recip_mass;
  float radius;
  float restitution;
  int   status;
}
ruratae_particle_t;

typedef struct _ruratae_spring_t
{
  int   enabled;
  int   particle_a;
  int   particle_b;
  float stiffness;
  float damping;
  float restlength;
  int   status;
}
ruratae_spring_t;

//=============================================================================
// ruratae core api
//=============================================================================

ruratae_t* ruratae_create(
  int max_particles, 
  int max_springs)
{
  int i = 0;
  ruratae_t* p = (ruratae_t*)malloc(sizeof(ruratae_t));
  if (NULL == p)
    goto memory_fault;

  p->particles = (ruratae_particle_t*)
    malloc(sizeof(ruratae_particle_t) * max_particles);
  if (NULL == p->particles)
    goto memory_fault;

  p->particle_stack = int_stack_create(max_particles);
  if (NULL == p->particle_stack)
    goto memory_fault;

  p->springs = (ruratae_spring_t*)
    malloc(sizeof(ruratae_spring_t) * max_springs);
  if (NULL == p->springs)
    goto memory_fault;

  p->spring_stack = int_stack_create(max_springs);
  if (NULL == p->spring_stack)
    goto memory_fault;

  memset(p->particles, 0, sizeof(ruratae_particle_t) * max_particles);
  memset(p->particles, 0, sizeof(ruratae_spring_t) * max_springs);

  for (i = max_particles - 1; i >= 0; i--)
  {
    if (!int_stack_push(p->particle_stack, i))
      goto memory_fault;
    p->particles[i].enabled = 0;
  }

  for (i = max_springs - 1; i >= 0; i--)
  {
    if (!int_stack_push(p->spring_stack, i))
      goto memory_fault;
    p->springs[i].enabled = 0;
  }

  p->listener[0] = 0;
  p->listener[1] = 0;
  p->listener[2] = 0;

  p->gravity[0] = 0;
  p->gravity[1] = 0;
  p->gravity[2] = 0;

  return p;

memory_fault:
  ruratae_destroy(p);
  return NULL;
}

//=============================================================================

void ruratae_destroy(
  ruratae_t* p)
{
  free(p->particles);
  int_stack_destroy(p->particle_stack);
  free(p->springs);
  int_stack_destroy(p->spring_stack);
  free(p);
}

//=============================================================================

void ruratae_process(
  ruratae_t* p,
  float*     out_buffer,
  float*     cx,
  float*     cy,
  float*     cz,
  int        num_samps)
{
  (ruratae_t*)p;
  (float*)out_buffer;
  (float*)cx;
  (float*)cy;
  (float*)cz;
  fprintf(stdout, 
    "You called the process function for %d samples.\n", num_samps);
}

//=============================================================================

const ruratae_drawlist_t* ruratae_get_drawlist(
  ruratae_t* p)
{
  (ruratae_t*)p;
  fprintf(stdout,
    "You called the drawlist function.\n");
  return NULL;
}

//=============================================================================

void ruratae_get_listener_position(
  ruratae_t* p,
  float*     x,
  float*     y,
  float*     z)
{
  *x = p->listener[0];
  *y = p->listener[1];
  *z = p->listener[2];
}

//=============================================================================

void ruratae_set_listener_position(
  ruratae_t* p,
  float      x,
  float      y,
  float      z)
{
  p->listener[0] = x;
  p->listener[1] = y;
  p->listener[2] = z;
}

//=============================================================================

void ruratae_get_gravity(
  ruratae_t* p,
  float*     x,
  float*     y,
  float*     z)
{
  *x = p->gravity[0];
  *y = p->gravity[1];
  *z = p->gravity[2];
}

//=============================================================================

void ruratae_set_gravity(
  ruratae_t* p,
  float      x,
  float      y,
  float      z)
{
  p->gravity[0] = x;
  p->gravity[1] = y;
  p->gravity[2] = z;
}

//=============================================================================
// particles
//=============================================================================

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
  int        status)
{
  int val;
  if (int_stack_pop(p->particle_stack, &val))
  {
    p->particles[val].enabled = 1;
    p->particles[val].position[0] = x;
    p->particles[val].position[1] = y;
    p->particles[val].position[2] = z;
    p->particles[val].velocity[0] = vx;
    p->particles[val].velocity[1] = vy;
    p->particles[val].velocity[2] = vz;
    p->particles[val].acceleration[0] = 0;
    p->particles[val].acceleration[1] = 0;
    p->particles[val].acceleration[2] = 0;
    p->particles[val].recip_mass = recip_mass;
    p->particles[val].radius = radius;
    p->particles[val].restitution = restitution;
    p->particles[val].status = status;
    return val;
  }
  return -1;
}

//=============================================================================

void ruratae_destroy_particle(
  ruratae_t* p,
  int        particle_id)
{
  int i = 0;
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled &&
      int_stack_push(p->particle_stack, particle_id))
  {
    //TODO: Replace with per-particle linked-list O(N) -> O(1)
    for (i = 0; i < p->spring_stack->max; i++)
    {
      if (p->springs[i].enabled &&
         (p->springs[i].particle_a == particle_id ||
          p->springs[i].particle_b == particle_id))      
        ruratae_destroy_spring(p, i);
    }
    p->particles[particle_id].enabled = 0;
  }
}

//=============================================================================

void ruratae_get_particle_position(
  ruratae_t* p,
  int        particle_id,
  float*     x,
  float*     y,
  float*     z)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
  {
    *x = p->particles[particle_id].position[0];
    *y = p->particles[particle_id].position[1];
    *z = p->particles[particle_id].position[2];
  }
}

//=============================================================================

void ruratae_set_particle_position(
  ruratae_t* p,
  int        particle_id,
  float      x,
  float      y,
  float      z)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
  {
    p->particles[particle_id].position[0] = x;
    p->particles[particle_id].position[1] = y;
    p->particles[particle_id].position[2] = z;
  }
}

//=============================================================================

void ruratae_get_particle_velocity(
  ruratae_t* p,
  int        particle_id,
  float*     vx,
  float*     vy,
  float*     vz)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
  {
    *vx = p->particles[particle_id].velocity[0];
    *vy = p->particles[particle_id].velocity[1];
    *vz = p->particles[particle_id].velocity[2];
  }
}

//=============================================================================

void ruratae_set_particle_velocity(
  ruratae_t* p,
  int        particle_id,
  float      vx,
  float      vy,
  float      vz)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
  {
    p->particles[particle_id].velocity[0] = vx;
    p->particles[particle_id].velocity[1] = vy;
    p->particles[particle_id].velocity[2] = vz;
  }
}

//=============================================================================

float ruratae_get_particle_recip_mass(
  ruratae_t* p,
  int        particle_id)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
    return p->particles[particle_id].recip_mass;
  return 0;
}

//=============================================================================

void ruratae_set_particle_recip_mass(
  ruratae_t* p,
  int        particle_id,
  float      recip_mass)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
    p->particles[particle_id].recip_mass = recip_mass;
}

//=============================================================================

float ruratae_get_particle_radius(
  ruratae_t* p,
  int        particle_id)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
    return p->particles[particle_id].radius;
  return 0;
}

//=============================================================================

void ruratae_set_particle_radius(
  ruratae_t* p,
  int        particle_id,
  float      radius)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
    p->particles[particle_id].radius = radius;
}

//=============================================================================

float ruratae_get_particle_restitution(
  ruratae_t* p,
  int        particle_id)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
    return p->particles[particle_id].restitution;
  return 0;
}

//=============================================================================

void ruratae_set_particle_restitution(
  ruratae_t* p,
  int        particle_id,
  float      restitution)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
    p->particles[particle_id].restitution = restitution;
}

//=============================================================================

int ruratae_get_particle_status(
  ruratae_t* p,
  int        particle_id)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
    return p->particles[particle_id].status;
  return 0;
}

//=============================================================================

void ruratae_set_particle_status(
  ruratae_t* p,
  int        particle_id,
  int        status)
{
  if (particle_id >= 0 &&
      particle_id < p->particle_stack->max &&
      p->particles[particle_id].enabled)
    p->particles[particle_id].status = status;
}

//=============================================================================
// springs
//=============================================================================

int ruratae_create_spring(
  ruratae_t* p,
  int        particle_a_id,
  int        particle_b_id,
  float      stiffness,
  float      damping,
  float      restlength,
  int        status)
{
  int val;
  if (particle_a_id >= 0 &&
      particle_b_id >= 0 &&
      particle_a_id < p->particle_stack->max &&
      particle_b_id < p->particle_stack->max &&
      p->particles[particle_a_id].enabled &&
      p->particles[particle_b_id].enabled &&
      int_stack_pop(p->spring_stack, &val))
  {
    p->springs[val].enabled = 1;
    p->springs[val].particle_a = particle_a_id;
    p->springs[val].particle_b = particle_b_id;
    p->springs[val].stiffness = stiffness;
    p->springs[val].damping = damping;
    p->springs[val].restlength = restlength;
    p->springs[val].status = status;
    return val;
  }
  return -1;
}

//=============================================================================

void ruratae_destroy_spring(
  ruratae_t* p,
  int        spring_id)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled &&
      int_stack_push(p->spring_stack, spring_id))
    p->springs[spring_id].enabled = 0;
}

//=============================================================================

float ruratae_get_spring_stiffness(
  ruratae_t* p,
  int        spring_id)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled)
    return p->springs[spring_id].stiffness;
  return 0;
}

//=============================================================================

void ruratae_set_spring_stiffness(
  ruratae_t* p,
  int        spring_id,
  float      stiffness)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled)
    p->springs[spring_id].stiffness = stiffness;
}

//=============================================================================

float ruratae_get_spring_damping(
  ruratae_t* p,
  int        spring_id)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled)
    return p->springs[spring_id].damping;
  return 0;
}

//=============================================================================

void ruratae_set_spring_damping(
  ruratae_t* p,
  int        spring_id,
  float      damping)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled)
    p->springs[spring_id].damping = damping;
}

//=============================================================================

float ruratae_get_spring_restlength(
  ruratae_t* p,
  int        spring_id)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled)
    return p->springs[spring_id].restlength;
  return 0;
}

//=============================================================================

void ruratae_set_spring_restlength(
  ruratae_t* p,
  int        spring_id,
  float      restlength)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled)
    p->springs[spring_id].restlength = restlength;
}

//=============================================================================

int ruratae_get_spring_status(
  ruratae_t* p,
  int        spring_id)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled)
    return p->springs[spring_id].status;
  return 0;
}

//=============================================================================

void ruratae_set_spring_status(
  ruratae_t* p,
  int        spring_id,
  int        status)
{
  if (spring_id >= 0 &&
      spring_id < p->spring_stack->max &&
      p->springs[spring_id].enabled)
    p->springs[spring_id].status = status;
}

