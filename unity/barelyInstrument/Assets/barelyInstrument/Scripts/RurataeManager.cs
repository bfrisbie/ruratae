using UnityEngine;
using System.Collections;

public class RurataeManager : MonoBehaviour {
  // Particle prefab.
  public GameObject particlePrefab;

  // Spring prefab.
  public GameObject springPrefab;

  // Listener center position.
  public Transform listener = null;

  // Maximum number of particles allowed.
  public int maxParticles = 32;

  // Maximum number of springs allowed.
  public int maxSprings = 32;

  // Gravity.
  public Vector3 gravity = Vector3.zero;

  void Awake () {
    Ruratae.InitializeSystem(maxParticles, maxSprings);
  }

  void OnDestroy () {
    Ruratae.ShutdownSystem();
  }

  void Update () {
    Ruratae.SetListenerPosition(listener);
    Ruratae.SetGravity(gravity);
  }

  public Particle InstantiateParticle (Vector3 position) {
    GameObject particleObject = GameObject.Instantiate(particlePrefab, transform) as GameObject;
    Particle particle = particleObject.GetComponent<Particle>();
    particle.transform.position = position;
    return particle;
  }

  public void DestroyParticle (Particle particle) {
    GameObject.Destroy(particle.gameObject);
  }

  public Spring InstantiateSpring (Particle a, Particle b) {
    GameObject springObject = GameObject.Instantiate(springPrefab, transform) as GameObject;
    Spring spring = springObject.GetComponent<Spring>();
    spring.particleA = a;
    spring.particleB = b;
    spring.enabled = true;
    return spring;
  }

  public void DestroySpring (Spring spring) {
    GameObject.Destroy(spring.gameObject);
  }
}
