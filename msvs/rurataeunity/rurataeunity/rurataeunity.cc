#include "rurataeunity.h"

#include <memory>

// TODO(bitllama): This is a temporary workaround, |ruratae_t| struct must be
// visible through the Ruratae public header instead.
#include "..\src\ruratae.c"

namespace ruratae {

namespace {

// Unique instance of the Ruratae system.
static std::unique_ptr<ruratae_t> ruratae = nullptr;

}  // namespace

// TODO(anokta): Don't we also need at least the sample rate here?
void Initialize(int max_particles, int max_springs) {
  ruratae.reset(ruratae_create(max_particles, max_springs));
}

void Shutdown() {
  ruratae_destroy(ruratae.get());
  ruratae.release();
}

void Process(int length, float* output) {
  // TODO(anokta): Should this function return the center of mass?
  ruratae_process(ruratae.get(), output, nullptr, nullptr, nullptr, length);
}

void SetListenerPosition(vec3 position) {
  ruratae_set_listener_position(ruratae.get(), position.x, position.y,
                                position.z);
}

void SetGravity(vec3 gravity) {
  ruratae_set_gravity(ruratae.get(), gravity.x, gravity.y, gravity.z);
}

// TODO(anokta): It's probably better to minimize the params for create calls,
// and use the corresponding setter calls instead.
int CreateParticle(vec3 position, vec3 velocity, float recip_mass, float radius,
                   float restitution) {
  return ruratae_create_particle(ruratae.get(), position.x, position.y,
                                 position.z, velocity.x, velocity.y, velocity.z,
                                 recip_mass, radius, restitution);
}

void DestroyParticle(int particle_id) {
  ruratae_destroy_particle(ruratae.get(), particle_id);
}

// TODO(anokta): It's probably better to minimize the params for create calls,
// and use the corresponding setter calls instead.
int CreateSpring(int particle_a_id, int particle_b_id, float stiffness,
                 float damping, float restlength) {
  return ruratae_create_spring(ruratae.get(), particle_a_id, particle_b_id,
                               stiffness, damping, restlength);
}

void DestroySpring(int spring_id) {
  ruratae_destroy_spring(ruratae.get(), spring_id);
}

}  // namespace ruratae