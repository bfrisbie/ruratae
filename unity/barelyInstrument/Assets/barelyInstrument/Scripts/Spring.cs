using UnityEngine;
using System.Collections;

// Ruratae spring.
public class Spring : MonoBehaviour {
  public Particle particleA;

  public Particle particleB;

  public float stiffness = 0.0f;

  public float damping = 0.0f;

  public float restLength = 1.0f;

  // Unique id.
  public int Id {
    get { return id; }
  }
  private int id = -1;

  void OnEnable () {
    id = Ruratae.CreateSpring(this);
  }

  void Start() {
    if(id == -1) {
      id = Ruratae.CreateSpring(this);
    }
  }

  void OnDisable() {
    if(id != -1) {
      Ruratae.DestroySpring(this);
      id = -1;
    }
  }

  void Update () {
    // Update scale.
    transform.localScale = Vector3.Lerp(transform.localScale, restLength * Vector3.one, 
                                        8 * Time.deltaTime);
  }
}
