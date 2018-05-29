using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BarrierWarningPlayer : MonoBehaviour {
	public AudioClip[] clip;
	private AudioSource mySound;

	// Use this for initialization
	void Start () {
		mySound = GetComponent<AudioSource> ();
	}

	void OnTriggerStay (Collider col){
		if (col.tag == "Player") {
			mySound.PlayOneShot (clip [Random.Range (0, 1)], 1f);
		}
	}
}
