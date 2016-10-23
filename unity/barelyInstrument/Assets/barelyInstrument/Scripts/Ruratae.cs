using UnityEngine;
using System;
using System.Collections;
using System.Runtime.InteropServices;

// Main Ruratae class that communicates with the native implementation.
public static class Ruratae {
  // Initializes the system.
  public static void InitializeSystem (int maxParticles, int maxSprings) {
    if (!initialized) {
      initialized = Initialize(maxParticles, maxSprings);
    }
  }

  // Shuts down the system.
  public static void ShutdownSystem () {
    if (initialized) {
      initialized = false;
      Shutdown();
    }
  }

  // Processes the next buffer.
  public static void Process (float[] data) {
    if (initialized) {
      Process(data.Length, data);
    }
  }

  public static void SetListenerPosition (Transform listener) {
    SetListenerPosition(Vec3FromUnityVector(listener.position));
  }

  public static void SetGravity (Vector3 gravity) {
    SetGravity(Vec3FromUnityVector(gravity));
  }

  public static int CreateParticle (Particle particle) {
    if(!initialized) {
      return -1;
    }
    Vec3 pos = Vec3FromUnityVector(particle.transform.position);
    Vec3 vel = Vec3FromUnityVector(particle.velocity);
    return CreateParticle(pos, vel, particle.recipMass, particle.radius, particle.restitution);
  }

  public static void DestroyParticle (Particle particle) {
    DestroyParticle(particle.Id);
  }

  public static int CreateSpring (Spring spring) {
    if(!initialized) {
      return -1;
    }
    return CreateSpring(spring.particleA.Id, spring.particleB.Id, spring.damping, spring.stiffness, 
                        spring.restLength);
  }

  public static void DestroySpring (Spring spring) {
    DestroySpring(spring.Id);
  }

  // Simple three-dimensional vector.
  [StructLayout(LayoutKind.Sequential)]
  private struct Vec3 {
    public float x;
    public float y;
    public float z;
  };

  private static Vec3 Vec3FromUnityVector (Vector3 unityVector) {
    Vec3 vec = new Vec3();
    vec.x = unityVector.x;
    vec.y = unityVector.y;
    vec.z = unityVector.z;
    return vec;
  }

  // Denotes whether the system is initialized.
  private static bool initialized = false;

#if UNITY_IOS
  private const string pluginName = "__Internal";
#else
  private const string pluginName = "rurataeunity";
#endif

  // System handlers.
  [DllImport(pluginName)]
  private static extern bool Initialize (int maxParticles, int maxSprings);

  [DllImport(pluginName)]
  private static extern void Shutdown ();

  [DllImport(pluginName)]
  private static extern void Process (int length, [In, Out]float[] output);

  [DllImport(pluginName)]
  private static extern void SetListenerPosition (Vec3 position);

  [DllImport(pluginName)]
  private static extern void SetGravity (Vec3 gravity);

  // Particle handlers.
  [DllImport(pluginName)]
  private static extern int CreateParticle (Vec3 position, Vec3 velocity, float recipMass,
                                            float radius, float restitution);

  [DllImport(pluginName)]
  private static extern void DestroyParticle (int particleId);

  // Spring handlers.
  [DllImport(pluginName)]
  private static extern int CreateSpring (int particleIdA, int particleIdB, float stiffness,
                                          float damping, float restlength);

  [DllImport(pluginName)]
  private static extern void DestroySpring (int springId);
}
