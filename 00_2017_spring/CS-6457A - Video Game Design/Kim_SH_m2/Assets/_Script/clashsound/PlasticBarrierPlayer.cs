using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlasticBarrierPlayer : MonoBehaviour {
	public AudioClip[] clip;
	private AudioSource mySound;
	private bool isHit = false;

	void Start () {
		mySound = GetComponent<AudioSource> ();
	}

	void OnEnable(){
		EventManager.StartListening ("PlasticBar", PlasticBarSound);
	}

	void OnDisable(){
		EventManager.StopListening ("PlasticBar", PlasticBarSound);
	}


	void OnTriggerStay (Collider col){
		if (col.tag == "Player") {
//			mySound.PlayOneShot (clip [Random.Range (0, 2)], Random.Range (0.1f, 0.3f));
			isHit = true;
		}

	}

	void PlasticBarSound(){
		if (isHit) {
			mySound.PlayOneShot (clip [Random.Range (0, 2)], Random.Range (0.1f, 0.3f));
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
