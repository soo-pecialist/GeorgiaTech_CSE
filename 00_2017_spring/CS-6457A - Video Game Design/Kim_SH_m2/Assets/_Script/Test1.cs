using System.Collections;
using System.Collections.Generic;
using UnityEngine.Events;
using UnityEngine;

public class Test1 : MonoBehaviour {

	private UnityAction someListener;

	void Awake (){
		someListener = new UnityAction (SomeFunction);
	}

	void OnEnable(){
		EventManager.StartListening ("test", someListener);
	}

	void OnDiable(){
		EventManager.StopListening ("test", someListener);
	}

	void SomeFunction(){
		Debug.Log ("Some Function was called!");
	}
}
