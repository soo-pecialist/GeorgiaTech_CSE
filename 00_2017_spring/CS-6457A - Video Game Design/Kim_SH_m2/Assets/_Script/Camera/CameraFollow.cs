using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollow : MonoBehaviour {
	public Transform target;
	public float smoothing = 5f;
	public GameObject player;

	private Animator anim;
	Vector3 offset;
	private Camera eyes;
	private	float defaultFOV;

	// Use this for initialization
	void Start () {
		anim = player.GetComponent<Animator> ();
		eyes = GetComponent<Camera> ();
		defaultFOV = eyes.fieldOfView;
		offset = transform.position - target.position; // Cam - target
	}

	// isMobing = true -> make sound 
	void LateUpdate () {
		if (!anim.GetBool("isRagDoll")) {
			Vector3 targetCamPos = target.position + offset;
			transform.position = Vector3.Lerp (transform.position, targetCamPos, smoothing * Time.deltaTime);
			if (Input.GetKey ("z")) {
				eyes.fieldOfView = Mathf.Lerp (eyes.fieldOfView, defaultFOV / 1.5f, .5f);
			} else if (Input.GetKey ("x")) {
				eyes.fieldOfView = Mathf.Lerp (eyes.fieldOfView, defaultFOV * 1.5f, .5f);
			} else {
				eyes.fieldOfView = defaultFOV;
			}
		}
	}
}
