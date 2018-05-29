using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlasticConePlayer : MonoBehaviour {
	public AudioClip[] clip;
	private AudioSource mySound;
	private bool isHit = false;

	// Use this for initialization
	void Start () {
		mySound = GetComponent<AudioSource> ();
	}

	void OnEnable(){
		EventManager.StartListening ("PlasticCone", PlasticConeSound);
	}

	void OnDisable(){
		EventManager.StopListening ("PlasticCone", PlasticConeSound);
	}

	void OnTriggerStay (Collider col){
		if (col.tag == "Player") {
//			mySound.PlayOneShot (clip [Random.Range (0, 1)], Random.Range (0.1f, 0.15f));
			isHit = true;
		}
	}

	void PlasticConeSound(){
		if (isHit) {
			mySound.PlayOneShot (clip [Random.Range (0, 1)], Random.Range (0.1f, 0.3f));
			isHit = false;
		}
	}

	void Update(){
		Hit ();
	}


	public bool Hit(){
		return isHit;
	}


}
