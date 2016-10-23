using UnityEngine;
using System.Collections;

// Ruratae particle.
public class Particle : MonoBehaviour {
  public Vector3 velocity = Vector3.zero;

  public float recipMass = 0.0f;

  public float radius = 1.0f;

  public float restitution = 0.0f;

  // Unique id.
  public int Id {
    get { return id; }
  }
  private int id = -1;

	void OnEnable () {
    id = Ruratae.CreateParticle(this);
	}

  void Start() {
    if(id == -1) {
      id = Ruratae.CreateParticle(this);
    }
  }

  void OnDisable() {
    if(id != -1) {
      Ruratae.DestroyParticle(this);
      id = -1;
    }
  }
	
	void Update () {
    // Update scale.
    transform.localScale = Vector3.Lerp(transform.localScale, radius * Vector3.one, 
                                        8 * Time.deltaTime);
	}
}
