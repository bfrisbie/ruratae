#ifndef _RURATAEUNITY_H_
#define _RURATAEUNITY_H_

#ifdef _WINDLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API
#endif

namespace ruratae {

// Simple three-dimensional vector.
struct vec3 {
  float x;
  float y;
  float z;
};

extern "C" {

// Initializes the Ruratae system.
EXPORT_API bool Initialize(int max_particles, int max_springs);

// Shuts down the Ruratae system.
EXPORT_API void Shutdown();

// Processes the next buffer.
EXPORT_API void Process(int length, float* output);

// Sets the listener position.
EXPORT_API void SetListenerPosition(vec3 position);

// Sets the gravity.
EXPORT_API void SetGravity(vec3 gravity);

// Creates a new particle and returns its id.
EXPORT_API int CreateParticle(vec3 position, vec3 velocity, float recip_mass,
                              float radius, float restitution);

// Destroys the particle with given |particle_id|.
EXPORT_API void DestroyParticle(int particle_id);

// Creates a new spring and returns its id.
EXPORT_API int CreateSpring(int particle_a_id, int particle_b_id,
                            float stiffness, float damping, float restlength);

// Destroys the spring with given |spring_id|.
EXPORT_API void DestroySpring(int spring_id);

}  // extern "C"

}  // namespace ruratae

#endif