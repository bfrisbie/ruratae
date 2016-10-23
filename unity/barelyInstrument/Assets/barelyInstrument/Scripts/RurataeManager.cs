using UnityEngine;
using System.Collections;

public class RurataeManager : MonoBehaviour {
  // Maximum number of particles allowed.
  public int maxParticles = 32;

  // Maximum number of springs allowed.
  public int maxSprings = 32;

  // Listener center position.
  public Transform listener = null;

  // Gravity.
  public Vector3 gravity = Vector3.zero;

  void Awake () {
    Ruratae.InitializeSystem(maxParticles, maxSprings);
  }

  void OnDestroy () {
    Ruratae.ShutdownSystem();
  }

  void Update () {
    if (listener != null) {
      Ruratae.SetListenerPosition(listener);
    }
    Ruratae.SetGravity(gravity);
  }
}
