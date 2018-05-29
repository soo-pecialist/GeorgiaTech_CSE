using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Wind : MonoBehaviour {
	public AudioClip[] clip;
	private AudioSource mySound;

	// Use this for initialization
	void Start () {
		mySound = GetComponent<AudioSource> ();
	}

	void OnEnable(){
		EventManager.StartListening ("Wind", WindSound);
	}

	void OnDisable(){
		EventManager.StopListening ("Wind", WindSound);
	}

	void WindSound(){
			mySound.PlayOneShot (clip [0], Random.Range (0.1f, 0.3f));
	}
}
