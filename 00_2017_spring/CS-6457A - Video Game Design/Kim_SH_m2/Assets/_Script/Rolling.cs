using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rolling : MonoBehaviour {
	public AudioClip[] clip;
	private AudioSource mySound;
	private float distToGround;
	private SphereCollider col;

	// Use this for initialization
	void Start () {
		mySound = GetComponent<AudioSource> ();
		col = GetComponent<SphereCollider> ();
		distToGround = col.bounds.extents.y;
//		Debug.Log (distToGround);
	}

	void OnEnable(){
		EventManager.StartListening ("Rolling", RollingSounds);
	}

	void OnDisable(){
		EventManager.StopListening ("Rolling", RollingSounds);
	}

//	void OnTriggerStay (Collider col){		
//		mySound.PlayOneShot (clip [Random.Range (0, 3)], Random.Range (0.1f, 0.3f));
//	}

	bool IsGounded(){
		return Physics.Raycast (transform.position, -Vector3.up, distToGround + 0.05f);
	}

	void RollingSounds(){
		if (IsGounded ()) {
			mySound.PlayOneShot (clip [Random.Range (0, 1)], Random.Range (0.1f, 0.3f));
		}
	}
}
